#include<record.h>





Record::Record(){
    for(int i = 0; i < FIELD_AMOUNT; i++){
        _record[i][0] = '\0';
    }
    _rec_no = -1;
}

Record::Record(char str[][MAX_WIDTH]){
    for(int i = 0; i < FIELD_AMOUNT; i++){
        _record[i][0] = '\0';
    }
    for(int i = 0; i < FIELD_AMOUNT; i++){
        strcpy(_record[i], str[i]);
    }
    _rec_no = -1;
}

Record::Record(std::vector<std::string> v){
    for(int i = 0; i < FIELD_AMOUNT; i++){
        _record[i][0] = '\0';
    }
    for(int i = 0; i < v.size(); i++){
        strcpy(_record[i], v[i].c_str());
    }
    _rec_no = -1;
}

long Record::write(std::fstream& outs){
    //write to the end of the file.
    outs.seekp(0, std::ios::end);
    for(int i = 0; i < FIELD_AMOUNT; i++){
        outs.write(_record[i], sizeof(_record[i]));
    }
    //return the record number.
    long pos = outs.tellp();
    return pos/sizeof(_record);
}

long Record::read(std::fstream &ins, long recno){
    //convert recno into bit pos
    long pos= recno*sizeof(_record);
    ins.seekg(pos, std::ios_base::beg);
    for(int i = 0; i < FIELD_AMOUNT; i++){
        ins.read(_record[i], sizeof(_record[i]));
    }

    //return the record number
    _rec_no = recno;
    return _rec_no;
}


