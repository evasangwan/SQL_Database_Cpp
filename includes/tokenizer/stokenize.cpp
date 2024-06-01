#include "stokenize.h"

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer(){
    strcpy(_buffer, "");
    _pos = 0;
    make_table(_table);
    // cout << "---------TABLE!-------------" << endl;
    // print_table(_table);
}

STokenizer::STokenizer(char str[]){
    strcpy(_buffer, str);
    _pos = 0;
    //init_table(_table);
    make_table(_table);
    //cout << "---------TABLE!-------------" << endl;
    // print_table(_table);
}


bool STokenizer::get_token(int& start_state, string& token){
    return getToken(_buffer,_pos,start_state,_table,token);
}

void STokenizer::make_table(int _table[][MAX_COLUMNS]){
    init_table(_table);
    mark_fail(_table,0);
    for (int i = 1; i < 13;i++){
        mark_success(_table,i);
    }
    mark_fail(_table,6);
    mark_cells(0,_table,DIGITS,1);
    mark_cells(0,_table,ALFA,2);
    mark_cells(0,_table,SPACES,3);
    mark_cells(0,_table,OPERATORS,4);
    mark_cells(0,_table,PUNC,5);
    mark_cells(1,_table,DIGITS,1);
    mark_cells(1,_table,PUNC,6);
    mark_cells(2,_table,ALFA,2);
    mark_cells(3,_table,SPACES,3);
    mark_cells(4,_table,OPERATORS,4);
    mark_cells(5,_table,PUNC,5);
    mark_cells(6,_table,DIGITS,7);
    mark_cells(7,_table,DIGITS,7);
    mark_cell(0,_table,35,8);
    mark_cell(0,_table,47,8); //for the /(unknown)
    mark_cell(8,_table,35,8); //for the #(unknown)
    mark_cell(8,_table,47,8); 
    mark_cell(0,_table,34,9);  //quotation marks
    mark_cell(9,_table,34,9);  //40 is ( 41 is )
    mark_cell(0,_table,40,8);  
    // mark_cell(8,_table,40,8);
    mark_cell(8,_table,40,12);
    mark_cell(0,_table,41,8);  
    // mark_cell(8,_table,41,8);
      
    mark_cell(8,_table,41,12);
    //cout << "#" <<  _table[0][35] << endl;
}

bool STokenizer:: done(){  //true: there are no more tokens
    if (_pos > strlen(_buffer))
        return true;
    return false;
}  

bool STokenizer:: more(){ //true: there are more tokens
    //cout << "STR OF BUFFER " << strlen(_buffer);
    // if (_pos == strlen(_buffer)){
    //     _pos--;
    // }
    // if (_pos == strlen(_buffer)){
    //     return true;
    // }
    //cout << "POSITION!!!!!! " << _pos << endl;
    //cout << "positiom: " << _pos << endl;
    return _pos <= strlen(_buffer);
    //return _pos < strlen(_buffer);
}            

void STokenizer:: set_string(char str[]){
    strcpy(_buffer,str);
    _pos = 0;
}

STokenizer& operator >> (STokenizer& s, Token& t){
    string temp;
    int p = s._table[0][s._buffer[s._pos]];
    //cout << "P IS " << p << endl;
    // if (s._buffer[s._pos] < 0){
    //     p = TOKEN_UNKNOWN;
    // }
    int type = p;
    //cout << "p " << p << endl;
    // if (s._pos == strlen(s._buffer)){
    //     s._pos++;
    //     // t = Token(temp, type); 
    //     return s;
    // }
    //cout << "strlen(s._buffer): " << strlen(s._buffer) << endl;
    if (s._pos == strlen(s._buffer)){
        s._pos++;
    return s;
    }
    s.get_token(p, temp); 
    // cout << "temp " << temp << endl;
    //cout << "p after " << p << endl;
    //cout << "token: " << temp<<endl;
    t = Token(temp, type); 
    //cout << "t is " << t << endl;
    return s;
}
