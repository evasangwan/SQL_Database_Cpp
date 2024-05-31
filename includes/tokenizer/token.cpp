#include "token.h"

Token::Token(){
    _token = "";
    _type = TOKEN_UNKNOWN;
}
Token::Token(string str, int type){
    _token = str;
    _type = type;
}
int Token::type()const {
    return _type;
}
string Token::type_string() const{
    if (_type == TOKEN_NUMBER){ // 1
        return "NUMBER";
    }
    if (_type == TOKEN_ALPHA){ //2
        return "ALPHA";
    }
    if (_type == TOKEN_SPACE){ //3
        return "SPACE";
    }
    if (_type == TOKEN_OPERATOR){ //4
        return "OPERATOR";
    }
    if (_type == TOKEN_PUNC){ //5
        return "PUNC";
    }
    if (_type == TOKEN_UNKNOWN || _type == 8 || _type == 47 || _type == 51){ //-1 or 8(#) or " " " 
        return "UNKNOWN";
    }
    else{
        return "ERROR";
    }
}
string Token::token_str() const{
    return _token;
}

ostream& operator <<(ostream& outs, const Token& t){
    outs << "|" << t.token_str() << "|";
    return outs;
}
