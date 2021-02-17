#include "parser.h"

Parser::Parser(){
    make_table();
    make_map();
}
Parser::Parser(char s[]){
    set_string(s);
    make_table();
    make_map();
}
void Parser::set_string(char s[]){
    _v.clear();
    bool debug = false;
    if(debug){
        cout<<endl<<"==parser:set_string======"<<endl;
        cout<<"s: "<<s<<endl;
    }
    s1.set_string(s);
    token t;
    s1>>t;
    while(s1.failed()){
        if(debug){
                    cout<<"---------------------"<<endl;
                    cout<<"t:"<<t;
                    cout<<"type:"<<t.type_string()<<endl;
                }

        if(t.type_string() == "ALPHA"||t.type_string() =="PUNCU"||t.type_string() =="NUMBER" || t.type_string() == "QUOTE"){
            string token = t.token_str();
            if(t.type_string() == "QUOTE"){

                if(token[0] == '\"'){
                    token = token.substr(1,token.size() -1 );
                }
                if(token.back() == '\"'){
//                        token[token.size()-1] = '\0';
                    token = token.substr(0,token.size()-1);
                }
            }

            _v.push_back(token);
        }
        s1>>t;
    }
//    cout<<"V: "<<_v<<"SIZE: "<<_v.size()<<endl;
    if(debug){
        cout<<"_v.size: "<<_v.size()<<endl;
        for(int i = 0 ; i< _v.size(); i++){
            cout<<_v.at(i);
        }
        cout<<endl;
        cout<<"==parser:set_string======"<<endl<<endl;
    }
}

MMap<string,string>Parser::parse_tree(){
bool debug= false;
if(debug){
    cout<<endl<<"==parser:parse_tree======"<<endl;
}
    MMap<string, string> parseTree;

    int current_state = 0;
    int size =  _v.size();
    for(int i = 0; i < size ; i++){
        if(debug){
            cout<<"start state   "<<current_state<<endl;
        }
        string s = _v[i];
        current_state = _table[current_state][word_to_type(s)];
        if(debug){
            cout<<"next state   "<<current_state<<endl;
        }
        switch (current_state) {
        case SELECT:
        case INSERT:
        case MAKE:
            if(debug){
                cout<<"inserting command   "<<s<<endl;
            }

            parseTree.insert("command",s);
            break;
        case SELECT_TABLE_NAME:
        case INSERT_TABLE_NAME:
        case MAKE_TABLE_NAME:
            if(debug){
                cout<<"inserting table:   "<<s<<endl;
            }

            parseTree.insert("table",s);
            break;

        case MAKE_FIELD_SYM:
        case SELECT_FIELD_NAME:
        case SELECT_STAR:
            if(debug){
                cout<<"inserting fields:   "<<s<<endl;
            }

            parseTree.insert("fields",s);
            break;
        case INSERT_VALUE:
            if(debug){
                cout<<"inserting value:   "<<s<<endl;
            }

            parseTree.insert("value",s);
            break;
        case SELECT_WHERE_FIELD_NAME:
        case RELATIONAL:
        case SELECT_VALUE:
        case LOGICAL:
            if(debug){
                cout<<"inserting condition:   "<<s<<endl;
            }


            parseTree.insert("condition",s);
        default:
            if(debug){
                cout<<"inserting keyword:   "<<s<<endl;
            }

            break;

        }
        if(debug){
            cout<<"****************************************"<<endl;

        }




    }
    if(debug){
            cout<<"==parser:parse_tree======"<<endl<<endl;
    }

    if(_table[current_state][0] == 1 ){
//            cout<<"NOW IS VALID"<<endl;
        _valid = true;
    }else{
//            cout<<"NOW IS NOT VALID"<<endl;
        _valid = false;
    }

    return parseTree;
}

