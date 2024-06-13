#ifndef ERROR_H
#define ERROR_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include <stdexcept>
using namespace std;

class invalidcommand: public exception{
private:
    string message;

public:
    explicit invalidcommand(const string& msg): message(msg){
        //nothing here
    }

    //overwriting what() func
    const char* what() const noexcept override{
        return message.c_str();
    }
};


#endif