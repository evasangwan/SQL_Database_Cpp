#include "ftokenize.h"

FTokenizer::FTokenizer(char* fname){
    _f.open(fname); 
    _pos = 0;      
    _blockPos = 0;  
    _more = true;   
    get_new_block();
}

Token FTokenizer::next_token(){
     Token t;
    if (_stk.done()){
        if (!get_new_block()){
            _more = false;
            _f.close();
            return Token();
        }
    }
   _stk >> t;
    return t;
}

bool FTokenizer:: more(){        //returns the current value of _more
    _more = _stk.more();
    if (!_more){
        _more = get_new_block();
    }  
   return _more;
}    

int FTokenizer:: pos(){          //returns the value of _pos
   return _pos;
}        
int FTokenizer::block_pos(){    //returns the value of _blockPos
    return _blockPos;
}    

FTokenizer& operator >> (FTokenizer& f, Token& t){
    t = f.next_token();
    return f;
}

bool FTokenizer::get_new_block(){
    if(_f.eof()){
        _f.close();
        return false;
    }
    char block[MAX_BLOCK-2];
    _f.read(block,sizeof(block) -1);
    _pos += _f.gcount();
    block[_f.gcount()] = '\0';
    _stk.set_string(block);
    if (more()){
        return true;
    }
    return false;
} 