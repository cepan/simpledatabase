#include "sql.h"




Sql::Sql(){
}

void Sql::run(string filename){

    ofstream f;
    f.open("output.txt", std::ofstream::out | std::ofstream::trunc);
    f.close();


    ifstream input;
    input.open(filename.c_str());
    if(!input.is_open()){
        cout<<"unable to open the file"<<endl;
        return;
    }
    string str;
    int i = 0;
    while(getline(input,str)){
        if(set_string(str)){
            cout<<setw(4)<<i<<"  "<<str<<endl;

            fstream output;
            open_fileRW(output,"output.txt");
            output.seekp(0,std::ios::end);
            output<<setw(4)<<i<<"  "<<str<<endl;
            output.close();

            string table_name = (_ptree["table"][0]);

            if(_ptree["command"][0] == "create" ||
                    _ptree["command"][0] == "make"){


                _table[table_name] = table(table_name,_ptree["fields"]);



            }else if ( _ptree["command"][0] == "insert"){
                _table[table_name] = table(table_name);
                _table[table_name].insert_into(_ptree["value"]);



            }else if(_ptree["command"][0] == "select"){



                _table[table_name] = table(table_name);

                if(!_ptree.contains("condition")){
                    if(_ptree["field"][0] == "*"){
                        _table[table_name].select(_table[table_name].get_field_list());
                    }else{
                        _table[table_name].select(_ptree["fields"]);
                    }
                }else{
                    if(_ptree["field"][0] == "*"){
                        _table[table_name].select_with_cond(_table[table_name].get_field_list(),_ptree["condition"]);
                    }else{
                        _table[table_name].select_with_cond(_ptree["fields"],_ptree["condition"]);
                    }
                }

                i++;

            }else{
                cout<<setw(4)<<str<<endl;

                fstream output;
                open_fileRW(output,"output.txt");
                output.seekp(0,std::ios::end);
                output<<setw(4)<<str<<endl;
                output.close();
            }
        }
    }
}
bool Sql::set_string(string str){
    int size = str.size();
    char command[size+1];
    for (int i = 0 ; i < size; i++){
        command[i] = str[i];
    }
    command[size] = '\0';
    parser.set_string(command);
    _ptree = parser.parse_tree();

    if(parser.is_valid()){
        return true;
    }else{
        return false;
    }
}
