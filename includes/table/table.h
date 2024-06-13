#ifndef TABLE_H
#define TABLE_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include <vector>
#include <fstream>
#include "../queue/MyQueue.h"
#include "typedefs.h"
#include "../Token/ttoken.h"
#include "../Token/logical.h"
#include "../Token/lparen.h"
#include "../Token/operator.h"
#include "../Token/relational.h"
#include "../Token/rparen.h"
#include "../Token/tokenstr.h"
#include "../binary_files/filerecord.h"
#include "../Token/rpn.h"
#include "../Token/shuntingyard.h"
using namespace std;

class Table{
    public:
        Table(){  //default empty table
            num = 0;
            string s = "temp";
            strcpy(file_name,s.c_str());
            fstream f;
            open_fileW(f, file_name);
            f.close();
            selectrecnos.clear();
            set_fields.clear();     
            mmap.clear();                  
            field_map.clear(); 
        }

        //creating a new table 
        Table(const string& name, vector<string>fieldnames){
            num = 0;
            selectrecnos.clear();
            set_fields.clear();
            set_fields = fieldnames;
            strcpy(file_name,name.c_str());
            ofstream txtfile(name + ".txt");  //creating txt file with same name
            //inserting the fieldnames into it
            if (txtfile.is_open()){
                for (int i = 0; i < fieldnames.size(); i++){
                    if (i == fieldnames.size()-1){
                        txtfile << fieldnames[i];
                    }
                    else{
                        txtfile << fieldnames[i] << endl;
                    }
                }
            }
            txtfile.close();
            fstream f;
            open_fileW(f, file_name);  //open file for writing
            for (int j = 0; j < fieldnames.size(); j++){
                field_map.insert(fieldnames[j],j);   //inserting fieldnames & index into field_map Map
                MMap<string,long> m;
                mmap.push_back(m);  //creating an empty mmap for each field 
            }
            f.close();
        }

        Table(const string& s){ //opens an existing table
            selectrecnos.clear();
            num = 0;
            string name = s + ".txt";  
            ifstream getdata(name);   //opens txt file to get its fields 
            string str;
            //pushes each field into set_fields vector 
            if (getdata.is_open()){
                while (getline(getdata,str)){
                    if (!str.empty()){
                        set_fields.push_back(str);
                    }
                }
            }
            getdata.close();
            strcpy(file_name,s.c_str());
            fstream f;
            open_fileRW(f, file_name);  //open file for read/writing
            for (int j = 0; j < set_fields.size(); j++){
                field_map.insert(set_fields[j],j);  //inserting fieldnames & index into field_map Map
                MMap<string,long> m;
                mmap.push_back(m);   //creating an empty mmap for each field
            }
            FileRecord r;
            long recno = 0;
            r.read(f, recno);   //reading from file 
            while (!f.eof()){
                vector<string> data = r.data;  //data is a vector of the values (input)
                for (int i = 0; i < data.size(); i++){
                    mmap[i].insert(data[i], recno);  //inserting each component of the vector data at its respective index
                }
                selectrecnos.push_back(recno); //pushing back all the valid recnos
                recno++;
                num++;   //num keeps track of how many records there are
                r.read(f, recno);
            }
            f.close();
        }

        //v is the vector of the values (data)
        void insert_into(vector<string> v){
            fstream f;
            open_fileRW(f, file_name); //opening file for read/writing
            FileRecord r(v);
            long recno = r.write(f);  //writing to the file + getting the recno 
            for (int i = 0; i < v.size(); i++){
                mmap[i].insert(v[i], recno);   //inserting each component of the vector v at its respective index
            }
            selectrecnos.push_back(recno);  //pushing back the valid recno
            num++;  //num keeps track of how many records there are
            f.close();
        }

        Table select_all(){
            set_fields.clear();
            string open = file_name;
            open += ".txt";
            ifstream getdata(open);  //opens txt file to get its fields 
            string str;
            //pushes each field into set_fields vector 
            if (getdata.is_open()){
                while (getline(getdata,str)){
                    if (!str.empty()){
                        set_fields.push_back(str);
                    }
                }
            }
            getdata.close();
            string name = file_name;
            name += "_" + to_string(serial++);
            Table selected(name, set_fields);  //creating new "selected" table
            fstream f;
            open_fileRW(f, file_name); //opening file for read/writing
            FileRecord r;
            long recno = 0;
            vector<long> recnos;
            r.read(f, recno);  //starting @ 0 and reading from file
            while (!f.eof()) {
                vector<string> data = r.data;  //data is a vector of the data (values)
                selected.insert_into(data);  //inserting vector of values into selected table
                recnos.push_back(recno);  //keeping track of the recno
                recno++;
                r.read(f, recno);
            }
            f.close();
            selectrecnos.clear();
            selected.selectrecnos = recnos;
            return selected;
        }

        Table select_all_plus_condition(vector<string>condition){
            string name = file_name;
            name += ".txt";
            ifstream getdata(name);  //opens txt file to get its fields 
            string str;
            set_fields.clear();
            //pushes each field into set_fields vector 
            if (getdata.is_open()){
                while (getline(getdata,str)){
                    if (!str.empty()){
                        set_fields.push_back(str);
                    }
                }
            }
            Table selected = select(set_fields, condition); //creates new table "selected" and passes fields + conditions to get new table
            return selected;
        }

