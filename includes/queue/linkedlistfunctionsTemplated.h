#ifndef LINKEDLISTFUNCTIONSTEMPLATED_H
#define LINKEDLISTFUNCTIONSTEMPLATED_H
#include <iostream> 
#include "node.h"
#include <cassert>
using namespace std; 

template <typename T>
void _print_list(node<T>* head){
    node<T>* walker = head;
    cout << "Head->";
    while (walker != nullptr){
        cout << " <-[" << walker->_item << "]-> ";
        walker = walker->_next;
    }
    cout << " |||";
}

// //recursive fun! :)
// template <typename T>
// void _print_list_backwards(node<T> *head);

//return ptr to key or NULL
template <typename T> 
node<T>* _search_list(node<T>* head, T key){
    node<T>* walker = head; 
    while (walker != nullptr){
        if (walker->_item == key){
            return walker;
        }
        walker = walker->_next;
    }
    return nullptr;
}

template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this){
    node<T>* temp = new node<T>(insert_this);
    temp->_next = head;
    if (head != nullptr) {
        head->_prev = temp;
    }
    head = temp;
    return head;
}

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this){
    if (head == NULL){
        head = _insert_head(head,insert_this);
        return head;
    }
    node<T>*temp = new node<T>(insert_this);
    temp->_next = after_this->_next;
    if (after_this->_next != nullptr){
        after_this->_next->_prev = temp;
    }
    after_this->_next = temp;
    return temp; 
}

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this,T insert_this){
    if (before_this == head || before_this == nullptr){
        head = _insert_head(head,insert_this);
        return head;
    }
    node<T>*temp = new node<T>(insert_this);
    node<T>*prev = before_this->_prev;
    temp->_next = prev->_next;
    prev->_next = temp;
    before_this->_prev = temp;
    return temp;
}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this){
    if (prev_to_this == nullptr || prev_to_this->_prev == nullptr) { //either its null or its the first node in the list 
        return nullptr;
    }
    return prev_to_this->_prev;
}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this){
    assert(delete_this != nullptr && "delete_this cannot be nullptr");
    node<T>*temp = delete_this;
    if (delete_this == head){
        T item = delete_this->_item;
        if (head->_next == nullptr){
            head = nullptr;
        }
        else{
            head = head->_next;
            head->_prev = nullptr;
        } 
        delete temp;
        return item;
    }
    node<T>* prev = delete_this->_prev;
    prev->_next = delete_this->_next;
    if (delete_this->_next != nullptr){
        delete_this->_next->_prev = prev;
    }
    T item = delete_this->_item;
    delete temp;
    return item;
}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head){
    node<T>* dest = nullptr;
    node<T>* _last = _copy_list(dest, head); 
    return dest; //returns the head of the new list 
}

//duplicate the list, return pointer to last node in dest... 
//use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src){
    node<T>* src_walker = src;
    node<T>* dest_walker = dest; 
    while(src_walker != nullptr){
       dest_walker = _insert_after(dest, dest_walker, src_walker->_item);
       src_walker = src_walker->_next;
    }
    return dest_walker;
}

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head){
    node<T>*leader = head;
    while (leader != nullptr){
        node<T>*follower = leader;
        leader = leader->_next;
        delete follower;
    }
    head = nullptr;
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos){
    node<T>*walker = head;
    for (int i = 0; i < pos; i++){
        walker = walker->_next;
    }
    T item = walker->_item;
    return item;
}

#endif
// #ifndef LINKEDLIST
// #define LINKEDLIST
// #include <iostream>
// #include <iomanip>
// #include <cassert>
// #include "../node/node.h"
// using namespace std;

// //Linked List General Functions:
// template <typename T>
// void _print_list(node<T>* head){

// }

// //this function isnt necessary 
// // //recursive fun! :)
// // template <typename T>
// // void _print_list_backwards(node<T> *head);

// //return ptr to key or NULL
// template <typename T>
// node<T>* _search_list(node<T>* head,
//                             T key){
// return nullptr;
//                             }


// template <typename T>
// node<T>* _insert_head(node<T> *&head,
//                             T insert_this){
// return nullptr;
//                             }

// //insert after ptr: insert head if marker null
// template <typename T>
// node<T>* _insert_after(node<T>*& head,
//                                 node<T> *after_this,
//                                 T insert_this){
// return nullptr;
//                                 }

// //insert before ptr: insert head if marker null
// template <typename T>
// node<T>* _insert_before(node<T>*& head,
//                                 node<T>* before_this,
//                                 T insert_this){
// return nullptr;
//                                 }

// //ptr to previous node
// template <typename T>
// node<T>* _previous_node(node<T>* prev_to_this){
//     return nullptr;
// }

// //delete, return item
// template <typename T>
// T _delete_node(node<T>*& head, node<T>* delete_this){
//     return delete_this->_item;
// }

// //duplicate the list...
// template <typename T>
// node<T>* _copy_list(node<T>* head){
//     return nullptr;
// }

// //duplicate the list, return pointer to last node in dest... 
// //     use this function in your queue big three 
// template <typename T>
// node<T> *_copy_list(node<T> *&dest, node<T> *src){
//     return nullptr;
// }

// //delete all the nodes
// template <typename T>
// void _clear_list(node<T>*& head){

// }

// //_item at this position
// template <typename T>
// T& _at(node<T>* head, int pos){
//     return head->_item;
// }

// #endif