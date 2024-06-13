#ifndef TOKENSTR_H
#define TOKENSTR_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include "ttoken.h"
using namespace std;

class TokenStr: public TToken{
    public:
        TokenStr(const string& s): t(s){

        }
        virtual int getType() override{
            return TOKENSTR;
        }
        virtual string getStr(){
            return t;
        }
        virtual ostream& print(ostream& outs) const override{
            outs << "[" << t << "]";
            return outs; 
        }
        virtual double getPrecedence() override{
            return -99;
        }
        
    private:
        string t;
};

#endif