int Parser::word_to_type(string s ){
    bool debug= false;
    if(_key_map.contains(s)){

        if(debug){
            cout<<"Pair::   _key_map[s]"<<_key_map[s]<<endl;
        }

        return _key_map[s];

    }else if (s ==","){
        return COMMA;

    }else{
        return SYMBOL;
    }




}
void Parser::make_table(){
    init_table(_table);

    //MAKE
    _table[ZEROS][MAKE] = MAKE;
    _table[MAKE][MAKE_TABLE] = MAKE_TABLE;
    _table[MAKE_TABLE][SYMBOL] = MAKE_TABLE_NAME;
    _table[MAKE_TABLE_NAME][MAKE_FIELD] = MAKE_FIELD;
    _table[MAKE_FIELD][SYMBOL] = MAKE_FIELD_SYM;
    _table[MAKE_FIELD_SYM][COMMA] = MAKE_COMMA;
    _table[MAKE_COMMA][SYMBOL] = MAKE_FIELD_SYM;
    mark_success(_table, MAKE_FIELD_SYM);
    //INSERT
    _table[ZEROS][INSERT] = INSERT;
    _table[INSERT][INSERT_INTO] = INSERT_INTO;
    _table[INSERT_INTO][SYMBOL] = INSERT_TABLE_NAME;
    _table[INSERT_TABLE_NAME][INSERT_VALUES] = INSERT_VALUES;
    _table[INSERT_VALUES][SYMBOL] = INSERT_VALUE;
    _table[INSERT_VALUE][COMMA] = INSERT_COMMA;
    _table[INSERT_COMMA][SYMBOL] = INSERT_VALUE;
    mark_success(_table, INSERT_VALUE);
    //SELECT
    _table[ZEROS][SELECT] = SELECT;
    _table[SELECT][SELECT_STAR] = SELECT_STAR;
    _table[SELECT_STAR][SELECT_FROM] = SELECT_FROM;
    _table[SELECT_FROM][SYMBOL] = SELECT_TABLE_NAME;
    mark_success(_table,SELECT_TABLE_NAME);
    //condition
    _table[SELECT_TABLE_NAME][SELECT_WHERE] = SELECT_WHERE;
    _table[SELECT_WHERE][SYMBOL] = SELECT_WHERE_FIELD_NAME;
    _table[SELECT_WHERE_FIELD_NAME][RELATIONAL] = RELATIONAL;
    _table[RELATIONAL][SYMBOL] = SELECT_VALUE;
    mark_success(_table,SELECT_VALUE);
    _table[SELECT_VALUE][LOGICAL] = LOGICAL;
    _table[LOGICAL][SYMBOL] = SELECT_WHERE_FIELD_NAME;
//    //SELECT  name, name
    _table[SELECT][SYMBOL] = SELECT_FIELD_NAME;
    _table[SELECT_FIELD_NAME][COMMA] = SELECT_COMMA;
    _table[SELECT_COMMA][SYMBOL] = SELECT_FIELD_NAME;
    _table[SELECT_FIELD_NAME][SELECT_FROM] = SELECT_FROM;

}
void Parser::make_map(){
    //MAKE make table employee fields  last, 		first, 			dep
    _key_map["create"] = KEY::MAKE;

    _key_map["make"] = KEY::MAKE;
    _key_map["table"] = KEY::MAKE_TABLE;
    _key_map["fields"] = KEY::MAKE_FIELD;

   //select * from student
    _key_map["select"] = KEY::SELECT;
    _key_map["*"] = KEY::SELECT_STAR;
    _key_map["from"] = KEY::SELECT_FROM;

    //condition
    _key_map["where"] = KEY::SELECT_WHERE;

    _key_map["<"] = KEY::RELATIONAL;
    _key_map[">"] = KEY::RELATIONAL;
    _key_map["="] = KEY::RELATIONAL;
    _key_map[">="] = KEY::RELATIONAL;
    _key_map["<="] = KEY::RELATIONAL;

    _key_map["and"] = KEY::LOGICAL;
    _key_map["or"] = KEY::LOGICAL;



    _key_map["insert"] = KEY::INSERT;
    _key_map["into"] = KEY::INSERT_INTO;
    _key_map["values"] = KEY::INSERT_VALUES;
}
