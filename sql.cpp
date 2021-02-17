#include "sql.h"




Sql::Sql(){
}
bool Sql::set_string(string str){
    parser.set_string(str.c_str());

    _ptree = parser.parse_tree();

    return parser.is_valid()
}

void Sql::run(string filename){
    ofstream ofs;
        ofs.open("output.txt", std::ofstream::out | std::ofstream::trunc);
        ofs.close();

        ifstream input;
        input.open(filename.c_str());
        if(!input.is_open()){
            cout<<"error"<<endl;
            return;
        }

        string str;
        int i = 0;
        while (getline(input,str)){
            if(set_string(str)){
                //print the command
                cout<<"***************************************"<<endl;

                cout<<setw(4)<<"["<<i<<"]  "<<str<<endl;
                cout<<"***************************************"<<endl;

                //write the command into a text file
                fstream output;
                open_fileRW(output,"output.txt");
                output.seekp(0, std::ios::end);
                output<<setw(4)<<i<<"  "<<str<<endl;
                output.close();

                string table_name = (_ptree["table"][0]);

                //CREATE
                if(_ptree["command"][0] == "create" || _ptree["command"][0] == "make"){
                    _table[table_name] = table(table_name, _ptree["fields"]);
                }

                //INSERT
                else if(_ptree["command"][0] == "insert"){
                    //open the table, insert the value into it
                    _table[table_name] = table(table_name);
                    _table[table_name].insert_into(_ptree["value"]);
                }

                //SELECT
                else if(_ptree["command"][0] == "select"){
                    //Open the table first
                    _table[table_name] = table(table_name);



                    //If there is no condition
                    if(!_ptree.contains("condition")){
                        //Select all fields
                        if(_ptree["fields"][0] == "*"){
                            _table[table_name].select_all();
                        }
                        //Select the desired fields
                        else{
                            _table[table_name].select(_ptree["fields"]);
                        }
                    }
                    //If there is a condition
                    else{
                        //Select all the fields with condition
                        if(_ptree["fields"][0] == "*"){
//                            cout<<"condition:   "<<_ptree["condition"]<<endl;
                            _table[table_name].select_with_cond(_table[table_name].get_field_list(), _ptree["condition"]);
                        }
                        //Select the desired fields with condition
                        else{
                            _table[table_name].select_with_cond(_ptree["fields"], _ptree["condition"]);
                        }
                    }
                }
                i++;

            }

            else{
                cout<<setw(4)<<str<<endl;

                //write the command into a text file
                fstream output;
                open_fileRW(output,"output.txt");
                output.seekp(0, std::ios::end);
                output<<setw(4)<<str<<endl;
                output.close();
            }
        }}
