#ifndef RPN_H
#define RPN_H
#include "ttoken.h"
#include "operator.h"
#include "tokenstr.h"
#include "relational.h"
#include "logical.h"
#include "../stack/MyStack.h"
#include "../queue/MyQueue.h"
#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
#include "constant/constant.h"
#include "resultset.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

class RPN{
    public:
    RPN(const Queue<TToken *> postfix):_postfix(postfix){

    }
    
    vector<long> condition(Map<string,long> &field_map, vector<MMap<string,long>> &mmap){
        // cout << "*********************************************"<<endl;
        // cout << _postfix << endl;
        // cout << "*********************************************"<<endl;
        Stack<TToken*> stack;
        Queue<TToken*>::Iterator it = _postfix.begin();
        while (!it.is_null()){
            TToken* temp = *it;
            int type = temp->getType();
            if (type == TOKENSTR){
                stack.push(temp);
            }
            if (type == RELATIONAL){
                //cout << "STACK " << stack << endl;
                TToken* value = stack.pop();
                if (stack.empty()){
                    throw invalid_argument("Invalid condition: missing field/value");
                }
                TToken* fieldname = stack.pop();
                if (value->getType() == TOKENSTR && fieldname->getType() == TOKENSTR){
                    const string val = static_cast<TokenStr*>(value)->getStr();
                    const string field = static_cast<TokenStr*>(fieldname)->getStr();
                    // cout << "value " << val << endl;
                    // cout << "filed " << field << endl;
                    Relational* rel = static_cast<Relational*>(temp);
                    long index = field_map[field];
                    vector<long> recs = rel->eval(mmap[index],val);
                    //cout << "recs in rpn " << endl << recs << endl << "recsssssssss"<<endl;
                    stack.push(new ResultSet(recs));
                }
                else{
                    throw invalid_argument("Invalid condition: Missing logical");
                }
            }
            if (type == LOGICAL){
                TToken* second = stack.pop(); 
                if (stack.empty()){
                    throw invalid_argument("Expected complete condition");
                }
                TToken* first = stack.pop();
                if (second->getType() == RESULTSET && first->getType() == RESULTSET){
                    vector<long> one = static_cast<ResultSet*>(first)->getVector();
                    vector<long> two = static_cast<ResultSet*>(second)->getVector();
                    Logical* log = static_cast<Logical*>(temp);
                    vector<long> recs = log->eval(one,two);
                    // cout << "recs logical " << endl;
                    // cout << recs << endl;
                    stack.push(new ResultSet(recs));
                }
                else{
                    throw invalid_argument("Invalid4");
                }
            }
            it++;
        }
        TToken* res = stack.pop();
        if (!stack.empty()){
            throw invalid_argument("Invalid logical");
        }
        vector<long> result = static_cast<ResultSet*>(res)->getVector();
        return result;
    }

    private:
        Queue<TToken *> _postfix;
        
};


#endif
