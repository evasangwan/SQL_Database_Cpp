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
            //cout << "yeeeeeeeeeeeeeeeeeeeeeeeeeee"<<endl;
            recnos.clear();
            return Table();
        }
        // ptree.print_lookup();
        // cout << endl;
       
        if (ptree["command"][0] == "select"){
            if (ptree["fields"][0] == "*"){
                if (ptree.contains("where")){
                    vector<string> condition = ptree["condition"];
                    string name = ptree["table_name"][0];
                    //cout << "-------------------------"<<endl;
                    //cout << "firing select_all_plus_condition(condition) "<<endl;
                    if (tables.find(name) == tables.end()){
                        ifstream findtable(table_names);
                        string str;
                        bool found = false;
                        if (findtable.is_open()){
                            while (getline(findtable,str)){
                                if (str == name){
                                    found = true;
                                    tables[name] = Table(name);
                                    Table selected = tables[name].select_all_plus_condition(condition);
                                    recnos.clear();
                                    recnos = selected.select_recnos();
                                    return selected;
                                }
                            }
                        }
                        findtable.close();
                        if (!found){
                            recnos.clear();
                            return Table();
                        }
                    }
                    Table selected = tables[name].select_all_plus_condition(condition);
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
                        string str;
                        bool found = false;
                        // cout << "filename is " << table_names << endl;
                        ifstream findtable(table_names);
                        // ifstream findtable(file_path);
                        // ifstream findtable("../../build/bin/filenames.txt");
                        // ifstream findtable(table_names);
                        // findtable.open(table_names,ios::in);
                        if (findtable.is_open()){
                            while (getline(findtable,str)){
                                //cout << "str [" << str << "] name [" << name << "]"<< endl; 
                                if (str == name){
                                    found = true;
                                    tables[name] = Table(name);
                                    recnos.clear();
                                    recnos = tables[name].select_recnos();
                                    return tables[name];
                                }
                            }
                        }
                        findtable.close();
                        if (!found){
                            recnos.clear();
                            return Table();
                        }
                    }
                    recnos.clear();
                    recnos = tables[name].select_recnos();
                    return tables[name];
                }
            }
            else {
                if (ptree.contains("where")){
                    vector<string> condition = ptree["condition"];
                    string name = ptree["table_name"][0];
                    //cout << "-------------------------"<<endl;
                    //cout << "firing select(ptree[fields], condition) "<<endl
                    if (tables.find(name) == tables.end()){
                        ifstream findtable(table_names);
                        string str;
                        bool found = false;
                        if (findtable.is_open()){
                            while (getline(findtable,str)){
                                if (str == name){
                                    found = true;
                                    tables[name] = Table(name);
                                    Table selected = tables[name].select(ptree["fields"], condition);
                                    recnos.clear();
                                    recnos = selected.select_recnos();
                                    return selected;
                                }
                            }
                        }
                        findtable.close();
                        if (!found){
                            recnos.clear();
                            return Table();
                        }
                    }
                    Table selected = tables[name].select(ptree["fields"], condition);
                    // cout << "printing selecteddddddddddddddddddddddddddddddd"<<endl;
                    // cout << selected << endl;
                    recnos.clear();
                    recnos = selected.select_recnos();
                    // cout << recnos << endl;
                    return selected;
                }
                else{
                    string name = ptree["table_name"][0];
                    //cout << "-------------------------"<<endl;
                    //cout << "firing select_specific_fields_no_condition(ptree[fields])"<<endl;
                    if (tables.find(name) == tables.end()){
                        ifstream findtable(table_names);
                        string str;
                        bool found = false;
                        if (findtable.is_open()){
                            while (getline(findtable,str)){
                                if (str == name){
                                    found = true;
                                    tables[name] = Table(name);
                                    Table selected = tables[name].select_specific_fields_no_condition(ptree["fields"]);
                                    recnos.clear();
                                    recnos = selected.select_recnos();
                                    return selected;
                                }
                            }
                        }
                        findtable.close();
                        if (!found){
                            recnos.clear();
                            return Table();
                        }
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
                ofstream tablenames(table_names, ios::app);
                if (tablenames.is_open()){
                    ifstream findtable(table_names);
                    string str;
                    bool found = false;
                    if (findtable.is_open()){
                        while (getline(findtable,str)){
                            if (str == name){
                                found = true;
                                tables[name] = Table(name, ptree["col"]);
                                recnos.clear();
                                recnos = tables[name].select_recnos();
                                return tables[name];;
                            }
                        }
                    }
                    findtable.close();
                    if (!found){
                        tablenames << name << endl;
                    }
                    
                }
                tablenames.close();
                tables[name] = Table(name, ptree["col"]);
                recnos.clear();
                recnos = tables[name].select_recnos();
                return tables[name];
            }
            else if (ptree["command"][0] == "insert"){
                string name = ptree["table_name"][0];
                if (tables.find(name) == tables.end()){
                    ifstream findtable(table_names);
                    string str;
                    bool found = false;
                    if (findtable.is_open()){
                        while (getline(findtable,str)){
                            if (str == name){
                                found = true;
                                tables[name] = Table(name);
                                tables[name].insert_into(ptree["values"]);
                                return tables[name];
                            }
                        }
                    }
                    findtable.close();
                    if (!found){
                        recnos.clear();
                        return Table();
                    }
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
    const string table_names = "tablenames.txt";
};


#endif


