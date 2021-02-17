#ifndef CONST_H
#define CONST_H

#include <cstring>

//state machine
using namespace std;
const bool DEBUG = false;
const int MAX_COL=128;
const int MAX_ROW=30;
const int NUMBER_MACHINE= 0;//start state in table
const int WORD_MACHINE= 5;//start state in table
const int PUNC_MACHINE=10;//start state in table
const int SPACE_MACHINE=15;//start state in table
const int UNKNOWN_MACHINE=20;
const int QUOTE_MACHINE =25;
const char ALPHABET[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
const char PUNCUATION [] = "!@#$%^&*()_+={};:',.<>/?";
const char DIGITS[]="0123456789";


const int MAX_BUF=500;

enum FUNCTION{FIELD,VALUE, REALATIONAL_OPERATOR, LOGICAL_OPERATOR  };


const int PARSER_TABLE_SIZE=60;
enum {NUMBER,ALPHA,SPACE,PUNC,QUOTE,UNKNOWN};


enum KEY{ZEROS, SYMBOL, COMMA,

    //Make
         MAKE,
         MAKE_TABLE,
         MAKE_TABLE_NAME,
         MAKE_FIELD,
         MAKE_FIELD_SYM,
         MAKE_COMMA,

    //INNSERT
         INSERT,
         INSERT_INTO,
         INSERT_TABLE_NAME,
         INSERT_VALUES,
         INSERT_VALUE,
         INSERT_COMMA,

     //SELECT
         SELECT,
         SELECT_STAR,
         SELECT_FROM,
         SELECT_TABLE_NAME,
         SELECT_WHERE,
         SELECT_FIELD_NAME,
         RELATIONAL,
         SELECT_VALUE,
         LOGICAL,
         SELECT_COMMA,
         SELECT_WHERE_FIELD_NAME





        };






#endif // CONST_H
