#ifndef MAP_H
#define MAP_H
#include"bplustree.h"
#include"pair.h"
#include<iostream>



using namespace std;
template<typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V> > map_base;

    class Iterator{
    public:
        friend class Map;
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
        Pair<K, V> operator *(){
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

    Map(){
        key_count =0;
    }
    //  Iterators

        Iterator begin(){
            return map.begin();
        }
        Iterator end(){
            return map.end();
        }

//  Capacity
    int size() const{
        return key_count;
    }
    bool empty() const{
    return key_count= 0 ;
    }

//  Element Access
    V& operator[](const K& key){
        if(DEBUG)
            cout<<"[] in map is being called"<<endl;
        Pair<K,V> item (key,V());
        return map.get(item).value;
    }
    const V& operator[](const K& key)const {
        Pair<K,V> item (key,V());
        return map.get(item).value;
    }

    V& at(const K& key){
        Pair<K,V> item (key,V());
        return map.get(item).value;

    }
    const V& at(const K& key) const{
        Pair<K,V> item (key,V());
        return map.get(item).value;

    }


//  Modifiers
    void insert(const K& k, const V& v){

        Pair<K,V>item(k,v) ;

        map.insert(item);
        key_count= map.size();
    }


    void erase(const K& key){



            map.remove( Pair<K,V>(key,V()));
            key_count=map.size();
    }


    void clear(){
        map.clear_tree();
    }
    V get(const K& key){
        return map.get(key);
    }

//  Operations:
    Iterator find(const K& key){
        return map.find(key);
    }
    Iterator lower_bound(const K& key){
        Iterator walker = map.begin();
        while(*walker < key){
            ++walker;
        }
        return walker;
    }
    Iterator upper_bound(const K& key){
        Iterator walker = map.begin();
        while(*walker > key){
            ++walker;
        }
        return walker;
    }

    bool contains(const Pair<K, V>& target) const{
        return map.contains(target);
    }

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:


    bool is_valid(){return map.is_valid();}

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};


#endif // MAP_H
