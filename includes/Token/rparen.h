#ifndef RPAREN_H
#define RPAREN_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include "ttoken.h"
using namespace std;

class RParen: public TToken{
    public:
        RParen(const string& t):_rparen(")"){
            
        }
        RParen(){
            _rparen = ")";
        }
        virtual int getType()override{
            return RPAREN;
        }
        string getRParen() const{
            return _rparen;
        }
        virtual double getPrecedence() override{
            return 4;
        }
        private: 
            string _rparen;
};

#endif