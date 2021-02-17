#include"record.h"





Record::Record(){

    for(int i = 0; i<FIELD_AMOUNT;i++){
        _record[i][0] = '\0';
    }
    _rec_no = -1;

}

Record::Record(char str[][MAX_WIDTH]){
    for(int i = 0; i<FIELD_AMOUNT;i++){
        _record[i][0] = '\0';
    }

    for(int i = 0 ;i< FIELD_AMOUNT; i++){
        strcpy(_record[i], str[i]);
    }

    _rec_no = -1;

}

Record::Record(vector<string> v){
    for(int i = 0 ; i<FIELD_AMOUNT ; i++){
        _record[i][0] = '\0';
    }

    for(int i = 0 ; i< v.size() ; i++){
        strcpy(_record[i], v[i].c_str());
    }

    _rec_no = -1;

}
long Record::write(fstream &outs){

    bool debug= false;
    outs.seekp(0,ios::end);
    long pos = outs.tellp();
    for(int i = 0 ; i <FIELD_AMOUNT; i++){
        outs.write(_record[i], sizeof(_record[i]));
    }

    _rec_no= pos/sizeof(_record);
    if(debug){
        cout<<"==Record::write=============="<<endl;
        cout<<"rec_no"<<_rec_no<<endl;
        cout<<"==Record::write=============="<<endl;
    }
    return _rec_no;


}

long Record::read(fstream &ins, long recno){
    long pos= _rec_no * sizeof(_record);
    ins.seekg(pos,ios::beg);

    for( int i = 0 ; i < FIELD_AMOUNT; i++){
        ins.read(_record[i], sizeof(_record[i]));
    }

    _rec_no = recno;
    return _rec_no;
}


