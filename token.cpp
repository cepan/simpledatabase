#include "token.h"

token::token(){

}
token::token(string str, int type):_token(str),_type(type){

}
token::token (char c, int type){
    _token.push_back(c);
    _type=type;

}
string token::type_string(){//NUMBER=1 ALPHA=2 SPACE=3 PUNC=4

    switch(_type){

    case NUMBER:
        return "NUMBER";
        break;
    case ALPHA:
        return "ALPHA";
        break;
    case SPACE:
        return "SPACE";
        break;
    case PUNC:
        return "PUNCU";
        break;
    case UNKNOWN:
    default:
        return "UNKNOWN";
        break;
    }


}
