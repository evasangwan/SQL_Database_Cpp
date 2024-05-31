#ifndef RESULTSET_H
#define RESULTSET_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include <vector>
#include "ttoken.h"
using namespace std;

class ResultSet: public TToken{
    public:
    ResultSet(const vector<long> r):resultset(r){
        
    }
    ResultSet(){
        
    }
    vector<long> getVector(){
        return resultset;
    }
    virtual int getType()override{
        return RESULTSET;
    }
    private: 
        vector<long> resultset;
};

#endif