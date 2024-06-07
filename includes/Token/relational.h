// #ifndef RELATIONAL_H
// #define RELATIONAL_H
// #include <iostream>
// #include <iomanip>
// #include <cmath>
// #include <string>
// #include <cstring>
// #include <cassert>
// #include <vector>
// #include "operator.h"
// using namespace std;

// class Relational: public Operator{
//     public:
//     Relational(const string& s):_rel(s){

//     }
//     int getType(){
//         return RELATIONAL;
//     }
//     virtual string Op() const override{
//         return _rel;
//     }
//     virtual ostream& print(ostream& outs)const override{
//             outs << "[" << _rel << "]";
//             return outs; 
//     }
//     virtual double getPrecedence() override{
//             return 3;
//     }
//     vector<long> eval(MMap<string, long>& mmap, const string& val){
//         result.clear();
//         vector<vector<long>> recs;
//         if (_rel == "="){
//             // cout << "in = " << endl; 
//             //cout << mmap << endl;
//             MMap<string,long>::Iterator found = mmap.find(val);
//             //cout << *found << endl;
//             if (found != mmap.end() && (*found).key == val){
//                 recs.push_back((*found).value_list);
//             }
//         } 
//         else if (_rel == "<"){  
//             // cout << "in < " << endl; 
//             // cout << "va " << val << endl; 
//             // cout << mmap << endl; 
//             MMap<string,long>::Iterator end = mmap.lower_bound(val);
//             //cout << *end << endl;
//             MMap<string,long>::Iterator begin = mmap.begin();
//             //cout << "------------------------"<<endl;
//             //cout << mmap << endl;
//             while (begin != mmap.end() && begin!=end && (*begin).key < val){
//                 //cout << *begin << endl;
//                 recs.push_back((*begin).value_list);
//                 begin++;
//                 // cout << *begin << endl;
//                 // cout << *end << endl; 
//             }
//         } 
//         else if (_rel == ">"){  
//             // cout << "in > " << endl; 
//             MMap<string,long>::Iterator begin = mmap.upper_bound(val);
//             MMap<string,long>::Iterator end = mmap.end();
//             while(begin != end && (*begin).key > val){
//                 recs.push_back((*begin).value_list);
//                 begin++;
//             }
//         } 
//         else if (_rel == ">="){
//             // cout << "in >= " << endl;
//             MMap<string,long>::Iterator begin = mmap.lower_bound(val);
//             MMap<string,long>::Iterator end = mmap.end();
//             while (begin != end && (*begin).key >= val){
//                 recs.push_back((*begin).value_list);
//                 begin++;
//             }
//         } 
//         else if (_rel == "<="){    
//             // cout << "<= " << endl; 
//             MMap<string,long>::Iterator end = mmap.upper_bound(val);
//             MMap<string,long>::Iterator begin = mmap.begin();
//             while (begin != mmap.end() && begin!= end && (*begin).key <= val){
//                 recs.push_back((*begin).value_list);
//                 begin++;
//             }
//         }

//         for (int i = 0; i < recs.size(); i++){
//             vector<long> temp = recs[i];
//             for (int j = 0; j < temp.size(); j++){
//                 result.push_back(temp[j]);
//             }
//         }
//         return result;  
//     }

//     private:
//         string _rel;
//         vector<long> result;
// };

// #endif

// #ifndef RELATIONAL_H
// #define RELATIONAL_H
// #include <iostream>
// #include <iomanip>
// #include <cmath>
// #include <string>
// #include <cstring>
// #include <cassert>
// #include <vector>
// #include "operator.h"
// using namespace std;

// class Relational: public Operator{
//     public:
//     Relational(const string& s):_rel(s){

//     }
//     int getType(){
//         return RELATIONAL;
//     }
//     virtual string Op() const override{
//         return _rel;
//     }
//     virtual ostream& print(ostream& outs)const override{
//             outs << "[" << _rel << "]";
//             return outs; 
//     }
//     virtual double getPrecedence() override{
//             return 3;
//     }
//     vector<long> eval(MMap<string,long> &mmap, const string &val){
//         vector<vector<long>> recs;
//         if (_rel == "="){
//             // cout << val <<endl;
//             // recs.push_back(mmap.get(val));
//             MMap<string,long>::Iterator iter = mmap.begin();
//             while (iter != mmap.end()){
//                 if ((*iter).key == val ){
//                     recs.push_back((*iter).value_list);
//                 }
//                 iter++;
//             }
//         }

