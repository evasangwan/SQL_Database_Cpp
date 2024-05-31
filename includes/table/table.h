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

        Table(){
            num = 0;
            string s = "";
            strcpy(file_name,s.c_str());
        }

        Table(const string& name, vector<string>fieldnames){
            set_fields.clear();
            set_fields = fieldnames;
            num = 0;
            strcpy(file_name,name.c_str());
            ofstream txtfile(name + ".txt");
            if (txtfile.is_open()){
                for (int i = 0; i < fieldnames.size(); i++){
                    txtfile << fieldnames[i] << endl;
                }
            }
            txtfile.close();
            fstream f;
            open_fileW(f, file_name);
            //cout << fieldnames << endl;
            for (int j = 0; j < fieldnames.size(); j++){
                field_map.insert(fieldnames[j],j);
                MMap<string,long> m;
                mmap.push_back(m);
            }
            // cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
            // cout << field_map << endl;
            f.close();
        }
        Table(const string& s){ //opens an existing table
            num = 0;
            string name = s + ".txt";
            ifstream getdata(name);
            string str;
            if (getdata.is_open()){
                while (getline(getdata,str)){
                    if (!str.empty()){
                        set_fields.push_back(str);
                    }
                }
            }
            getdata.close();
            // for (int i = 0; i < set_fields.size(); i++){
            //     cout << "[" << set_fields[i] << "]"<<endl;
            // }
            strcpy(file_name,s.c_str());
            fstream f;
            open_fileRW(f, file_name);
            for (int j = 0; j < set_fields.size(); j++){
                field_map.insert(set_fields[j],j);
                MMap<string,long> m;
                mmap.push_back(m);
            }
            FileRecord r;
            long recno = 0;
            r.read(f, recno);    
            while (!f.eof()){
                vector<string> data = r.data;
                for (int i = 0; i < data.size(); i++){
                    mmap[i].insert(data[i], recno);
                }
                recno++;
                num++;
                r.read(f, recno);
            }
            f.close();
        }

        void insert_into(vector<string> v){
            // cout << "INSIDE INSER INTO "<<endl;
            // cout << "inserted: " << endl;
            // cout << v << endl;
            // cout << "Mmap we're starting with " << endl;
            // cout << mmap << endl;
            fstream f;
            open_fileRW(f, file_name);
            FileRecord r(v);
            long recno = r.write(f);
            for (int i = 0; i < v.size(); i++){
                mmap[i].insert(v[i], recno);
            }
            num++;
            // cout << "###################"<<endl;
            // cout << mmap << endl;
            f.close();
        }

        Table select_all(){
            //cout << "file_name " << file_name << endl;
            set_fields.clear();
            num = 0;
            string open = file_name;
            open += ".txt";
            ifstream getdata(open);
            string str;
            if (getdata.is_open()){
                while (getline(getdata,str)){
                    if (!str.empty()){
                        set_fields.push_back(str);
                    }
                }
            }
            getdata.close();
            // for (int i = 0; i < set_fields.size(); i++){
            //     cout << "[" << set_fields[i] << "]"<<endl;
            // }
            string name = file_name;
            name += "_" + to_string(serial++);
            Table selected(name, set_fields);
            fstream f;
            open_fileRW(f, file_name);
            FileRecord r;
            long recno = 0;
            vector<long> recnos;
            r.read(f, recno);    
            while (!f.eof()) {
                vector<string> data = r.data;
                selected.insert_into(data);
                recnos.push_back(recno);
                recno++;
                r.read(f, recno);
            }
            f.close();
            // cout << "HELLO"<<endl;
            // cout << recnos << endl;
            selected.num = recno;
            selectrecnos.clear();
            selected.selectrecnos = recnos;
            // cout << selectrecnos << endl;
            return selected;
        }
        //for now! 
        Table select_all_plus_condition(vector<string>condition){
            // num = 0;
            string name = file_name;
            // cout <<"name "<<name << endl;
            name += ".txt";
            ifstream getdata(name);
            string str;
            set_fields.clear();
            if (getdata.is_open()){
                while (getline(getdata,str)){
                    if (!str.empty()){
                        set_fields.push_back(str);
                    }
                }
            }
            // for (int i = 0; i < set_fields.size(); i++){
            //     cout << "[" << set_fields[i] << "]"<<endl;
            // }
            // cout << set_fields<<endl;
            Table selected = select(set_fields, condition);
            // cout <<"testing hereeeeeeeeeeee"<<endl;
            // cout << selected << endl;
            // cout << selected.select_recnos() << endl;
            return selected;
        }

        Table select_specific_fields_no_condition(vector<string>&fields){
            num = 0;
            string name = file_name;
            name += "_" + to_string(serial++);
            Table selected(name, fields);
            fstream f;
            open_fileRW(f, file_name);
            FileRecord r;
            long recno = 0;
            vector<long> recnos;
            r.read(f, recno);    
            while (!f.eof()) {
                vector<string> filtered; 
                for (int k = 0; k < fields.size(); k++){
                    long index = field_map[fields[k]];
                    filtered.push_back(r.data[index]);
                }
                selected.insert_into(filtered);
                recnos.push_back(recno);
                recno++;
                r.read(f, recno);
            }
            f.close();
            selected.num = recno;
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
            string s = file_name;
            Table selected(s);
            // cout << "printing selected "<<endl;
            // cout << selected << endl;
            ShuntingYard sy(conditions);
            Queue<TToken*> post = sy.postfix();
            // cout << "POST "<<endl;
            // cout << post << endl;
            RPN rpn(post);
            // cout << "field_nam" <<endl;
            // cout << field_map << endl;
            // cout << "mmap " << endl;
            // cout << mmap << endl;
            vector<long> recs = rpn.condition(field_map, mmap);
            //  cout << "no freaking way"<<endl;
            // cout << recs << endl;
            fstream f;
            open_fileRW(f, file_name);
            FileRecord r;
            for (int i = 0; i < recs.size(); i++){
                r.read(f, recs[i]);
                vector<string> filtered; 
                for (int k = 0; k < fields.size(); k++){
                    long index = selected.field_map[fields[k]];
                    filtered.push_back(r.data[index]);
                }
                selected.insert_into(filtered);
            }
            selected.num = recs.size();
            selected.selectrecnos.clear();
            // cout << "RECS"<<endl;
            // cout << recs << endl;
            selected.selectrecnos = recs;
            f.close();
            // cout << "testing to see if they r the same tbale " << endl;
            // cout << selected << endl;
            // cout << "-------------------------------------------------------------"<<endl;
            return selected;
        }

        Table select(vector<string> fields, Queue<TToken*> post){
            set_fields.clear();  //added this !!! in case all hell breaks loose
            set_fields = fields;
            string name = file_name;
            name += "_" + to_string(serial++);
            Table selected(name, fields);
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
