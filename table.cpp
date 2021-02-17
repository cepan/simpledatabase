#include "table.h"

table::table(){}

table::table(string name):_name(name){
    /*existing table
     *for starter hard code the table
     * you must read the name of the field in
     * a txt file: student field
     * create a map (or vector)of multimap,
     * and reindext the ta
     */

    //set up
    bool debug = false;
    if(debug){
        cout<<endl<<"==table::constructor (string name)======"<<endl;
    }
    string field_name;
    fstream f;
    f.open(_name + "_field.txt");
    if(f.fail()){
        cout<<"unable to open file"<<endl;
    }else{
        while(f>> field_name){
            _field_list.push_back(field_name);
        }
    }
    fstream f_tbl;
    string file = _name + ".tbl";
    open_fileRW(f_tbl,file.c_str());


    Record temp_record;

    f_tbl.seekg(0, ios::end);
    long size = f_tbl.tellp()/sizeof(char [FIELD_AMOUNT][MAX_WIDTH]);     //??
    f_tbl.seekg(0, ios::beg);

    for(int i =0 ; i < size; i ++){
        temp_record.read(f_tbl,i);
//        cout<<temp_record<<endl;
        for(int i=0;i<_field_list.size(); i++){
            _indices_map[_field_list[i]].insert(temp_record.get_field_i(i),temp_record.get_recno());

        }


    }
    if(debug){
        cout<<endl<<"==table::constructor (string name)======"<<endl;
    }
}

table::table(const string &name, const vector<string> &field_list):_name(name), _field_list(field_list){
    //the txt and the tbl file hasn't been created
    //set up
    bool debug = true;
    if(debug){
        cout<<endl<<"==BEGIN::table::constructor (string name, vector<string> field_list)======1"<<endl;
    }




    string output = _name + "_fields.txt";
    fstream f;
    open_fileRW(f,output.c_str());

    for(int i =0 ; i < _field_list.size(); i ++){
        f << _field_list[i] <<endl;
    }




     string file = _name + ".tbl";
     fstream f_tbl;
     open_fileW(f_tbl, file.c_str());


     f.close();




     if(debug){
        cout<<endl<<"==END::table::constructor (string name, vector<string> field_list)======2"<<endl;
    }

}

void table::insert_into(vector<string > field){
    bool debug = true;
    if(debug){
        cout<<endl<<"==BEGIN::table::insert_into() ======1"<<endl;
    }


    Record r1(field);
    fstream f;
    string temp_name= _name + ".tbl";
    open_fileRW(f, temp_name.c_str());
    r1.write(f);
    int a=_field_list.size();

    for(int i=0;i<a; i++){
        _indices_map[_field_list[i]].insert(r1.get_field_i(i),r1.get_recno());

    }
    f.close();
    if(debug){

        /*
        cout<<_field_list.size()<<endl;
       cout<<"fields:"<<field<<endl;
       cout<<"insert is being called"<<endl;
       cout<<"temp:"<<temp_name<<endl;
      cout<<"_table_name:"<<_name<<endl;
       cout<<"in insert f.tellp():"<<f.tellp()<<endl;
       cout<<"r.rec_no:"<<r1.get_recno()<<endl;
       */

        cout<<endl<<"==END::table::insert_into() ======2"<<endl;
    }


}
void table::insert_r_into(Record r ){
    bool debug = true;
    if(debug){
        cout<<endl<<"==BEGIN_table::insert_record_into() ======1"<<endl;
    }

    fstream f;
    string temp_name= _name + ".tbl";
    open_fileRW(f,temp_name.c_str());
    r.write(f);
    int a=_field_list.size();
    for(int i=0;i<a; i++){
        _indices_map[_field_list[i]].insert(r.get_field_i(i),r.get_recno());

    }
    f.close();






    if(debug){
        cout<<endl<<"==END_table::insert_record_into() ======2"<<endl;
    }
}

