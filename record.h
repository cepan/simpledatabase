#ifndef RECORD_H
#define RECORD_H
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<vector>
#include<record_func.h>

using namespace std;
const int MAX_WIDTH = 50;
const int FIELD_AMOUNT= 10;

class Record{
public:
    Record();
    Record(char str[][MAX_WIDTH]);
    Record(vector<std::string> v );

    long write(fstream& outs);
    long read(fstream & ins, long recno);
    long get_recno(){return _rec_no;}
    long record_size(){return MAX_WIDTH * FIELD_AMOUNT;}




    friend ostream& operator <<(ostream& outs,
                          const Record& r){
        for (int i=0;FIELD_AMOUNT; i++){
                outs<<setw(15)<<r._record[i];
        }
        return outs;
    }
    string get_field_i(int i)const {return _record[i];}
    string operator[](int i )const{return _record[i];}


private:
    char _record[FIELD_AMOUNT][MAX_WIDTH];
    int _rec_no;
};

#endif // RECORD_H