        Table select_specific_fields_no_condition(vector<string>&fields){
            string name = file_name;
            name += "_" + to_string(serial++);
            Table selected(name, fields);  //creating new "selected" table
            fstream f;
            open_fileRW(f, file_name);  //opening file for read/writing
            FileRecord r;
            long recno = 0;
            vector<long> recnos;
            r.read(f, recno);  //starting at 0 and reading from the file
            while (!f.eof()){
                //this is where we filter
                //"filtered" will be the vector we will insert into selected
                vector<string> filtered;  
                for (int k = 0; k < fields.size(); k++){
                    long index = field_map[fields[k]];  //get the respective index (order of specific fields)
                    filtered.push_back(r.data[index]);  //push into "filtered" the data at that specific index
                }
                selected.insert_into(filtered);
                recnos.push_back(recno);
                recno++;
                r.read(f, recno);
            }
            f.close();
            selected.selectrecnos.clear();
            selected.selectrecnos = recnos;
            return selected;
        }
        
        Table select(vector<string>fields, const string& field, const string& op, const string& value){
            set_fields = fields; 
            string name = file_name;
            name += "_" + to_string(serial++);
            Table selected(name, fields);
            Queue<TToken*> post;
            post.push(new TokenStr(field));
            post.push(new TokenStr(value));
            post.push(new Relational(op));
            RPN rpn(post);
            vector<long> recs = rpn.condition(field_map, mmap);
            fstream f;
            open_fileRW(f, file_name);
            FileRecord r;
            for (int i = 0; i < recs.size(); i++){
                r.read(f, recs[i]);
                vector<string> filtered; 
                for (int k = 0; k < fields.size(); k++){
                    long index = field_map[fields[k]];
                    filtered.push_back(r.data[index]);
                }
                selected.insert_into(filtered);
            }
            selected.num = recs.size();
            selectrecnos.clear();
            selectrecnos = recs;
            f.close();
            return selected; 
        }

        Table select(vector<string>fields,vector<string> conditions){
            set_fields.clear();
            set_fields = fields;
            string name = file_name;
            name += "_" + to_string(serial++);
            Table selected(name,set_fields);  //create a new table "selected"
            ShuntingYard sy(conditions);
            Queue<TToken*> post = sy.postfix();
            RPN rpn(post);
            vector<long> recs = rpn.condition(field_map, mmap); //recs are a vector of all the selected recnos 
            fstream f;
            open_fileRW(f, file_name);  //open file for read/writing
            FileRecord r;
            for (int i = 0; i < recs.size(); i++){
                r.read(f, recs[i]);
                //"filtered" will be the vector we will insert into selected
                vector<string> filtered; 
                for (int k = 0; k < fields.size(); k++){
                    long index = field_map[fields[k]];  //get the respective index (order of specific fields)
                    filtered.push_back(r.data[index]);  //push into "filtered" the data at that specific index
                }
                selected.insert_into(filtered);
            }
            selected.selectrecnos.clear();
            selected.selectrecnos = recs;
            recs.clear();
            f.close();
            return selected;
        }

        Table select(vector<string> fields, Queue<TToken*> post){
            set_fields.clear();  
            set_fields = fields;
            string name = file_name;
            name += "_" + to_string(serial++);
            Table selected(name, fields);  //create new table with those fields 
            RPN rpn(post);
            vector<long> recs = rpn.condition(field_map, mmap);  //after running it thru rpn, we get a vector of recnos
            fstream f;
            open_fileRW(f, file_name);  //opening file to read/write 
            FileRecord r;
            for (int i = 0; i < recs.size(); i++){
                r.read(f, recs[i]);
                //"filtered" will be the vector we will insert into selected
                vector<string> filtered; 
                for (int k = 0; k < fields.size(); k++){
                    long index = field_map[fields[k]];  //get the respective index (order of specific fields)
                    filtered.push_back(r.data[index]);  //push into "filtered" the data at that specific index
                }
                selected.insert_into(filtered);
            }
            selected.num = recs.size();
            selected.selectrecnos.clear();
            selected.selectrecnos = recs;
            f.close();
            return selected;
        }
        
        vector<long> select_recnos(){
            return selectrecnos;
        }

        int getnum(){
            return num;
        }

        friend ostream& operator<<(ostream& outs, const Table& t){
            fstream j;
            FileRecord r;
            open_fileRW(j, t.file_name);
            outs << "Table name: " << t.file_name << ", records: " << t.num << endl;
            outs << setw(26) << "record";
            for (int j = 0; j < t.set_fields.size(); j++){
                outs << setw(25) << t.set_fields[j];
            }
            cout << endl << endl;
            for (int i = 0; i < t.num; i++){
                r.read(j, i);
                outs << setw(24) << i << " " << r << endl;
            }
            j.close();
            return outs;
        }
        
    private:
    vector<MMap<string,long>> mmap;
    Map<string,long> field_map;
    bool empty;
    int num;
    char file_name[100];
    vector<string> set_fields;
    vector<long> selectrecnos;
    static int serial;
};
int Table::serial = 1;
#endif
