#ifndef TABLE_H
#define TABLE_H
#include<iostream>
#include<fstream>
#include<vector>
#include<cstring>
#include"const.h"
#include"map.h"
#include"mmap.h"
#include"record.h"
#include<iostream>
#include"queue"
#include"stack"
using namespace std;


class table
{
public:
    table();
    table(string name );
    table(const string& name, const vector<string >& field_list );

    void insert_into(vector<string> field);                                //ok
    void insert_r_into(Record r );                                         //ok


    table select_all();
    table select(vector<string> field);
    table select_with_cond(vector<string> field, vector<string> cond);

    void reindex(const Record & r);


    vector<unsigned long> fields_to_index(vector<string> field_list);

    bool is_field(string s );

    vector<int> field_to_relational(string field, string value, string relational);

    vector<int> range(MMap<string, int >::Iterator from, MMap<string, int >::Iterator to);
    vector<int > add_vec (vector<int> a, vector <int> b);
    const vector<string> get_field_list (){return _field_list;}

    int priority(string s );
    int condition_token_type(string s );
    queue<string> shunting(vector <string>condition);

    vector<int> eval(queue<string> rpn);
    vector<int> intersection(vector<int> a , vector<int> b );
    vector<int> union_set(vector<int> a , vector<int> b );

//    void print_indices(int i){cout<<_indices_map[i];}

private:
    string _name;
    vector<string> _field_list;
    Map<string, MMap<string, int> > _indices_map;

};

#endif // TABLE_H
