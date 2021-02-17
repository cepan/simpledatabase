#include "table_func.h"


void init_table(int _table[][MAX_COL]){
    for(int i= 0; i<MAX_ROW;i++){
        for(int j=0;j<MAX_COL;j++){
            if(j==0){
                _table[i][j]=0;
            }
            _table[i][j]=-1;

        }
    }
}
void mark_success(int _table[][MAX_COL], int state){
    _table[state][0]=1;

}
void mark_fail(int _table[][MAX_COL], int state){
    _table[state][0]=0;

}
bool is_success(int _table[][MAX_COL], int state){
    return _table[state][0];

}
void mark_cells(int row, int _table[][MAX_COL], int from, int to, int state){
    for(int j=from; j<=to; j++){
        _table[row][j] = state;
}

}
void mark_cells(int row, int _table[][MAX_COL], const char columns[], int state){
    int num_col;
       int i=0;
       while(columns[i]!= '\0'){
           num_col=(int)columns[i];
           _table[row][num_col]=state;
           i++;
       }

}
void mark_cell(int row, int table[][MAX_COL], int column, int state){
    table[row][column]=state;

}
void print_table(int _table[][MAX_COL]){
    for(int i=0; i<MAX_ROW;i++){
        for(int j= 0; j<MAX_COL;j++){
            cout<<setw(2)<<_table[i][j];
        }
        cout<<endl;
    }

}
void show_string(char s[], int _pos){
    cout<<"POS: "<<_pos<<endl;

    int i=0;
       int size = 0;
       while(s[i]!= '\0'){
           cout<<s[i];
           size++;
           i++;
       }
       cout<<size<<"size"<<endl;
       for(int i=1; i<=size; i++){
           if(i==_pos)
               cout<<"^";
           else
               cout<<" ";
       }
       cout<<endl;

}
bool _get_token(string input, int& pos, int _table[][MAX_COL], int start_state, string& t){
    char c = input [pos];//grabing every character
    int current_state = start_state;//assumed starting from 0
    bool success = false;    //default
    int sPos;
    int walker = pos;
    bool debug= false;



    while(c!='\0'||current_state!=-1){
        // if char is not 0-128
        if((int)c > MAX_COL ||c < 0){
            current_state = -1;
            break;
            if(debug){
                cout<<"c is not within 0 to 128"<<endl;
                cout<<"current state is set to "<<current_state<<endl;
            }
        }

        if(debug){
            cout<<"input:"<<input[walker]<<endl;
        }
        c=input[walker];

        current_state = _table[current_state][(int)c];
        if(debug){
            cout<<"processing:   "<<c<<endl;
        }
        if(current_state==-1||c=='\0'){
            break;
        }
        if(is_success(_table,current_state)){
            if(debug){
                cout<<"the state is success!"<<endl;
                cout<<"current_state:    "<<current_state<<endl;
            }
            if(success == false){

                success =true;
            }
            sPos = walker;
            if(debug){
                cout<<"success turned true"<<endl;
                cout<<"recording success position at: "<<sPos<<endl;
            }
        }
        walker++;
        if(debug){
            cout<<"*************************************"<<endl;

        }
    }
    if(debug){
    cout<<endl<<"==============================="<<endl;

    }if(success){
        t = "";
        for(int i = pos; i<=sPos; i++){
            if(debug){
                cout<<"adding the element to the token"<<endl;
            }
            t += input[i];
            if(debug){
                cout<<t<<endl;

            }
        }
        pos = sPos+1;
    }
    return success;

}
void print_part_table(int row,int _table[][MAX_COL],int from, int to){
    for(int i= row; i<row+4;i++){
        cout<<"row:"<<i<<endl;
        for(int j=from;j<=to;j++){
            cout<<setw(3)<<_table[i][j];
        }
        cout<<endl;
    }

}
void print_character(int row,int _table[][MAX_COL],int col){
    for(int i= row; i<row+4;i++){
        cout<<"row:"<<i<<endl;
        cout<<setw(3)<<_table[i][col]<<endl;
        }
        cout<<endl;

}
void printprinttable(int _table[][MAX_COL]){
    cout<<"**************************************"<<endl;
    cout<<endl<<"NUMBER MACHINE: from 0-9"<<endl;
    print_part_table(NUMBER_MACHINE,_table,'0','9');
    cout<<endl<<":"<<endl;
    print_character(NUMBER_MACHINE,_table,'.');
    cout<<endl<<"PUNC MACHINE from ! - /"<<endl;
    print_part_table(PUNC_MACHINE,_table,'!','/');
    cout<<endl<<"PUNC MACHINE from : - @"<<endl;
    print_part_table(PUNC_MACHINE,_table,':','@');
    cout<<endl<<"PUNC MACHINE from [ - `"<<endl;
    print_part_table(PUNC_MACHINE,_table,'[','`');
    cout<<endl<<"PUNC MACHINE from { - ~"<<endl;
    print_part_table(PUNC_MACHINE,_table,'{','~');
    cout<<endl<<"ALPHA MACHINE from A - Z"<<endl;
    print_part_table(WORD_MACHINE,_table,'A','Z');
    cout<<endl<<"ALPHA MACHINE from a - z"<<endl;
    print_part_table(WORD_MACHINE,_table,'a','z');
    cout<<endl<<"SPACE MACHINE"<<endl;
    print_character(SPACE_MACHINE,_table,' ');
    cout<<"********************************************"<<endl<<endl;

}
