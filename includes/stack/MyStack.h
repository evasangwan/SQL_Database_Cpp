#ifndef MYSTACK_H
#define MYSTACK_H
#include "../queue/linkedlistfunctionsTemplated.h"
#include <cassert>
using namespace std;

template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                           
        Iterator(){
            _ptr = NULL;
        }                      
        Iterator(node<ITEM_TYPE>* p){
            _ptr = p;      
        }                            
        ITEM_TYPE operator *(){
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
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    };

    Stack():_top(nullptr),_size(0){
        //initialized up
    }
    Stack(const Stack<ITEM_TYPE>& copyMe){
        _top = nullptr;
        _top = _copy_list(copyMe._top);
        _size = copyMe._size;
    }
    ~Stack(){
        _clear_list(_top);
        _size = 0;
    }
    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS){
        if (&RHS == this){
            return *this;
        }
        _clear_list(_top);
        _top = _copy_list(RHS._top);
        _size = RHS._size;
        return *this;
    }
    ITEM_TYPE top(){
        return _top->_item;
    }
    bool empty(){
        return _top == nullptr;
    }
    void push(ITEM_TYPE item){
        _top = _insert_head(_top,item);
        _size++;
    }
    ITEM_TYPE pop(){
        assert(_top && "trying to pop nullptr");
        ITEM_TYPE item = _delete_node(_top,_top);
        _size--;
        return item;
    }

    template<typename T>
    friend ostream& operator<<(ostream& outs, const Stack<T>& printMe){
        _print_list(printMe._top);
        return outs;
    }
    Iterator begin() const{                   //Iterator to the head node
        return Iterator(_top);
    }                
    Iterator end() const{                     //Iterator to NULL
        return Iterator(nullptr);
    }               
    int size() const { return _size; }

private:
    node<ITEM_TYPE>* _top;
    int _size;
};

#endif