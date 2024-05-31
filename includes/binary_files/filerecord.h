#ifndef FILE_RECORD_H
#define FILE_RECORD_H
#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <cstring>
#include <cassert>
#include "utilities.h"
#include <vector>
using namespace std;

class FileRecord{
public:
    static const int MAX = 100;
    int recno;
    char _record[MAX+1][MAX+1];
    vector<string> data;
    //when you construct a Record, it's either empty or it
    //  contains a word
    FileRecord(){
        for (int i = 0; i < MAX; i++){
            _record[i][0] = '\0';
        }
        recno = -1;
    }
    FileRecord(vector<string> v){
        // data.clear();
        data = v;
        //cout << "v is " << v << endl;
        // strncpy(_record[0], v.c_str(), MAX);
        for (int i = 0; i < v.size(); i++){
            // string s = v[i];
            strncpy(_record[i], v[i].c_str(), MAX);
        }
        for (int i = v.size(); i < MAX; i++){
             _record[i][0] = '\0';
        }
    }

    long write(fstream& outs);              //returns the record number
    long read(fstream& ins, long recno);    //returns the number of bytes
                                            //      read = MAX, or zero if
                                            //      read passed the end of file


    friend ostream& operator<<(ostream& outs, const FileRecord& r);
private:
    
};

long FileRecord::write(fstream &outs){
    //r.write(f); //take the Record r and write it into file f
    //  and return the record number of this Record

    //write to the end of the file.
    outs.seekg(0, outs.end);


    long pos = outs.tellp();    //retrieves the current position of the
                                //      file pointer

    //outs.write(&record[0], sizeof(record));
    //outs.write(_record[0], sizeof(_record));
    for (int i = 0; i < MAX; i++){
        outs.write(_record[i], MAX);
    }

    // return pos/sizeof(_record);  //record number
    return pos/(MAX*MAX);
}

long FileRecord::read(fstream &ins, long recno){
    data.clear();
    //returns the number of bytes read.
    //    r.read(f, 6);
    //    cout<<r<<endl;
    //long pos= recno * sizeof(_record);
    long pos= recno * (MAX*MAX);
    ins.seekg(pos, ios_base::beg);

    //ins.read(&_record[0], sizeof(_record))
    // ins.read(_record[0], sizeof(_record));
    for (int i = 0; i < MAX; i++) {
        ins.read(_record[i], MAX);
        // data.push_back(string(_record[i]));
         if (ins.gcount() > 0 && _record[i][0] != '\0'){
            data.push_back(string(_record[i]));
        }
        _record[i][ins.gcount()] = '\0'; 
    }
    //don't you want to mark the end of  the cstring with null?
    //_record[] => [zero'\0'trash trash trash trash]
    //don't need the null character, but for those of us with OCD and PTSD:
    //_record[0][ins.gcount()]= '\0';
    return ins.gcount();

}
ostream& operator<<(ostream& outs, const FileRecord& r){
    // outs<<r._record;
    for (int i = 0; i < FileRecord::MAX; i++){
        if (r._record[i][0] != '\0'){
            outs << setw(25) << r._record[i] << " ";
        }
    }
    return outs;
}



#endif