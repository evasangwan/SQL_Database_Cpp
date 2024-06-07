#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include <vector> 
#include "btree_array_funcs.h"
using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
       
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){

        }

        T operator *(){
            // assert(it && "trying to dereference nullptr");
            T item = it->data[key_ptr];
            return item;
        }

        Iterator operator++(int un_used){
            Iterator hold = *this;
            ++(*this);
            return hold;
        }
        Iterator operator++(){
            key_ptr++;
            if(key_ptr == it->data_count){
                key_ptr = 0;
                it = it->next;
            }
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return !(lhs==rhs);
        }
        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){
            return !it;
        }
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false){
        data_count = 0;
        child_count = 0;
        next = nullptr;
    }
    BPlusTree(T *a, int size, bool dups = false){
        data_count = 0;  
        child_count = 0; 
        next = nullptr;
        for (int i = 0; i < size; i++) {
            insert(a[i]); 
        }
    }

    //big three:
    BPlusTree(const BPlusTree<T>& other){
        copy_tree(other);
    }
    ~BPlusTree(){
        clear_tree();
    }
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS){
        if (&RHS == this){
            return *this;
        }
        clear_tree();
        copy_tree(RHS);
        return *this;
    }

    void copy_tree(const BPlusTree<T>& other){
        BPlusTree<T>* last = nullptr;
        copy_tree(other,last);
    }  //copy other into this object

    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node){
        data_count = other.data_count;
        child_count = other.child_count;
        for (int i = 0; i < other.data_count; i++){
            data[i] = other.data[i];
        }
        if (other.is_leaf()){
            if (last_node){
                last_node->next = this;
                last_node = this;
            }
            else{ //if its nullptr
                last_node = this;
            }
            next = nullptr;
        }
        else{
            for (int j = 0; j < other.child_count; j++){
                BPlusTree<T>*sub = new BPlusTree();
                subset[j] = sub;
                BPlusTree<T>* ptr = other.subset[j];
                if (ptr){
                    sub->copy_tree(*ptr,last_node);
                }
            }
        }
    }

    void insert(const T& entry){  //insert entry into the tree
        bool found = false;
        Iterator it = find(entry);
        if (!it.is_null()){
            found = true;
        }
        if (found){
            *it = entry;
        }
        else{
            loose_insert(entry);
            // if (next == nullptr){
            //     cout << "next is null"<<endl;
            // }
            if (data_count > MAXIMUM){
                BPlusTree<T>* grow = new BPlusTree<T>();
                BPlusTree<T>* root = this;
                for (int i = 0; i < data_count; i++){
                    grow->data[i] = root->data[i];
                    grow->subset[i] = root->subset[i];
                }
                grow->subset[data_count] = root->subset[data_count];
                grow->data_count = root->data_count;
                grow->child_count = root->child_count;
                //grow->next = root->next;
                root->data_count = 0;
                root->child_count = 1;
                root->subset[0] = grow;
                root->fix_excess(0);
                // root->next = nullptr;
            }
        }
    }  

    void remove(const T& entry){ //remove entry from the tree
        Iterator found = find(entry);
            if (!found.is_null()){
                loose_remove(entry);
                if (data_count < MINIMUM && child_count > 0){
                    //cout << "here lastly"<<endl;
                    BPlusTree* shrink_ptr = subset[0];
                    data_count = shrink_ptr->data_count;
                    child_count = shrink_ptr->child_count;
                    for (int i = 0; i < data_count; i++){
                        data[i] = shrink_ptr->data[i];
                        subset[i] = shrink_ptr->subset[i];
                    }
                    subset[data_count] = shrink_ptr->subset[data_count];
                    shrink_ptr->data_count = 0;
                    shrink_ptr->child_count = 0;
                    delete shrink_ptr;
                    shrink_ptr = nullptr;
                }
            }
            else{

            }
    }   
    void clear_tree(){
        if (!is_leaf()){
            for (int i = 0; i < child_count; i++){
                subset[i]->clear_tree();
                delete subset[i];
                subset[i] = nullptr;
            }
        }
        data_count = 0;
        child_count = 0; 
        // next = nullptr;
    }//clear this object (delete all nodes etc.)

    bool contains(const T& entry) const{
        int i = first_ge(data,data_count,entry);
        bool found = false;
        if (i < data_count){
            found = (data[i] == entry);
        }
        if (found && is_leaf()){
            return true;
        }
        if (found && !is_leaf()){
            return subset[i+1]->contains(entry);
        }
        else if(!is_leaf()){
            return subset[i]->contains(entry);
        } 
        else{
            return false;
        }   
    } //true if entry can be found

    T& get(const T& entry){
        // int i = first_ge(data,data_count,entry);
        // bool found = false;
        // if (i < data_count){
        //     found = (data[i] == entry);
        // }
        // if (found && is_leaf()){
        //     T* ptr = &data[i];
        //     return *ptr;
        //     // return data[i];
        // }
        // if (found && !is_leaf()){
        //     return subset[i+1]->get(entry);
        // }
        // else if(!is_leaf()){
        //     return subset[i]->get(entry);
        // }   
        if (!contains(entry))
            insert(entry);

        return get_existing(entry);
    } //return a reference to entry

    const T& get(const T& entry)const{
        int i = first_ge(data,data_count,entry);
        bool found = false;
        if (i < data_count){
            found = (data[i] == entry);
        }
        if (found && is_leaf()){
            const T* ptr = &data[i];
            return *ptr;
        }
        if (found && !is_leaf()){
            return subset[i+1]->get(entry);
        }
        else if(!is_leaf()){
            return subset[i]->get(entry);
        }   
        // if (!contains(entry)){
        //     insert(entry);
        // }
        // return get_existing(entry);
    }   //return a reference to entry

    T& get_existing(const T& entry){
        int i = first_ge(data,data_count,entry);
        bool found = false;
        if (i < data_count){
            found = (data[i] == entry);
        }
        if (found && is_leaf()){
            T* ptr = &data[i];
            return *ptr;
            // return &data[i];
        }
        if (found && !is_leaf()){
            return subset[i+1]->get_existing(entry);
        }
        else if(!is_leaf()){
            return subset[i]->get_existing(entry);
        }   
        //shouldnt ever come here!!
        assert(false);
    }     //return a reference to entry

    Iterator find(const T& key){
        int i = first_ge(data,data_count,key);
        // BPlusTree<T>* ptr = this;
        bool found = false;
        if (i < data_count){
            found = (data[i] == key);
        }
        if (found && is_leaf()){
            BPlusTree<T>* ptr = this;
            // cout << "ptr " << *ptr << endl;
            return Iterator(ptr,i);
        }
        else if (found && !is_leaf()){
            return subset[i+1]->find(key);
        }
        else if(!is_leaf()){
            return subset[i]->find(key);
        } 
        else{
            return Iterator();
        }
    } //return an iterator to this key. NULL if not there.
 

    Iterator equal_range(T num) {
        return Iterator();
    }

    Iterator lower_bound(const T& key){
        BPlusTree<T>* ptr = this;
        while (ptr && !ptr->is_leaf()){
            int i = first_ge(ptr->data, ptr->data_count, key);
            ptr = ptr->subset[i];
        }
        int i = first_ge(ptr->data, ptr->data_count, key);
        if (i < ptr->data_count &&  ptr->data[i] >= key){   //!(ptr->data[i] < key) ptr->data[i] >= key 
            return Iterator(ptr,i);
        }
        else{
            if (ptr->next){
                return Iterator(ptr->next,0);
            }
            else{
                return Iterator();
            }
        }
    }  //return first that goes NOT BEFORE key entry or next if does not exist: >= entry

    Iterator upper_bound(const T& key){
        BPlusTree<T>*ptr = this;
        while (ptr && !ptr->is_leaf()){
            int i = first_ge(ptr->data,ptr->data_count, key);
            ptr = ptr->subset[i];
        }
        int i = first_ge(ptr->data,ptr->data_count,key);
        if (i < ptr->data_count && ptr->data[i] > key){  
            return Iterator(ptr,i);
        }
        else{
            i++;
            if (i < ptr->data_count && ptr->data[i] > key){  
                return Iterator(ptr,i);
            }
            else if (ptr->next){
                return Iterator(ptr->next,0);
            }
            else{
                return Iterator();
            }
        }
    }  //return first that goes AFTER key exist or not, the next entry  >entry

    int size() const{
        return -1;
    }                    //count the number of elements

    bool empty() const{
        return data_count == 0 && child_count == 0;
    }  //true if the tree is empty

    void print_tree(int level = 0, ostream &outs=cout) const{
        if (is_leaf()){
            for (int i = data_count - 1; i >= 0; i--){
                outs << setw(level*6);
                outs << "[" << data[i] << "]" << endl;
            }
        }
        else if (!is_leaf()){
            subset[child_count-1]->print_tree(level + 1, outs);
            for (int i = data_count - 1; i >= 0; --i){
                outs << setw(level*6);
                outs << "[" << data[i] << "]" << endl;
                subset[i]->print_tree(level + 1, outs);
            }
        }
    }
    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid(){
        return false;
    }
    string in_order(){
        string s = "";
        if (is_leaf()){
            string leaf = "";
            for (int i = 0; i < data_count; i++){
                leaf += to_string(data[i]) + "|";
            }
            return leaf;
        }
        else if (!is_leaf()){
            for (int i = 0; i < data_count; i++){
                s+= subset[i]->in_order();
                s+= to_string(data[i]) + "|";
            }
            s+= subset[data_count]->in_order();
        }                                  
        return s;    
    }
    string pre_order(){
        string s = "";
        if (is_leaf()){
            string leaf = "";
            for (int i = 0; i < data_count; i++){
                leaf += to_string(data[i]) + "|";
            }
            return leaf;
        }
        else if (!is_leaf()){
            for (int i = 0; i < data_count; i++){
                s+= to_string(data[i]) + "|";
                s+= subset[i]->pre_order();
            }
            s+= subset[data_count]->pre_order();
        }                                  
        return s;   
    }
    string post_order(){
        string s = "";
        if (is_leaf()){
            string leaf = "";
            for (int i = 0; i < data_count; i++){
                leaf += to_string(data[i]) + "|";
            }
            return leaf;
        }
        else if (!is_leaf()){
            int num = 0;
            for (int k = 0; k < data_count; k++){
            for (int i = num; i <= k+1; i++){
                s+= subset[i]->post_order();
                num++;
            }
            s+= to_string(data[k]) + "|";
            }
        }                                  
        return s;   
    }
    Iterator begin(){
        // return Iterator(get_smallest_node());
        BPlusTree<T>* smallestNode = get_smallest_node();
        if (smallestNode){
            return Iterator(smallestNode, 0); 
        } 
        else{
            return Iterator(); 
        }

    }
    Iterator end(){
        return Iterator(nullptr,0);
    }
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout << setw(11) << *it << endl;
        return cout;
    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next;
    bool is_leaf() const{
        return child_count==0;
    }  //true if this is a leaf node

    T* find_ptr(const T& entry){
        int i = first_ge(data,data_count,entry);
        bool found = false;
        if (i < data_count){
            found = (data[i] == entry);
        }
        if (found && is_leaf()){
            T* ptr = &data[i];
            return ptr;
        }
        if (found && !is_leaf()){
            return subset[i+1]->find(entry);
        }
        else if(!is_leaf()){
            return subset[i]->find(entry);
        } 
        else{
            return nullptr;
        }
    }         //return a pointer to this key.
            // NULL if not there.

    //insert element functions
    void loose_insert(const T& entry){   //allows MAXIMUM+1 data elements in the root    
        bool found = false;
        Iterator it = find(entry);
        if (!it.is_null()){
            found = true;
        } 
        if (found && is_leaf()){
            //its a duplicate!
        }   
        else if (found && !is_leaf()){
            int i = first_ge(data,data_count,entry);
            subset[i]->loose_insert(entry);
            if (subset[i]->data_count > MAXIMUM){
                fix_excess(i);
            }
        } 
        else if(is_leaf()){
            ordered_insert(data,data_count,entry);
            child_count = 0;
            //next = nullptr;
        }  
        else if (!is_leaf()){
            int i = first_ge(data,data_count,entry);
            subset[i]->loose_insert(entry);
            if (subset[i]->data_count > MAXIMUM){
                fix_excess(i);
            }
        }           
    }   


    void fix_excess(int i){
        // if (subset[i]->!is_leaf()){
            BPlusTree* node = new BPlusTree<T>();
            //node->next = nullptr;
            child_count++;   
            split(subset[i]->data,subset[i]->data_count,node->data,node->data_count);
            split(subset[i]->subset,subset[i]->child_count,node->subset,node->child_count);
            T item;
            delete_item(subset[i]->data,subset[i]->data_count-1,subset[i]->data_count, item);
            //data_count++;
            // data[data_count-1] = item;
            ordered_insert(data,data_count,item);
            for (int j = data_count; j > (i+1); j--) {
            subset[j] = subset[j-1];
            }
            subset[i+1] = node;   
        // }
        if (subset[i]->is_leaf()){
            // subset[i+1]->next = nullptr;
            // insert_item(subset[i+1]->data,0,subset[i+1]->data_count,item);
            // subset[i]->next = subset[i+1];
            insert_item(subset[i+1]->data,0,subset[i+1]->data_count,item);
            subset[i+1]->next = subset[i]->next;
            subset[i]->next = subset[i+1];
            // if (i+1 == data_count){
            //     subset[i+1]->next = nullptr;
            // }
        }
        
    } //fix excess in child i

    //remove element functions:
    void loose_remove(const T& entry){
            int i = first_ge(data,data_count,entry);
            bool found = false;
            if (i < data_count && data[i]==entry){
                found = true;
            }
            if (found){
                if (is_leaf()){
                    delete_item(data,i,data_count,data[i]);
                }
                else if(!is_leaf()){
                    subset[i+1]->loose_remove(entry);
                    //cout << "in subsi+1"<<endl;
                    //cout << data[0] << "| ";
                    if (subset[i+1]->data_count < MINIMUM){
                        //cout << "i in fix shortage is "<< i+1 << endl;
                        fix_shortage(i+1);
                        for (int a = 0; a < data_count; a++){
                            if (data[a] == entry){
                                subset[i+1]->get_smallest(data[a]);
                                break;
                            }
                        }
                        // if (subset[i]){
                            for (int a = 0; a < subset[i]->data_count; a++){
                                if (subset[i]->data[a] == entry){
                                    //subset[i+1]->get_smallest(subset[i]->data[a]);
                                    subset[i]->subset[a+1]->get_smallest(subset[i]->data[a]);
                                    break;
                                }
                            }
                        // }
                        if ((i+1) <= child_count-1){
                            for (int a = 0; a < subset[i+1]->data_count; a++){
                                if (subset[i+1]->data[a] == entry){
                                    subset[i+1]->subset[a+1]->get_smallest(subset[i+1]->data[a]);
                                    break;
                                }
                            }
                        }
                    }
                    for (int a = 0; a < data_count; a++){
                        if (data[a] == entry){
                            subset[i+1]->get_smallest(data[a]);
                            break;
                        }
                    }
                }
            }
            else if (!is_leaf()){
                subset[i]->loose_remove(entry);
                if (subset[i]->data_count < MINIMUM){
                    fix_shortage(i);
                }
            }   
            else{
                return;
            }
    }  //allows MINIMUM-1 data elements
    //  in the root


    BPlusTree<T>* fix_shortage(int i){
        if (i < data_count && subset[i+1]->data_count > MINIMUM){
            //cout << "transfer left "<<endl;
            transfer_left(i);
        }
        else if (i > 0 && subset[i-1]->data_count > MINIMUM){
            //cout << "transfer right "<<endl;
            transfer_right(i);
        }
        else if (i < data_count){
            //cout << "merge i "<<endl;
            merge_with_next_subset(i); //the one in the right merges left 
        }
        else if (i > 0){
            //cout << "merge i-1 "<<endl;
            merge_with_next_subset(i-1); //merge to the left 
        }
    }  //fix shortage in child i
    // and return the smallest key in this subtree

    BPlusTree<T>* get_smallest_node(){
        BPlusTree<T>* ptr = this;
        if (!is_leaf()){
            return ptr->subset[0]->get_smallest_node();
        }
        return ptr;
    }
    void get_smallest(T& entry){
        // if(!is_leaf()){
        //     subset[0]->get_smallest(entry);
        // }
        // entry = data[0];
        
        // cout << "entry is " << entry << endl;
        BPlusTree<T>* ptr = this;
        BPlusTree<T>* smallest = ptr->get_smallest_node();
        entry = smallest->data[0];
        //cout << "entry is " << entry << endl;
    }      //entry := leftmost leaf
    void get_biggest(T& entry){

    }       //entry := rightmost leaf
    void remove_biggest(T& entry){

    }    //remove the biggest child of tree->entry
    void transfer_left(int i){
        if (subset[i]->is_leaf()){
            T entry; 
            delete_item(subset[i+1]->data,0,subset[i+1]->data_count,entry);
            attach_item(subset[i]->data,subset[i]->data_count,entry);
            //insert_item(subset[i]->data,0,subset[i]->data_count,entry);
            if (i > 0){
                data[i-1] = subset[i]->data[0];
            }
            data[i] = subset[i+1]->data[0];
            // data[i-1] = entry;
        }
        else{
            attach_item(subset[i]->data,subset[i]->data_count,data[i]);
            if(!subset[i]->is_leaf()){
                 subset[i]->child_count = subset[i]->data_count+1;
            }
            subset[i]->subset[subset[i]->data_count] = subset[i+1]->subset[0];
            T detached_item;
            delete_item(subset[i+1]->data,0,subset[i+1]->data_count,detached_item);
            if(!subset[i+1]->is_leaf()){
                subset[i+1]->child_count = subset[i+1]->data_count+1;
            }
            for (int j = 0; j <= subset[i+1]->data_count; j++){
                subset[i+1]->subset[j] = subset[i+1]->subset[j+1];
            }
            data[i] = detached_item;
        }
    }        //transfer one element LEFT from child i
    void transfer_right(int i){
        if (subset[i]->is_leaf()){
            T entry;
            detach_item(subset[i-1]->data,subset[i-1]->data_count,entry);
            data[i-1] = entry;
            insert_item(subset[i]->data,0,subset[i]->data_count,entry);
        }
        else{
            insert_item(subset[i]->data,0,subset[i]->data_count,data[i-1]);
            if(!subset[i]->is_leaf()){
                subset[i]->child_count = subset[i]->data_count+1;
            }
            for (int j = subset[i]->data_count; j > 0; j--) {
                subset[i]->subset[j] = subset[i]->subset[j-1];
            }
            subset[i]->subset[0] = subset[i-1]->subset[subset[i-1]->data_count];
            T detached_item;
            detach_item(subset[i-1]->data,subset[i-1]->data_count,detached_item);
            if(!subset[i-1]->is_leaf()){
                subset[i-1]->child_count = subset[i-1]->data_count+1;
            }
            data[i-1] = detached_item;
        }
    }       //transfer one element RIGHT from child i

    BPlusTree<T> *merge_with_next_subset(int i){
        if(subset[i]->is_leaf()){
            subset[i]->next = subset[i+1]->next;
            merge(subset[i]->data,subset[i]->data_count,subset[i+1]->data,subset[i+1]->data_count);
            T entry;
            delete_item(data,i,data_count,entry);
            for (int j = i+1; j < child_count; j++){
                subset[j] = subset[j+1];
            }
            child_count--;
            return nullptr;
        } //merge subset i with  i+1
        else{
            T entry;
            delete_item(data,i,data_count,entry);
            ordered_insert(subset[i+1]->data,subset[i+1]->data_count,entry);
            BPlusTree<T>* temp = subset[i+1];
            merge(subset[i]->data,subset[i]->data_count,subset[i+1]->data,subset[i+1]->data_count);
            merge(subset[i]->subset,subset[i]->child_count,subset[i+1]->subset,subset[i+1]->child_count);
            
            //cout << "child count " << child_count << endl;
            for (int j = i+1; j < child_count; j++){
                subset[j] = subset[j+1];
            }
            delete temp;
            temp = nullptr;
            if(!is_leaf()){
                child_count = data_count+1;
            }
            return nullptr;
        }
    }

};


#endif