table table::select(vector <string> field){
    //setup
    bool debug = false;
    if(debug){
        cout<<endl<<"==table::select(vector <string> new_field) ======1"<<endl;
    }
        Record r;
        //since we are filtering whatever we need,
        //we create a new file and stuff those in there.

        //constructor opens a new file
        table temp(_name+"_update",field);
        fstream f;
        string new_string = temp._name+".tbl";
        open_fileRW(f,new_string.c_str());


        //open original file
        string original_name = _name+".tbl";
        fstream original;
        open_fileRW(original,original_name .c_str());


        //insert r from original to new file
        f.seekp(0,ios::end);                                                                           //or seekg(0.f.ios::end)
        long size=f.tellp()/sizeof(char [FIELD_AMOUNT][MAX_WIDTH]);     //??
        f.seekp(0, ios::beg);

        for(int i=0; i<size; i++){
            r.read(original,i);
            vector<unsigned long> index_list= fields_to_index(field);
            vector<string> r_val_list;
            for(int i = 0 ; i < index_list.size(); i ++){
                r_val_list.push_back(r[index_list[i]]);
            }
            Record new_r(r_val_list);
//            cout<<new_r<<endl;
            temp.insert_r_into(new_r);

        }
                                                                //close??
//        f.close();
        original.close();

        if(debug){
            cout<<endl<<"==table::select(vector <string> new_field) ======2"<<endl;
        }

        return temp;
}
table table::select_all(){
    bool debug= true;
    if(debug){
        cout<<"==BEGIN::table::select_all======1"<<endl;
    }
    table temp(_name + "selected",_field_list);
    Record temp_record;

    fstream f;
    string string_name = _name + ".tbl";
    open_fileRW(f,string_name.c_str());

    f.seekg(0, ios::end);
    long size = f.tellp()/sizeof(char [FIELD_AMOUNT][MAX_WIDTH]);     //??
    f.seekg(0, ios::beg);

    for(int i =0 ; i < size; i ++){
        temp_record.read(f,i);
//        cout<<temp_record<<endl;
        temp.insert_r_into(temp_record);
    }
                                                                    //close??
//    f.close();
    if(debug){
        cout<<"==END::table::select_all======2"<<endl;
    }

    return temp;

}
table table::select_with_cond(vector<string> field, vector<string> cond){
     vector<int> recnum_set = eval(shunting(cond)) ;

     table temp(_name + "_cond", field);
     Record r_temp;

     fstream f;
     string filename = _name + ".tbl";
     open_fileRW(f,filename.c_str());


     for(int i = 0 ; i < recnum_set.size(); i++){
         r_temp.read(f,recnum_set[i]);


         vector<unsigned long> index_list = fields_to_index(field);
         vector<string> record_val_list;
         for(int i = 0 ; i < recnum_set.size(); i++){
             record_val_list.push_back(r_temp[index_list[i]]);
         }
         Record new_rec(record_val_list);
         temp.insert_r_into(new_rec);
     }
return temp;
}
vector<unsigned long> table::fields_to_index(vector<string> field_list){
    vector<unsigned long> index_list;
    for(unsigned long i = 0 ; i < field_list.size(); i ++){
        for(unsigned long j = 0 ; j < _field_list.size(); j ++){
            if(field_list[i] == _field_list[i]){
                index_list.push_back(j);
            }
        }
    }
    return index_list;
}
vector<int> table::field_to_relational(string field, string value, string relational){
    vector<int> result;
    if(relational == "="){
        result = add_vec(result, _indices_map[field].get(value));
    }else if(relational == "<"){
        result = range(_indices_map[field].begin(),_indices_map[field].lower_bound(value));
    }else if(relational == ">"){
        result = range(_indices_map[field].upper_bound(value),_indices_map[field].end());
    }else if(relational == "<="){
        result = range(_indices_map[field].begin(),_indices_map[field].upper_bound(value));
    }else if(relational == ">="){
        result = range(_indices_map[field].lower_bound(value),_indices_map[field].end());

    }

    return result;


}
vector<int> table::range(MMap<string, int >::Iterator from, MMap<string, int >::Iterator to){

    vector<int> result;
    while((from != to)){
        result = add_vec(result, (*from).value_list);
        from++;
    }
    return result;


}
vector<int > table::add_vec (vector<int> a, vector <int> b){
    vector<int> result = a ;
    for(unsigned long i = 0; i<b.size(); i++){

    result.insert(end(a),begin(b),end(b));

    }
    return result;
}
int table::priority(string s ){
    if(s == "=" | s == "<" | s == ">" | s == ">=" | s == "<=" ){
        return 3;
    }else if(s == "and"){
        return 2;

    }else if(s == "or"){
        return 1;
    }else{
        return 0;
    }

}
int table::condition_token_type(string s ){
    if(s == "=" | s == "<" | s == ">" | s == ">=" | s == "<=" ){
        return FUNCTION::REALATIONAL_OPERATOR;
    }else if(s == "and" || s == "or"){
        return FUNCTION::LOGICAL_OPERATOR;

    }else if(is_field(s)){
        return FUNCTION::FIELD;
    }else{
        return FUNCTION::VALUE;
    }

}
queue<string> table:: shunting(vector <string>condition){// infix to postfix
    queue <string> output;
    stack<string> optstck;



    for(unsigned long i = 0; i<condition.size(); i++){
        if((condition_token_type(condition[i]) == FUNCTION::FIELD) || (condition_token_type(condition[i]) == FUNCTION::VALUE)){
            output.push(condition[i]);
        }else if((condition_token_type(condition[i]) == FUNCTION::REALATIONAL_OPERATOR) ||
                 (condition_token_type(condition[i])==FUNCTION::LOGICAL_OPERATOR)){
            string token = condition[i];

            if(! optstck.empty()){
                while(!optstck.empty()&&priority(token) <= priority(optstck.top())){
                    string item = optstck.top();
                    optstck.pop();
                    output.push(item);
                }
                optstck.push(token);

            }else{
                optstck.push(token);
            }
        }
    }
    while(!optstck.empty()){
        string item1 = optstck.top();
        optstck.pop();
        output.push(item1);
    }
    return output;
}
vector<int> table::eval(queue<string> rpn){

    stack <string> stack_token;
    stack <vector<int> > stack_vector;
    vector<int> result;

    while(!rpn.empty()){
        string item = rpn.front();
        rpn.pop();
        if((condition_token_type(item) == FUNCTION::FIELD) || (condition_token_type(item) == FUNCTION::VALUE)){
            stack_token.push(item);

        }else if(condition_token_type(item) == FUNCTION::REALATIONAL_OPERATOR){
            string value= stack_token.top();
            stack_token.pop();
            string field= stack_token.top();
            stack_token.pop();
            vector<int>new_result =field_to_relational(field,value,item);
            stack_vector.push(new_result);
        }else if(condition_token_type(item) == FUNCTION::LOGICAL_OPERATOR){
            vector<int> a = stack_vector.top();
            stack_vector.pop();
            vector<int> b = stack_vector.top();
            stack_vector.pop();
            if( item == "and"){
                result = intersection(a,b);
            }else if(item == "or"){
                result = union_set(a,b);
            }
            stack_vector.push(result);
        }

    }

    result = stack_vector.top();
    stack_vector.pop();
    return result;
}
vector<int> table::intersection(vector<int> a , vector<int> b ){
    vector<int> result;
    sort(a.begin(),a.end());
    sort(b.begin(),b.end());
    set_intersection(a.begin(),a.end(),b.begin(),b.end(),inserter(result,result.begin()));
    return result;
}
vector<int> table::union_set(vector<int> a , vector<int> b ){
    vector<int> result;
    sort(a.begin(),a.end());
    sort(b.begin(),b.end());
    set_union(a.begin(),a.end(),b.begin(),b.end(),inserter(result, result.begin()));
    return result;
}
bool table::is_field(string s ){
    for(unsigned long i = 0; i < _field_list.size(); i ++ ){
        if(s == _field_list[i]){
            return true;
        }
    }
    return false;
}
