#ifndef PARSER_H
#define PARSER_H
#include<iostream>
#include<cstring>
#include<ftk.h>
#include<map.h>
#include<mmap.h>

class Parser
{
public:
    Parser();
    Parser(char s[] );



    void set_string(char s[]);

    MMap<string,string> parse_tree();


    bool is_valid(){return _valid;}




    private:
    vector <string> _v;
    int _table[MAX_ROW][MAX_COL];
    Map<string, KEY>_key_map;
    stk s1;
    bool _valid;
    int word_to_type(string s);

    void make_table();
    void make_map();
};

#endif // PARSER_H
