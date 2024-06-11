#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include <ctype.h>
#include <algorithm>
#include <cstdlib> 
#include "../tokenizer/ftokenize.h"
#include "../tokenizer/state_machine_functions.h"
#include "../bplustree/multimap.h"
#include "../queue/MyQueue.h"
#include "../table/typedefs.h"
#include <vector>
#include <stdexcept>
using namespace std;

class Parser{
    public:
    Parser(char s[300]){
        set_keywords();     //creates the keywords map
        make_table(table);  //initializes the table
        set_string(s);      //puts input into queue
    }

    MMap<string, string> parse_tree(){
        get_parse_tree();   //parses input
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
        string token;
        if (!queue.empty()){
           token = queue.pop();
        }
        else{
            ptree.clear();
            throw false;
        }
        int state = 0;
        int col;
        string s = "yes";
        string keep = token;
        //converting popped token to lowercase
        for (int i = 0; i < token.size(); i++){
            token[i] = tolower(token[i]);
        }
        //seeing the col num for the specific token
        if (keywords.contains(token)){
            col = keywords[token];
        } 
        else{
            col = keywords["symbol"];
            token = keep;
        }
        //getting the state
        state = table[state][col];

        while (state != -1){
            switch (col){
                case MAKE:
                case CREATE:
                case INSERT:
                    ptree["command"] += token;
                    break;

                case TABLE:
                case INTO:
                    table_name = true;
                    break;

                case FIELDS:
                    coll = true;
                    break;

                case VALUES:
                    values = true;
                    break;

                case SELECT:
                    ptree["command"] += token;
                    fields = true; 
                    break;

                case FROM:
                    table_name = true;
                    fields = false;
                    break;

                case WHERE:
                    ptree["where"] += s;
                    condition = true;
                    break;

                default:
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
                    else{
                        ptree["garbage"] += token;  //for excess input (error handling)
                    }
                    break;
            }
            //if queue isn't empty, pop the next token
            if (!queue.empty()){
                token = queue.pop();
            } 
            else{  //when/if it's empty, validate if the ptree commands are valid
                if (validate()){
                    return true;
                } 
                else{
                    ptree.clear();
                    throw false;  //if not valid throw exception
                    // throw runtime_error("invalid");
                }
            }
            keep = token;
            //converting popped token to lowercase
            for (int i = 0; i < token.size(); i++){
                token[i] = tolower(token[i]);
            }
            //seeing the col num for the specific token
            if (keywords.contains(token)){
                col = keywords[token];
            } 
            else{
                col = keywords["symbol"];
                token = keep;
            }
            //getting the next state
            state = table[state][col];
        }
        //if it leaves while loop (fail state)...
        ptree.clear();
        // return false;
        throw false;
        // throw runtime_error("invalid");
    }

    void set_string(char s[300]){
        //clearing queue and ptree
        inputq.clear();
        ptree.clear();
        Queue<string> clear;
        queue = clear;
        
        //for " " (quotes)
        bool insidequotes = false;
        string inside;
        int count = 0;

        STokenizer stk(s);
        Token t;
        stk>>t;

        while(stk.more()){
            //cout<<setw(10)<<t.type_string()<<setw(10)<<t<<endl;
            if (t.type_string() == "QUOTES"){
                insidequotes = true;
                count++;
                if (count == 2){
                    inputq.push_back(inside); //pushing the string between both quotes as one
                    insidequotes = false;
                    count = 0;
                    inside.clear();
                }
            }
            else if (insidequotes && count < 2){
                inside += t.token_str();    
            }
            else if (t.type_string() != "SPACE"){  //else push everything that isn't a space 
                inputq.push_back(t.token_str());
            }
            t = Token();
            stk>>t;
        }
        fix_vect(); //gets rid of commas
        //pushing everything from that vector into the queue
        for (int i = 0; i < inputq.size(); i++){
            queue.push(inputq[i]);    
        }
    }  

    void fix_vect(){ //gets rid of commas 
        vector<string> fixed;
        for (int i = 0; i < inputq.size(); i++){
            if (inputq[i] != ","){
                fixed.push_back(inputq[i]);
            }
        }
        inputq = fixed;
    }

    bool validate(){  //error handling 
        if (ptree.contains("command") && ptree["command"][0] == "select"){
            if (ptree.contains("table_name") && ptree.contains("fields") && !ptree["table_name"][0].empty() && (ptree["fields"][0] == "*" || !ptree["fields"].empty())){
                if (ptree.contains("where")){
                    if (ptree.contains("condition") && !ptree["condition"].empty() && ptree["garbage"].empty()){
                        return true; 
                    }
                }
                else{
                    if (ptree["garbage"].empty()){
                        return true; 
                    }
                }
            }
        }
        else if (ptree.contains("command") && ptree["command"][0] == "make" || ptree["command"][0] == "create"){
            if (ptree.contains("table_name") && ptree.contains("col") && !ptree["table_name"][0].empty() && !ptree["col"].empty() && ptree["garbage"].empty()){
                return true;
            }
        }
        else if (ptree.contains("command") && ptree["command"][0] == "insert"){
            if (ptree.contains("table_name") && ptree.contains("values") && !ptree["table_name"][0].empty() && !ptree["values"].empty() && ptree["garbage"].empty()){
                return true;
            }
        }
        return false;
    } 

    void make_table(int table[][MAX_COLUMNS]){
        init_table(table);
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

        //create table
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