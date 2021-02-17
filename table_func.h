#ifndef TABLE_FUNC_H
#define TABLE_FUNC_H
#include<string>
#include<iostream>
#include<iomanip>
#include"const.h"
#include"table_func.cpp"
using namespace std;
void init_table(int _table[][MAX_COL]);
void mark_success(int _table[][MAX_COL], int state);
void mark_fail(int _table[][MAX_COL], int state);
bool is_success(int _table[][MAX_COL], int state);
void mark_cells(int row, int _table[][MAX_COL], int from, int to, int state);

void mark_cells(int row, int _table[][MAX_COL], const char columns[], int state);
void mark_cell(int row, int table[][MAX_COL], int column, int state);
void print_table(int _table[][MAX_COL]);
void show_string(char s[], int _pos);
bool _get_token(string input, int& pos, int _table[][MAX_COL], int start_state, string& t);
void print_part_table(int row,int _table[][MAX_COL],int from, int to);
void print_character(int row,int _table[][MAX_COL],int col);
void printprinttable(int _table[][MAX_COL]);

#endif // TABLE_FUNC_H
