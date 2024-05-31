#ifndef LOGICAL_H
#define LOGICAL_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include <vector>
#include "operator.h"
#include <algorithm>
using namespace std;

class Logical: public Operator{
    public: 
    Logical(const string& s): _log(s){
        
    }
    int getType(){
        return LOGICAL;
    }
    virtual string Op() const override{
        return _log;
    }
    virtual ostream& print(ostream& outs)const override{
            outs << "[" << _log << "]";
            return outs; 
    }
    virtual double getPrecedence() override{
            if (_log == "or"){
                return 1;
            }
            if (_log == "and"){
                return 2;
            }
            return -99;
    }
    vector<long> eval(vector<long>& _one, vector<long>& _two){
        if (_log == "and"){
            vector<long> res;
            vector<long> one;
            vector<long> two;
            if (_one.size() > _two.size()){
                one = _one;
                two = _two;
            }
            else{
                one = _two;
                two = _one;
            }
            for (int i = 0; i < one.size(); i++){
                int m = 0;
                for (int j = 0; j < two.size(); j++){
                    if (one[i] == two[j] && m < 1){
                        res.push_back(one[i]);
                        m++;
                    }
                    // cout << "RECS "<<endl;
                    // cout << res << endl;
                }
            }
            result = res;
        }

        if (_log == "or"){
            vector<long> res;
            vector<long> one;
            vector<long> two;
            if (_one.size() > _two.size()){
                one = _one;
                two = _two;
            }
            else{
                one = _two;
                two = _one;
            }
            // cout << "one " << endl;
            // cout << one << endl;
            // cout << "two " << endl;
            // cout << two << endl;
            for (int i = 0; i < two.size(); i++){
                vector<long>::iterator t = find(res.begin(),res.end(),two[i]);
                if (t == res.end()){
                    res.push_back(two[i]);
                }
            }
            for (int i = 0; i < one.size(); i++){
                vector<long>::iterator t = find(res.begin(),res.end(),one[i]);
                if (t == res.end()){
                    res.push_back(one[i]);
                }
            }
            result = res;
        }
        return result;
    }
    private:
        string _log;
        vector<long> result;

};
#endif