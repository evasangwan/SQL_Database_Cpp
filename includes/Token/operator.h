#ifndef OPERATOR_H
#define OPERATOR_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include "ttoken.h"
using namespace std;

class Operator: public TToken{
    public:
        Operator(){
            
        }

        Operator(const string& t):_op(t){

        }
        virtual int getType() override{
            return -999;
        }
        virtual int typeofOp(){
            return -2;
        }
        virtual double getPrecedence() override{
            return 4;
        }
        virtual ostream& print(ostream& outs)const override{
            outs << "[" << _op << "]";
            return outs; 
        }

        virtual string Op() const{
            return _op;
        }
    private:
        string _op;

};
#endif