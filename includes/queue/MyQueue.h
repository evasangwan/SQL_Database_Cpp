#ifndef MYQUEUE_H
#define MYQUEUE_H
#include <iostream>
#include <cassert>
#include "linkedlistfunctionsTemplated.h"
using namespace std;

template <typename T>
class Queue{
public:
    class Iterator{
    public:
        friend class Queue;                               
        Iterator(){
            _ptr = NULL;
        }                      
        Iterator(node<T>* p){
            _ptr = p;      
        }                            
        T operator *(){
            assert(_ptr != nullptr && "dereference operator (*) called on null Iterator.");  
            return _ptr->_item;
        }    
        bool is_null(){
            return _ptr == nullptr;
        }            
        friend bool operator !=(const Iterator& left, const Iterator& right){  //true if left != right
            return left._ptr != right._ptr;
        }  
        friend bool operator ==(const Iterator& left, const Iterator& right){  //true if left == right
            return left._ptr == right._ptr;
        } 
        Iterator& operator++(){                         //member operator:++it  or ++it = new_value
            assert(_ptr != nullptr && "++() called on null iterator.");  
            _ptr = _ptr->_next;
            return *this;
        }
        friend Iterator operator++(Iterator& it, int unused){         //friend operator: it++
            assert(it._ptr != nullptr && "()++ called on null iterator.");
            Iterator hold = it;
            it._ptr = it._ptr->_next;
            return hold;
        }

    private:
        node<T>* _ptr;    //pointer being encapsulated
    };

    Queue():_front(nullptr), _rear(nullptr), _size(0){
        //initialized above
    }
    Queue(const Queue<T>& copyMe){
        _front = nullptr;
        _rear = _copy_list(_front, copyMe._front);
        _size = copyMe._size;
    }
    ~Queue(){
        _clear_list(_front);
        _size = 0;
    }
    Queue& operator=(const Queue<T>& RHS){
        if (&RHS == this){
            return *this;
        }
        _clear_list(_front);
        _front = nullptr;
        _rear = _copy_list(_front, RHS._front);
        _size = RHS._size;
        return *this;
    }

    bool empty(){
        return _front == nullptr;
    }
    T front(){
        return _front->_item;
    }
    T back(){
        return _rear->_item;
    }

    void push(T item){
         _rear = _insert_after(_front, _rear, item); 
         _size++;
    }
    T pop(){
        T item_ = _delete_node(_front, _front);
        if (_front == nullptr){
            _rear = nullptr;
        }
        if (_size == 1){
            _rear = _front;
        }
        _size--;
        return item_;
    }

    Iterator begin() const{                                     //Iterator to the head node
        return Iterator(_front);
    }                                  
    Iterator end() const{                                       //Iterator to NULL
        return Iterator(nullptr);
    }                                       
    void print_pointers(){
        _print_list(_front);
    }
    int size() const { return _size; }

    friend ostream& operator << (ostream& outs, const Queue<T>& printMe){
        _print_list(printMe._front);
        return outs;
    }
    
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
};


#endif



