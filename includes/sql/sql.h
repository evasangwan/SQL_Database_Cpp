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
    bool is_error(){   //returns if it's in error or not
        return sqlerror_flag; 
    }
    Table command(const string &input){
        try{
            sqlerror_flag = false;
            strcpy(s, input.c_str());
            Parser p(s);
            p.set_string(s);
            ptree = p.parse_tree(); 
        }
        catch(const invalid_argument e){
            sqlerror_flag = true;
            cerr << e.what() << endl; 
            return Table();
        }
        //if it's valid... 
        try{
        if (ptree["command"][0] == "select"){
            if (ptree["fields"][0] == "*"){ //select all
                if (ptree.contains("where")){   //select all + condition
                    vector<string> condition = ptree["condition"];
                    string name = ptree["table_name"][0];
                    if (tables.find(name) == tables.end()){ //if not found in tables Map
                        ifstream findtable(table_names);    //check in "tablenames.txt"
                        string str;
                        bool found = false;
                        if (findtable.is_open()){
                            while (getline(findtable,str)){
                                if (str == name){   //if found in "tablenames.txt"
                                    found = true;
                                    tables[name] = Table(name);  //opens the existing table and inserts that into the tables Map
                                    Table selected = tables[name].select_all_plus_condition(condition); //creates specific table
                                    recnos.clear();
                                    recnos = selected.select_recnos();  //gets the selected recnos
                                    findtable.close();
                                    return selected;
                                }
                            }
                        }
                        findtable.close();
                        if (!found){    //if it's not found, means it hasn't been created yet / it doesn't exist
                            recnos.clear();
                            return Table();
                        }
                    }
                    Table selected = tables[name].select_all_plus_condition(condition);
                    recnos.clear();
                    recnos = selected.select_recnos();
                    return selected;
                }
                else{   //select * no condition
                    string name = ptree["table_name"][0];
                    if (tables.find(name) == tables.end()){  //if not found in tables Map
                        ifstream findtable(table_names);  //check in "tablenames.txt"
                        string str;
                        bool found = false;
                        if (findtable.is_open()){
                            while (getline(findtable,str)){
                                if (str == name){  //if found in "tablenames.txt"
                                    found = true;
                                    tables[name] = Table(name);  //opens the existing table and inserts that into the tables Map
                                    recnos.clear();
                                    recnos = tables[name].select_recnos();  //gets the selected recnos
                                    findtable.close();
                                    return tables[name];
                                }
                            }
                        }
                        findtable.close();
                        if (!found){   //if it's not found, means it hasn't been created yet / it doesn't exist
                            recnos.clear();
                            return Table();
                        }
                    }
                    //else if it was found in the tables Map
                    recnos.clear();
                    recnos = tables[name].select_recnos();  //get the recnos and return that table
                    return tables[name];
                }
            }
            else{   //select [fields] 
                if (ptree.contains("where")){  //select [fields] + condition
                    vector<string> condition = ptree["condition"];
                    string name = ptree["table_name"][0];
                    if (tables.find(name) == tables.end()){  //if not found in tables Map
                        ifstream findtable(table_names);  //check in "tablenames.txt"
                        string str;
                        bool found = false;
                        if (findtable.is_open()){
                            while (getline(findtable,str)){
                                if (str == name){   //if found in "tablenames.txt"
                                    found = true;
                                    tables[name] = Table(name);   //opens the existing table and inserts that into the tables Map
                                    Table selected = tables[name].select(ptree["fields"], condition);  //creates specific table
                                    recnos.clear();
                                    recnos = selected.select_recnos();  //gets the selected recnos
                                    findtable.close();
                                    return selected;
                                }
                            }
                        }
                        findtable.close();
                        if (!found){   //if it's not found, means it hasn't been created yet / it doesn't exist
                            recnos.clear();
                            return Table();
                        }
                    }
                    //else if it was found in the tables Map
                    Table selected = tables[name].select(ptree["fields"], condition);
                    recnos.clear();
                    recnos = selected.select_recnos();
                    return selected;
                }
                else{   //select [fields] no condition
                    string name = ptree["table_name"][0];
                    if (tables.find(name) == tables.end()){  //if not found in tables Map
                        ifstream findtable(table_names);   //check in "tablenames.txt"
                        string str;
                        bool found = false;
                        if (findtable.is_open()){
                            while (getline(findtable,str)){
                                if (str == name){   //if found in "tablenames.txt"
                                    found = true;
                                    tables[name] = Table(name);  //opens the existing table and inserts that into the tables Map
                                    Table selected = tables[name].select_specific_fields_no_condition(ptree["fields"]); //creates specific table
                                    recnos.clear();
                                    recnos = selected.select_recnos(); //gets the selected recnos
                                    findtable.close();
                                    return selected;
                                }
                            }
                        }
                        findtable.close();
                        if (!found){    //if it's not found, means it hasn't been created yet / it doesn't exist
                            recnos.clear();
                            return Table();
                        }
                    }
                    //else if it was found in the tables Map
                    Table selected = tables[name].select_specific_fields_no_condition(ptree["fields"]);
                    recnos.clear();
                    recnos = selected.select_recnos();
                    return selected;
                }
            }
        }
        else{
            if (ptree["command"][0] == "make" || ptree["command"][0] =="create"){
                string name = ptree["table_name"][0];
                ofstream tablenames(table_names, ios::app); //open "tablenames.txt"
                if (tablenames.is_open()){
                    ifstream findtable(table_names);  //check in "tablenames.txt" if name exists
                    string str;
                    bool found = false;
                    if (findtable.is_open()){
                        while (getline(findtable,str)){
                             if (str == name){
                                found = true;
                                // tables[name] = Table(name, ptree["col"]);
                                tables[name] = Table(name);
                                cout << "Table [" << name << "] already exists: " << endl;
                                recnos.clear();
                                recnos = tables[name].select_recnos();
                                findtable.close();
                                return tables[name];
                            }
                        }
                    }
                    findtable.close();
                    if (!found){   //if it was not found in "tablenames.txt"
                        tablenames << name << endl;  //append name 
                    }
                }
                tablenames.close();
                //after appending it to "tablenames.txt"
                cout << ptree["col"] << endl; 
                tables[name] = Table(name, ptree["col"]);  //creates the table and inserts that into the tables Map
                recnos.clear();
                recnos = tables[name].select_recnos();  //gets the selected recnos
                return tables[name];
            }
            else if (ptree["command"][0] == "insert"){
                string name = ptree["table_name"][0];
                if (tables.find(name) == tables.end()){  //if not found in tables Map
                    ifstream findtable(table_names);  //check in "tablenames.txt"
                    string str;
                    bool found = false;
                    if (findtable.is_open()){
                        while (getline(findtable,str)){
                            if (str == name){  //if found in "tablenames.txt"
                                found = true;
                                tables[name] = Table(name);  //opens the existing table and inserts that into the tables Map
                                tables[name].insert_into(ptree["values"]);  //inserts values into that table
                                findtable.close();
                                return tables[name];
                            }
                        }
                    }
                    findtable.close();
                    if (!found){  //if it's not found, means it hasn't been created yet / it doesn't exist
                        recnos.clear();
                        return Table();
                    }
                }
                //else if it was found in tables Map
                tables[name].insert_into(ptree["values"]); //inserts that into the table
                return tables[name];
            }
        }
        }
        catch(const invalid_argument e){
            sqlerror_flag = true;
            cerr << e.what() << endl; 
            return Table();
        }
    }

    vector<long> select_recnos(){
        return recnos;
    }

    private:
    char s[300] = {};
    Map<string, Table> tables;
    mmap_ss ptree;
    vector<long>recnos;
    const string table_names = "tablenames.txt";
    bool sqlerror_flag; 
};


#endif
