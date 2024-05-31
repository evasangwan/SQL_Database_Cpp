#ifndef LPAREN_H
#define LPAREN_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include "ttoken.h"
using namespace std;

class LParen: public TToken{
    public:
    LParen(const string& t):lparen("("){
        
    }
    LParen(){
        lparen = "(";
    }
    virtual int getType()override{
        return LPAREN;
    }
    string getLParen() const{
        return lparen;
    }
    virtual double getPrecedence() override{
        return 4;
    }
    private: 
        string lparen;
};

#endif