//         if (_rel == "<"){
//             MMap<string,long>::Iterator iter = mmap.begin();
//             while (iter != mmap.end()){
//                 if ((*iter).key < val ){
//                     recs.push_back((*iter).value_list);
//                 }
//                 iter++;
//             }
//         }

//         if (_rel == ">"){
//             MMap<string,long>::Iterator iter = mmap.begin();
//             while (iter != mmap.end()){
//                 if ((*iter).key > val){ 
//                     recs.push_back((*iter).value_list);
//                 }
//                 iter++;
//             }
//         }

//         if (_rel == ">="){
//             MMap<string,long>::Iterator iter = mmap.begin();
//             while (iter != mmap.end()){
//                 if ((*iter).key >= val){
//                     recs.push_back((*iter).value_list);
//                 }
//                 iter++;
//             }
//         }

//         if (_rel == "<="){
//             MMap<string,long>::Iterator iter = mmap.begin();
//             while (iter != mmap.end()){
//                 if ((*iter).key <= val){
//                     recs.push_back((*iter).value_list);
//                 }
//                 iter++;
//             }
//         }

//         for (int i = 0; i < recs.size(); i++){
//             vector<long> temp = recs[i];
//             for (int j = 0; j < temp.size(); j++){
//                 result.push_back(temp[j]);
//             }
//         }
//         return result;        
//     }
//     private:
//         string _rel;
//         vector<long> result;

// };

// #endif


#ifndef RELATIONAL_H
#define RELATIONAL_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include <vector>
#include "operator.h"
using namespace std;

class Relational: public Operator{
    public:
    Relational(const string& s):_rel(s){

    }
    int getType(){
        return RELATIONAL;
    }
    virtual string Op() const override{
        return _rel;
    }
    virtual ostream& print(ostream& outs)const override{
            outs << "[" << _rel << "]";
            return outs; 
    }
    virtual double getPrecedence() override{
            return 3;
    }
    vector<long> eval(MMap<string,long> &mmap, const string &val){
        vector<vector<long>> recs;
        if (_rel == "="){
            // cout << val <<endl;
            // recs.push_back(mmap.get(val));
            MMap<string,long>::Iterator iter = mmap.find(val);
            if (iter != mmap.end()){
                // if ((*iter).key == val ){
                    recs.push_back((*iter).value_list);
                // }
                // iter++;
            }
        }

        if (_rel == "<"){
            MMap<string,long>::Iterator iter = mmap.begin();
            MMap<string,long>::Iterator end = mmap.lower_bound(val);
            while (iter != mmap.end() && iter!= end){
                if ((*iter).key < val ){
                    recs.push_back((*iter).value_list);
                }
                iter++;
            }
        }

        if (_rel == ">"){
            MMap<string,long>::Iterator iter = mmap.begin();
            while (iter != mmap.end()){
                if ((*iter).key > val){ 
                    recs.push_back((*iter).value_list);
                }
                iter++;
            }
        }

        if (_rel == ">="){
            MMap<string,long>::Iterator iter = mmap.begin();
            while (iter != mmap.end()){
                if ((*iter).key >= val){
                    recs.push_back((*iter).value_list);
                }
                iter++;
            }
        }

        if (_rel == "<="){
            MMap<string,long>::Iterator iter = mmap.begin();
            while (iter != mmap.end()){
                if ((*iter).key <= val){
                    recs.push_back((*iter).value_list);
                }
                iter++;
            }
        }

        for (int i = 0; i < recs.size(); i++){
            vector<long> temp = recs[i];
            for (int j = 0; j < temp.size(); j++){
                result.push_back(temp[j]);
            }
        }
        return result;        
    }
    private:
        string _rel;
        vector<long> result;

};

#endif