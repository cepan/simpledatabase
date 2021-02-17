#ifndef TOKEN_H
#define TOKEN_H
#include<const.h>
#include<iostream>

using namespace std;

class token{

public:

    token();
    token(string str, int type);
    token (char c, int type);



    friend ostream& operator <<(ostream& outs,  token& t){
        outs<<"|||"<<t.token_str()<<"|||"<<endl;
        return outs;
    }

    int type(){return _type;}

    string type_string();
    string token_str(){return _token;}



private:
    string _token;
    int _type;








};

#endif // TOKEN_H
