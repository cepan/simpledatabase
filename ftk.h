#ifndef FTK_H
#define FTK_H
#include<stk.h>
#include<fstream>


class ftk
{
public:
    const int MAX_BLOCK = MAX_BUF;
    ftk(char* fname){
    _pos=0;
    _blockPos=0;
    _more= false;
    get_new_block();
    _f = ifstream(fname);
    }

    token next_token()
    {
        token t;
        if(!_stk.failed())
        {
            if(!get_new_block())
            {
                return t;
            }
        }
        _stk>>t;
    return t;
    }

    bool more(){        //returns the current value of _more
        return _more;
    }
    int pos(){//returns the value of _pos
        return _pos;
    }
    int block_pos(){//returns the value of _blockPos
        return _blockPos;
    }
    friend ftk& operator >> (ftk& f, token& t){
    t=f.next_token();
    return f;
    }
private:
    bool get_new_block(){//gets the new block from the file
        char block[MAX_BLOCK];
        _f.read(block,MAX_BLOCK);
        if(_f.gcount()<=0){
            _more= false;

            return more();
        }
        block[_f.gcount()]='\0';
        _stk.set_string(block);
        _pos+= MAX_BLOCK;
        _more = true;
        return more();
    }
    std::ifstream _f;   //file being tokenized
    stk _stk;     //The STokenizer object to tokenize current block
    int _pos;           //Current position in the file
    int _blockPos;      //Current position in the current block
    bool _more;         //false if last token of the last block
                        //  has been processed and now we are at
                        //  the end of the last block.
};





#endif // FTK_H
