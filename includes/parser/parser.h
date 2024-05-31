#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include "../tokenizer/ftokenize.h"
#include "../tokenizer/state_machine_functions.h"
#include "../bplustree/multimap.h"
#include "../queue/MyQueue.h"
#include "../table/typedefs.h"
#include <vector>
using namespace std;

class Parser{
    public:
    Parser(char s[300]){
        set_keywords();
        make_table(table);
        set_string(s);
    }
    MMap<string, string> parse_tree(){
        get_parse_tree();
        return ptree;
    }
    void set_keywords(){
        keywords.insert("make",MAKE);
        keywords.insert("create",CREATE);
        keywords.insert("insert",INSERT);
        keywords.insert("select",SELECT);
        keywords.insert("table",TABLE);
        keywords.insert("fields",FIELDS);
        keywords.insert("into",INTO);
        keywords.insert("values",VALUES);
        keywords.insert("*",STAR);
        keywords.insert("symbol",SYMBOL);
        keywords.insert("where",WHERE);
        keywords.insert("condition",CONDITION);
        keywords.insert("tempstore", TEMPSTORE);
        keywords.insert("from",FROM);
    }
    bool get_parse_tree(){
        bool table_name = false;
        bool values = false;
        bool coll = false;
        bool fields = false;
        bool condition = false;
        bool where = false;
        bool fields_check = false;
        bool values_check = false;
        bool starcheck = false;
        //cout << queue << endl;
        string token = queue.pop();
        int state = 0;
        int col;
        if (keywords.contains(token)){
            col = keywords[token];
        }
        else{
            col = keywords["symbol"];
        }
        state = table[state][col];
        while (state != -1){
            if (token == "make" || token == "insert" || token == "create"){
                ptree["command"] += token;
            }
            else if (token == "table" || token == "into"){
                table_name = true;
                //do nothing
            }
            else if (token == "fields"){
                //do nothing
                coll = true;
                fields_check = true;
                values_check = true;
                starcheck = true;
            }
            else if (token == "values"){
                values = true;
                fields_check = true;
                values_check = true;
                starcheck = true;
            }
            else if (token == "select"){
                 ptree["command"] += token;
                 fields = true; // *
            }
            else if (token == "from"){
                table_name = true;
                fields = false;
                fields_check = true;
                values_check = true;
                starcheck = true;
                //do nothing 
            }
            else if (token == "where"){
                string s = "yes";
                ptree["where"] += s;
                condition = true;
            }
            else{
                if (table_name){
                    ptree["table_name"] += token;
                    table_name = false;
                }
                else if (coll){
                     ptree["col"] += token;
                }
                else if (fields){
                    ptree["fields"] += token;
                }
                else if (condition){
                    ptree["condition"] += token;
                }
                else if (values){
                    ptree["values"] += token;
                }
            }
            if(!queue.empty()){
                token = queue.pop();
            }
            else{
                if (fields_check && values_check && starcheck && table[state][0] == 1){
                    return true;
                }
                else{
                    MMap<string, string> reset;
                    ptree = reset;
                    return false; 
                }
               
            }
            //cout << "token " << token << endl;
            if (keywords.contains(token)){
                col = keywords[token];
            }
            else{
                col = keywords["symbol"];
            }
            state = table[state][col];
        }
        MMap<string, string> reset;
        ptree = reset;
        return false;
    }
   
    void set_string(char s[300]){
        inputq.clear();
        Queue<string> clear;
        queue = clear;
        MMap<string, string> reset;
        ptree = reset;
        cout << ptree << endl;
        STokenizer stk(s);
        Token t;
        stk>>t;
        //cout << "IN SET STRING"<<endl;
        while(stk.more()){
            //cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;
            if (t.type_string() != "SPACE"){
                inputq.push_back(t.token_str());
            }
            t = Token();
            stk>>t;
        }
        fix_vect();
        for (int i = 0; i < inputq.size(); i++){
            queue.push(inputq[i]);
        }
        //cout << queue << endl;
        // cout << inputq << endl;
    }    
    void fix_vect(){
        vector<string> fixed;
        string combine;
        bool insidequotes = false;
        int count = 0;
        // cout << "Inputq"<<endl;
        // cout << inputq << endl;
        for (int i = 0; i < inputq.size(); i++){
            if (inputq[i] == "\""){
                insidequotes = true;
                count++;
                if (count == 2){
                    fixed.push_back(combine);
                    insidequotes = false;
                    count = 0;
                    combine.clear();
                }
            }
            else if (insidequotes && count < 2){
                combine += inputq[i];
            }
            else{
                if (inputq[i] != ","){
                    fixed.push_back(inputq[i]);
                }
            }
        }
        // cout << "fixed "<<endl;
        // cout << fixed << endl;
        inputq = fixed;
    }
    void make_table(int table[][MAX_COLUMNS]){
        init_table(table);
        // mark_fail(_table,0);
        for (int i = 0; i < 20; i++){
            mark_fail(table,i);
        }
        mark_success(table,5);

        //make table
        mark_success(table,5);
        mark_cell(0,table,MAKE,MAKE);
        mark_cell(1,table,TABLE,TABLE);
        mark_cell(2,table,SYMBOL,SYMBOL);
        mark_cell(5,table,FIELDS,FIELDS);
        mark_cell(6,table,SYMBOL,SYMBOL);
        mark_cell(5,table,SYMBOL,SYMBOL);

        //think this should work for create table
        mark_cell(0,table,CREATE,CREATE);
        mark_cell(CREATE,table,TABLE,TABLE);

        //insert into 
        mark_cell(0,table,INSERT,INSERT);
        mark_cell(INSERT,table,INTO,INTO);
        mark_cell(7,table,SYMBOL,SYMBOL);
        mark_cell(SYMBOL,table,VALUES,VALUES);
        mark_cell(VALUES,table,SYMBOL,SYMBOL);

        //select * from student
        mark_cell(0,table,SELECT,SELECT);
        mark_cell(SELECT,table,STAR,STAR);
        mark_cell(STAR,table,FROM,FROM);
        mark_cell(FROM,table,SYMBOL,SYMBOL);

        //select fieldnames from student where [condition]
        mark_cell(SELECT,table,SYMBOL,SYMBOL);
        mark_cell(SYMBOL,table,FROM,FROM);
        mark_cell(FROM,table,SYMBOL,SYMBOL);
        mark_cell(SYMBOL,table,WHERE,WHERE);
        mark_cell(WHERE,table,SYMBOL,SYMBOL);
    }

    private:
    vector<string> inputq;
    Queue<string> queue;
    Map<string,int> keywords;
    MMap<string, string> ptree;
    static int table[MAX_ROWS][MAX_COLUMNS];
};

#endif