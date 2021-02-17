#ifndef RECORD_H
#define RECORD_H
#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<vector>
#include"record_func.h"

using namespace std;
const int MAX_WIDTH = 50;
const int FIELD_AMOUNT= 10;

class Record{
public:
    Record();
    Record(char str[][MAX_WIDTH]);
    Record(std::vector<std::string> v);

    long write(std::fstream& outs);
    long read(std::fstream& ins, long recno);

    friend std::ostream& operator<<(std::ostream& outs, const Record& r){
        for(int i=0; i<FIELD_AMOUNT; i++){
            outs<<std::setw(10)<<r._record[i];
        }
        return outs;
    }

    long record_size() {return FIELD_AMOUNT*MAX_WIDTH;}
    long get_recno() const {return _rec_no;}
    string get_field_i(int i) const {return _record[i];}
    string operator[](int i) const {return _record[i];}
private:
    char _record[FIELD_AMOUNT][MAX_WIDTH];
    int _rec_no;
};

#endif // RECORD_H
