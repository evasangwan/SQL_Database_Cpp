#ifndef TTOKEN_H
#define TTOKEN_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include "constant/constant.h"
using namespace std;

class TToken{
    public:
        TToken(){
            
        }
        TToken(const string& s): tok(s){

        }
        virtual int getType(){
            return TOKEN;
        }
        virtual double getPrecedence(){
            return -1;
        }
        virtual ostream& print(ostream& outs) const{
            outs << "[" << tok << "]";
            return outs; 
        }
        friend ostream& operator<<(ostream& outs, const TToken& token){
            return token.print(outs);
        }

    private:
        string tok;
};

#endif