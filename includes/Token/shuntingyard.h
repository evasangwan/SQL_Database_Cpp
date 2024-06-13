#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include <vector>
#include "ttoken.h"
#include "operator.h"
#include "tokenstr.h"
#include "relational.h"
#include "logical.h"
#include "lparen.h"
#include "rparen.h"
#include "resultset.h"
#include <algorithm>
#include "../stack/MyStack.h"
#include "../queue/MyQueue.h"
#include "constant/constant.h"
#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
using namespace std;

class ShuntingYard{
    public:
    ShuntingYard(vector<string> &input):_input(input){
        // cout << input << endl; 
       for (int i = 0; i < _input.size(); i++){
            if (_input[i] == "("){
                _postfix.push(new LParen());
            }
            else if (_input[i] == ")"){
                _postfix.push(new RParen());
            }
            else if (_input[i] == "=" || _input[i] == "<" || _input[i] == ">" || _input[i] == ">=" || _input[i] == "<="){
                _postfix.push(new Relational(_input[i]));
            }
            else if (_input[i] == "or" || _input[i] == "and"){
                _postfix.push(new Logical(_input[i]));
            }
            else{
                _postfix.push(new TokenStr(_input[i]));
            }
       }
    }

    ShuntingYard(const Queue<TToken*> postfix):_postfix(postfix){
        
    }

    ShuntingYard(){
        
    }

    Queue<TToken *> postfix(){
        Queue<TToken *> postfix;
        Stack<TToken*> stack;
        Queue<TToken*>::Iterator it = _postfix.begin();
        while (!it.is_null()){
            TToken* temp = *it;
            int type = temp->getType();
            if (type == TOKENSTR){
                postfix.push(temp);
            }
            else if (type == RELATIONAL || type == LOGICAL){
                while (!stack.empty() && stack.top()->getType() != LPAREN && stack.top()->getPrecedence() >= temp->getPrecedence()){
                    TToken* popped = stack.pop();
                    postfix.push(popped);
                }
                stack.push(temp);
            }
            else if (type == RPAREN){
                while (!stack.empty() && stack.top()->getType() != LPAREN){
                    TToken* pop = stack.pop();
                    postfix.push(pop);
                }
                stack.pop();  
            } 
            else{
                while (!stack.empty() && stack.top()->getPrecedence()!= LPAREN && stack.top()->getPrecedence() >= temp->getPrecedence()){
                    TToken* popped = stack.pop();
                    postfix.push(popped);
                }
                stack.push(temp);
            }
            it++;
        }
        while (!stack.empty()){
            TToken *popped = stack.pop();
            postfix.push(popped);
        }
        return postfix;
    }

private:
    Queue<TToken *> _postfix;
    vector<string> _input;
};




#endif