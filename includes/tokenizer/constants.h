#ifndef CONSTANTS_H
#define CONSTANTS_H
const int MAX_COLUMNS = 256;
const int MAX_ROWS = 100;
const int MAX_BUFFER = 200;

const char ALFA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char DIGITS[] = "0123456789";
const char OPERATORS[] = "><=!+-%&|*"; //added *
const char SPACES[] = {' ', '\t', '\n', '\0'};
const char PUNC[] = "?.,:;'`~!";
const int START_DOUBLE = 0;
const int START_SPACES = 4;
const int START_ALPHA = 6;
const int START_OPERATOR = 20;
const int START_PUNC = 10;

//token types:
const int TOKEN_NUMBER = 1;
const int TOKEN_ALPHA = 2;
const int TOKEN_SPACE = 3;
const int TOKEN_OPERATOR = 4;
const int TOKEN_PUNC = 5;

const int TOKEN_UNKNOWN = -1;
const int TOKEN_END = -2;


//added
const int MAKE = 1;
const int TABLE = 2;
const int INSERT = 3;
const int SELECT = 4;
const int SYMBOL = 5;
const int FIELDS = 6;
const int INTO = 7;
const int VALUES = 8;
const int STAR = 9;
const int FROM = 10;
const int WHERE = 11;
const int CONDITION = 12;
const int CREATE = 13;
const int TEMPSTORE = 14;

#endif