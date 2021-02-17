#ifndef MMAP_H
#define MMAP_H
#include<iostream>
#include<vector>
#include"mpair.h"
#include"bplustree.h"

using namespace std;

template <typename K, typename V>
class MMap
{
public:


    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it= nullptr):_it(it){}
        Iterator operator ++(int unused){
            Iterator temp = *this;
            ++_it;
            return temp;
        }
        Iterator operator ++(){
            ++_it;
            return *this;
        }
        MPair<K, V> operator *(){
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs._it== rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return lhs._it!= rhs._it;
        }

    private:
        typename map_base::Iterator _it;
    };





    MMap(){}

    //  Iterators
        Iterator begin(){return mmap.begin();}
        Iterator end(){return mmap.end();}

//  Capacity
    int size() const{return mmap.size();}
    bool empty() const{return mmap.empty();}

//  Element Access
    const vector<V>& operator[](const K& key) const{
        MPair<K,V>item(key, V());
        return mmap.get(item).value_list;
    }
    vector<V>& operator[](const K& key){
        MPair<K,V>item(key, V());
        return mmap.get(item).value_list;

    }

//  Modifiers
    void insert(const K& k, const V& v){
        MPair<K,V> item(k,v);
        bool debug= false;
        if(debug){
            cout<<"mmap insert item"<<item<<endl;
        }
        if(mmap.contains(item)){
            mmap.get(item).value_list.push_back(v);
        }else{
            mmap.insert(item);

        }
    }




    void erase(const K& key){
        MPair<K,V> item(key,V());
        mmap.remove(item);
    }




    void clear(){
        mmap.clear_tree();
    }

//  Operations:
    bool contains(const K& key) const {
        MPair<K,V> item(key,V());
        return mmap.contains(item);
    }
    vector<V> &get(const K& key){
        MPair<K,V> item(key,V());
        return mmap.get(item).value_list;

    }

    Iterator find(const K& key){
        return mmap.find(key);
    }
    Iterator lower_bound(const K& key){
        Iterator walker = mmap.begin();
        while(*walker < key){
            ++walker;
        }
        return walker;
    }
    Iterator upper_bound(const K& key){
        Iterator walker = mmap.begin();
        while(*walker > key){
            ++walker;
        }
        return walker;
    }

    int count(const K& key){//return the # of value in a key
        MPair<K,V> item(key,V());

        return item.value_list.size();
    }
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:



    bool is_valid(){
    return mmap.is_valid();
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

private:
    BPlusTree<MPair<K, V> > mmap;
};


#endif // MMAP_H
