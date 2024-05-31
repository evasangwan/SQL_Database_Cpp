#ifndef SQL_H
#define SQL_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include <vector>
#include "../table/table.h"
#include "../parser/parser.h"
using namespace std;

class SQL{
    public: 
    SQL(){

    }
    Table command(const string &input){
        strcpy(s, input.c_str());
        Parser p(s);
        p.set_string(s);
        ptree = p.parse_tree(); 
        if (ptree.empty()){
            cout << "yeeeeeeeeeeeeeeeeeeeeeeeeeee"<<endl;
            recnos.clear();
            return Table();
        }
        ptree.print_lookup();
        cout << endl;
       
        if (ptree["command"][0] == "select"){
            if (ptree["fields"][0] == "*"){
                if (ptree.contains("where")){
                    vector<string> condition = ptree["condition"];
                    string name = ptree["table_name"][0];
                    //cout << "-------------------------"<<endl;
                    //cout << "firing select_all_plus_condition(condition) "<<endl;
                    if (tables.find(name) == tables.end()){
                        //cout << "HERE"<<endl;
                        recnos.clear();
                        return Table();
                    }
                    Table selected = tables[name].select_all_plus_condition(condition);
                    //cout << selected << endl;
                    recnos.clear();
                    recnos = selected.select_recnos();
                    return selected;
                }
                else{
                    string name = ptree["table_name"][0];
                    //cout << "JUST CHECKING THE NAME should be employee" << name << endl; 
                    //cout << "-------------------------"<<endl;
                    //cout << "firing select_all() "<<endl;
                    if (tables.find(name) == tables.end()){
                        recnos.clear();
                        return Table();
                    }
                    Table selected = tables[name].select_all();
                    recnos.clear();
                    recnos = selected.select_recnos();
                    // cout << recnos << endl;
                    // cout << selected << endl;
                    return selected;
                }
            }
            else {
                if (ptree.contains("where")){
                    vector<string> condition = ptree["condition"];
                    string name = ptree["table_name"][0];
                    //cout << "-------------------------"<<endl;
                    //cout << "firing select(ptree[fields], condition) "<<endl
                    if (tables.find(name) == tables.end()){
                        recnos.clear();
                        return Table();
                    }
                    Table selected = tables[name].select(ptree["fields"], condition);
                    recnos.clear();
                    recnos = selected.select_recnos();
                    // cout << recnos << endl;
                    // cout << "should be 2 1 0 3" << endl;
                    return selected;
                }
                else{
                    string name = ptree["table_name"][0];
                    //cout << "-------------------------"<<endl;
                    //cout << "firing select_specific_fields_no_condition(ptree[fields])"<<endl;
                    if (tables.find(name) == tables.end()){
                        recnos.clear();
                        return Table();
                    }
                    Table selected = tables[name].select_specific_fields_no_condition(ptree["fields"]);
                    //cout << selected << endl;
                    recnos.clear();
                    recnos = selected.select_recnos();
                    return selected;
                }
            }
        }
        else{
            if (ptree["command"][0] == "make" || ptree["command"][0] =="create"){
                string name = ptree["table_name"][0];
                tables[name] = Table(name, ptree["col"]);
                //cout << tables[name] << endl;
                return tables[name];
            }
            else if (ptree["command"][0] == "insert"){
                string name = ptree["table_name"][0];
                // cout << "~~~~~~~~~inside insert~~~~~~~~~~~~~~~~~~"<<endl;
                // cout << ptree["values"] << endl;
                // vector<string> check = ptree["values"];
                // for (int i = 0; i < check.size(); i++){
                //     cout << check[i]<<endl;
                // }
                if (tables.find(name) == tables.end()){
                    recnos.clear();
                    return Table();
                }
                tables[name].insert_into(ptree["values"]);
                //cout << tables[name] << endl;
                return tables[name];
            }
        }
    }

    vector<long> select_recnos(){
        return recnos;
    }

    private:
    Table t;
    char s[300];
    Map<string, Table> tables;
    mmap_ss ptree;
    vector<long>recnos;
};


#endif
