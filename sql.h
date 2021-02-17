#ifndef SQL_H
#define SQL_H
#include<const.h>
#include<table.h>
#include"parser.h"



using namespace std ;



class Sql
{
public:
    Sql();
    void run(string filename );
    bool set_string(string command);


private:
    Map<string,table>_table;
    MMap<string, string>_ptree;

    Parser parser;

};

#endif // SQL_H
