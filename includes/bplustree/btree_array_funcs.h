#ifndef BTREE_ARRAY_FUNCS_H
#define BTREE_ARRAY_FUNCS_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include <vector>
using namespace std;

template <class T>
T maximal(const T& a, const T& b){
    if (a > b){
        return a;
    }
    else if (b > a){
        return b;
    }
    else{ //if theyre equal 
        return a;
    }
}                      //return the larger of the two items

template <class T>
void swap(T& a, T& b){
    T c = a;
    a = b;
    b = c;
}  //swap the two items

template <class T>
int index_of_maximal(T data[ ], int n){
    T max = data[0];
    int max_index = 0;
    for (int i = 0; i < n; i++){
        if (data[i] > max){
            max = data[i];
            max_index = i;
        }
    }
    return max_index;
}                 //return index of the largest item in data

template <class T>
int first_ge(const T data[ ], int n, const T& entry){   //return the first element in data that is
    for (int i = 0; i < n; i++){
        // cout << "[" << data[i] << "] ";
        if (data[i] >= entry){
            // cout << "[" << data[i] << "] ";
            // cout << "[" << data[i].key << "] ";
            return i;
        }
    }
    return n;                                           //not less than entry
}
template <class T>
void ordered_insert(T data[ ], int& n, T entry){
    int index = first_ge(data,n,entry);
    n++;
    for (int i = n-1; i > index; i--){
        data[i] = data[i-1];
    }
    data[index] = entry;
}        //insert entry into the sorted array
         //data with length n

template <class T>
void attach_item(T data[ ], int& n, const T& entry){
    n++;
    data[n-1] = entry;
}    //append entry to the right of data

template <class T>
void insert_item(T data[ ], int i, int& n, T entry){
    n++;
    for (int j = n-1; j > i; j--){
        data[j] = data[j-1];
    }
    data[i] = entry;
}    //insert entry at index i in data

template <class T>
void detach_item(T data[ ], int& n, T& entry){
    entry = data[n-1];
    n--;
}          //remove the last element in data and place
           //it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){
    entry = data[i];
    for (int j = i; j < n-1; j++){
        data[j] = data[j+1];
    }
    n--;
}   //delete item at index i and place it in entry

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){
    if (n2 > 0){
        int starting = n1;
        n1 += n2;
        n2 = 0;
        int j = 0;
        for (int i = starting; i < n1; i++){
            data1[i] = data2[j];
            j++;
        }
    }
}   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){
    int to_copy = n1 / 2;
    int index = n1 - to_copy;
    n2 += to_copy;
    int z = 0;
    for (int i = index; i < n1; i++){
        data2[z] = data1[i];
        z++;
    }
    n1 -= to_copy;
}   //move n/2 elements from the right of data1
    //and move to data2


template <class T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size){
    dest_size = src_size;
    for (int i = 0; i < src_size; i++){
        dest[i] = src[i];
    }
}              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = -1){
    cout << "[";
    for (int i = 0; i < n; i++){
        cout << data[i] << " ";
    }
    cout << "]";
}  //print array data

template <class T>
bool is_gt(const T data[], int n, const T& item){
    for (int i = 0; i < n; i++){
        if (data[i] >= item){
            return false;
        }
    }
    return true;
}       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item){
    for (int i = 0; i < n; i++){
        if (data[i] < item){
            return false;
        }
    }
    return true;
}       //item <= all data[i]

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list){
    for(int i =0; i < list.size();i++){
        outs << list[i] << " ";
    }
    return outs;
}//print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){
    list.push_back(addme);
    return list;
} //list.push_back addme

#endif