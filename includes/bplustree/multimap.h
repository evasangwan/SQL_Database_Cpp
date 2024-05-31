#ifndef MULTIMAP_H
#define MULTIMAP_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include "bplustree.h"
#include "mpair.h"
#include "btree_array_funcs.h"
using namespace std;

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(){
            _it = nullptr;
        }
        Iterator(typename map_base::Iterator it){
            _it = it;
        }
        Iterator operator ++(int unused){
            Iterator hold = *this;
            ++(*this);
            return hold;
        }
        Iterator operator ++(){
            ++_it;
            return *this;
        }
        MPair<K, V> operator *(){
            if (!_it.is_null()) {
                return *_it;
            }
            return MPair<K, V>();
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
             return lhs._it != rhs._it;
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap(){

    }

//  Iterators
    Iterator begin(){
        return Iterator(mmap.begin());
    }
    Iterator end(){
        return Iterator(mmap.end());
    }

//  Capacity
    int size() const{
        return mmap.size();
    }
    bool empty() const{
        return mmap.empty();
    }

//  Element Access
    const vector<V>& operator[](const K& key) const{
        return mmap.get(key).value_list;
    }
    vector<V>& operator[](const K& key){
    //    MPair<K, V> mpair(key, V());
        // if (!mmap.contains(mpair)) {
        //     mmap.insert(mpair);
        // }
        return mmap.get(key).value_list;
    }

//  Modifiers
    void insert(const K& k, const V& v){
        // mmap.insert(MPair<K, V>(k, v));
        if (mmap.contains(MPair<K, V>(k))) {
            mmap.get(k).value_list.push_back(v);
        } 
        else{
            mmap.insert(MPair<K, V>(k, v));
        }
    }
    void erase(const K& key){
        mmap.remove(key);
    }
    void clear(){
        mmap.clear_tree();
    }

//  Operations:
    bool contains(const K& key) const{
        return mmap.contains(key);
    }
    vector<V> &get(const K& key){
       return mmap.get(key).value_list;
    }

    Iterator find(const K& key){
       return Iterator(mmap.find(key));
    }
    int count(const K& key){
        return -1;
    }
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    Iterator lower_bound(K num){
        return Iterator(mmap.lower_bound(num));
    }
    Iterator upper_bound(K num){
        return Iterator(mmap.upper_bound(num));
    }
    Iterator equal_range(K num){
        return Iterator(mmap.equal_range(num));
    }

   vector<V>& at(const K& key){
        // MPair<K, V> pair(num,V());
        return mmap.get(key).value_list;
    }

    bool is_valid(){
        return mmap.is_valid();
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs << print_me.mmap << endl;
        return outs;
    }
    void print_lookup(){
        //cout << mmap << endl;
        mmap.list_keys();
        // for (int i = 0; i < 3; i++){
        //     // cout << keys[i] << " : " << mmap[keys[i]] << endl;
            
        //     const K s = "command";
        //     mmap.list_keys
        //     cout << mmap.get(s).value_list << endl;
        // }
        
    }

private: 
    BPlusTree<MPair<K, V> > mmap;
};

#endif