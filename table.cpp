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
    f.open(_name + "_fields.txt");
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
    bool debug = false;
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
    bool debug = false;
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
        _indices_map[_field_list[i]].insert(field[i],r1.get_recno());

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
    bool debug = false;
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
    //    cout<<"select is being called"<<endl;

    //setup
    Record r;

    //create new file = _table_name_new
    table new_table(_name+"_new",field);
    fstream f;
    string new_string = new_table._name+".tbl";
    open_fileRW(f,new_string.c_str());


    //open original file
    string original_name = _name+".tbl";
    fstream original;
    open_fileRW(original,original_name .c_str());


    //insert r from original to new file
    original.seekg(0, ios::end);
    long size=original.tellp()/sizeof(char [FIELD_AMOUNT][MAX_WIDTH]);
    original.seekg(0, ios::beg);

    fstream output;
    open_fileRW(output,"output.txt");
    output.seekp(0, std::ios::end);


    //    cout<<"select record amount: "<<size<<endl;
    for(int i=0; i<size; i++){


        //read
        r.read(original,i);

        //take target fields from this record
        vector<unsigned long> fields_list = fields_to_index(field);
        vector<string> record_fields;
        for(int i=0; i<fields_list.size(); i++){
            (record_fields).push_back(r.get_field_i(fields_list[i]));
        }

        //make new record
        Record new_record(record_fields);
        cout<<new_record<<endl;
        output<<new_record<<endl;
        new_table.insert_r_into(new_record);
    }
    output.close();
    f.close();
    original.close();
    return new_table;

}

table table::select_all(){
    return select(_field_list);
}

table table::select_with_cond(vector<string> field, vector<string> cond){
    //    cout<<"select_con is being called"<<endl;
    bool debug=false;
    vector<int> range;

    //evaluate and shunting yard condition into vector<int>
    range = eval(shunting(cond));
    if(debug){
        cout<<endl<<"range:   "<<range<<endl;

    }

    //make new table
    table new_table(_name + "_select_condition", field);
    Record temp;

    //open og file
    fstream bin;
    string filename = _name + ".tbl";
    open_fileRW(bin, filename.c_str());

    fstream output;
    open_fileRW(output,"output.txt");
    output.seekp(0, std::ios::end);

    for(int i=0; i<range.size(); i++){
        //read
        temp.read(bin, range[i]);

        //take target fields from this record
        vector<unsigned long > fields_list = fields_to_index(field);
        vector<string> record_fields;

        for(int i=0; i<fields_list.size(); i++){
            record_fields.push_back(temp.get_field_i(fields_list[i]));
        }

        //make new record
        Record new_record(record_fields);
        cout<<new_record<<endl;
        output<<new_record<<endl;
        new_table.insert_r_into(new_record);
    }

    bin.close();
    return new_table;
}

vector<unsigned long> table::fields_to_index(vector<string> field_list){
    vector<unsigned long> index_list;
    for(unsigned long i=0; i<field_list.size(); i++){
        for(unsigned long j=0; j<_field_list.size(); j++){
            if(field_list[i] == _field_list[j]){
                index_list.push_back(j);
            }
        }
    }
    return index_list;
}

vector<int> table::field_to_relational(string field, string value, string relational){
    vector<int> result;
    if(relational == "="){
        if (_indices_map[field].contains(value))
            result = add_vec(result, _indices_map[field].get(value));
    }
    else if(relational == ">"){
        result = range(_indices_map[field].upper_bound(value), _indices_map[field].end());
    }
    else if(relational == "<"){
        result = range(_indices_map[field].begin(), _indices_map[field].lower_bound(value));
    }
    else if(relational == ">="){
        result = range(_indices_map[field].lower_bound(value), _indices_map[field].end());
    }
    else if(relational == "<="){
        result = range(_indices_map[field].begin(), _indices_map[field].upper_bound(value));
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
        result += b[i];
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

    bool debug= false;

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
    queue <string> debugqueue = output;

    if(debug){
        while (!debugqueue.empty()) {
            cout<<debugqueue.front()<<" ";
            debugqueue.pop();
        }
    }
    return output;
}

vector<int> table::eval(queue<string> rpn){
    stack <string> stack_operation;
    stack <vector<int> > stack_vector;
    vector<int> result;

    while(!rpn.empty()){
        //pop to eval the first element
        string item = rpn.front();
        rpn.pop();

        if((condition_token_type(item) == FUNCTION::FIELD) || (condition_token_type(item) == FUNCTION::VALUE)){
            stack_operation.push(item);

        }else if(condition_token_type(item) == FUNCTION::REALATIONAL_OPERATOR){//< > <= >= ==

            //pop the 2 elements to evaluate
            string value= stack_operation.top();
            stack_operation.pop();

            string field= stack_operation.top();
            stack_operation.pop();


            //            cout<<" FIELD:"<<field;
            //            cout<<" VALUE:"<<value;
            //            cout<<" ITEM: "<<item<<endl;

            result =field_to_relational(field,value,item);
            //            cout<<"3 in 1 SET: "<<result<<endl;
            stack_vector.push(result);

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
