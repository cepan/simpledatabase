#ifndef STK_H
#define STK_H
#include"token.h"
#include"table_func.h"

using namespace std;

class stk{
public:
    stk(){make_table(_table);}
    stk(char str[]){
        make_table(_table);
        set_string(str);
    }
    bool done(){//there are no more token
        return _pos >strlen(_buffer);
    }
    bool failed(){//there are token
        return !done();
    }
    //extract one token (very similar to the way cin >> works)
    friend stk& operator >>(stk& s,token& t ){
        string str;

        if(s.get_token(NUMBER_MACHINE,str)){
           t= token(str,NUMBER);
        }
        else if(s.get_token(WORD_MACHINE,str)){
            t=token(str,ALPHA);

        }
        else if(s.get_token(PUNC_MACHINE,str)){
            t=token(str,PUNC);
        }
        else if(s.get_token(SPACE_MACHINE,str)){
            t=token(str,SPACE);
        }
        else if(s.get_token(QUOTE_MACHINE,str)){
            t= token(str,QUOTE);
        }
        else{
            t= token(str,UNKNOWN_MACHINE);
            s._pos++;
        }
        return s;

    }
    //set a new string as the input string
    void set_string(char str[]){
        strcpy(_buffer,str);
        _pos=0;
    }

   private:
    //create table for all the tokens we will recognize
        //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COL]){
           bool debug= false;
           //doubles:
           init_table(_table);

           //number machine
           mark_cells(NUMBER_MACHINE, _table, DIGITS, 1);    //state [0] --- DIGITS ---> [1]
           mark_cells(1, _table, DIGITS, 1);    //state [1] --- DIGITS ---> [1]
           mark_cell(NUMBER_MACHINE, _table, '.', 2);  //state [0] --- '.' ------> [2]
           mark_cell(1, _table, '.', 2);  //state [1] --- '.' ------> [2]
           mark_cells(2, _table, DIGITS, 3);    //state [2] --- DIGITS ---> [3]
           mark_cells(3, _table, DIGITS, 3);    //state [3] --- DIGITS ---> [3]
           mark_fail(_table, 0);            //Mark states 0 and 2 as fail states
           mark_success(_table, 1);         //Mark states 1 and 3 as success states
           mark_fail(_table, 2);
           mark_success(_table, 3);
           //word machine
           mark_cells(WORD_MACHINE,_table,ALPHABET,WORD_MACHINE+1);
           mark_cells(WORD_MACHINE+1,_table,ALPHABET,WORD_MACHINE+1);
           mark_fail(_table,WORD_MACHINE);
           mark_success(_table,WORD_MACHINE+1);

       //    //SPACE machine
           mark_cell(SPACE_MACHINE,_table,' ',SPACE_MACHINE+1);
           mark_cell(SPACE_MACHINE+1,_table,' ',SPACE_MACHINE+1);
           mark_fail(_table,SPACE_MACHINE);
           mark_success(_table,SPACE_MACHINE+1);

       //    //PUNC machine
           mark_cells(PUNC_MACHINE,_table,'#','/',PUNC_MACHINE+1);
           mark_cells(PUNC_MACHINE+1,_table,'#','/',PUNC_MACHINE+1);
           mark_cells(PUNC_MACHINE,_table,':','@',PUNC_MACHINE+1);
           mark_cells(PUNC_MACHINE+1,_table,':','@',PUNC_MACHINE+1);
           mark_cells(PUNC_MACHINE,_table,'[','`',PUNC_MACHINE+1);
           mark_cells(PUNC_MACHINE+1,_table,'[','`',PUNC_MACHINE+1);
           mark_cells(PUNC_MACHINE,_table,'{','~',PUNC_MACHINE+1);
           mark_cells(PUNC_MACHINE+1,_table,'{','~',PUNC_MACHINE+1);
           mark_fail(_table,PUNC_MACHINE);
           mark_success(_table,PUNC_MACHINE+1);

           //Quote machine
           mark_fail(_table, 25);
           mark_fail(_table, 26);
           mark_fail(_table, 27);
           mark_success(_table, 28);
           mark_fail(_table, 29);
           mark_cells(25, _table, 34, 34, 26);         //state [25] --- " ------> [26]
           mark_cells(26, _table, ALPHABET, 27);        //state [26] --- LETTERS ---> [27]
           mark_cells(26, _table, PUNCUATION, 27);   //state [26] --- PUNCT ------> [27]
           mark_cells(27, _table, ALPHABET, 27);        //state [27] --- LETTERS ---> [27]
           mark_cells(27, _table, PUNCUATION, 27);   //state [27] --- PUNCT ------> [27]
           mark_cells(27, _table, 34, 34, 28);         //state [27] --- " ------> [28]
           mark_cells(27, _table, ' ', ' ', 29);       //state [27] --- SPACE ------> [29]
           mark_cells(29, _table, ALPHABET, 27);        //state [29] --- LETTERS ---> [27]
           mark_cells(29, _table, PUNCUATION, 27);   //state [29] --- PUNCT ------> [27]

         if(debug){
               printprinttable(_table);
           }
    }
    //extract the longest string that match
        //     one of the acceptable token types
    bool get_token(int start_state, string &token){
        return _get_token(_buffer,_pos,_table,start_state,token);
    }
    int _table[MAX_ROW][MAX_COL];
    char _buffer[MAX_BUF];//input string
    int _pos;//current position in the string

};


#endif // STK_H
