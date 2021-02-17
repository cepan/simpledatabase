#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include<iostream>
#include<cassert>
#include"array_func.h"
#include<iomanip>
using namespace std;

template <class T>
class BPTree
{
public:

    class Iterator{
    public:
        friend class BPTree;
        Iterator(BPTree<T>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}

        T& operator *(){
            assert(key_ptr < it->data_count);
            return (it->data[key_ptr]);
        }

        Iterator operator++(int un_used){
            Iterator temp (it);
            ++ (*this);
            return temp;
        }

        operator bool() const{
            return (it && key_ptr < it->data_count );
        }

        Iterator operator++(){
            assert(it);
            if(key_ptr+1 <= it->data_count-1){
                key_ptr++;
            }else{
                it = it->_next;
                key_ptr =0;
            }
            return *this;
        }


        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return !(lhs==rhs);
        }

        void print_Iterator(){
            cout<<it->data[key_ptr];
        }
        bool is_null(){return !it;}
    private:
        BPTree<T>* it;
        int key_ptr;
    };


    void makeTree(){
        insert(75);
        insert(50);
        insert(25);
        insert(12);
        insert(30);
        insert(60);
        insert(100);
        insert(80);
        insert(110);
    }


    BPTree(bool dups = false){

        dups_ok = dups; data_count = 0; child_count = 0;
        _next = nullptr;

    }


    //big three:
    BPTree(const BPTree<T>& other){
        child_count = 0;
        data_count = 0 ;
        _next = nullptr;
        copy_tree(other);
    }

    ~BPTree(){
        clear_tree();
    }

    BPTree<T>& operator =(const BPTree<T>& RHS){
        if(&RHS != this){
            copy_tree(RHS);
        }
        return *this;
    }

    void insert(const T& entry){
        loose_insert(entry);
        if(data_count >MAXIMUM){
            BPTree<T>* node = new BPTree<T>();
            copy_array(node->subset,subset,node->child_count,child_count);
            copy_array(node->data,data,node->data_count,data_count);

            clearArr(this->data, data_count);
            clearArr(this->subset,child_count);

            subset[0] = node;
            child_count ++;
            fix_excess(0);
        }                //insert entry into the tree
    }

    void remove(const T& entry){
        loose_remove(entry);
        if(data_count < MINIMUM && child_count == 1 ){
            BPTree<T>* temp = subset[0];
            copy_array(data,subset[0]->data,data_count,subset[0]->data_count);
            copy_array(subset,subset[0]->subset,child_count,subset[0]->child_count);
            temp->child_count =0;
            delete temp;
        }
    }                //remove entry from the tree.

    void clear_tree(){

        for(int i = 0; i < child_count ; i ++){
            subset[i]->clear_tree();
            delete subset[i];
            subset[i] = nullptr;
        }
        data_count = 0;
        child_count = 0;
    }                          //clear this object (delete all nodes etc.)
    void copy_tree(const BPTree<T>& other){

        clear_tree();
        const BPTree<T>* temp = other.get_smallest_node();

        while (temp != nullptr) {
            for(int i =0; i< temp->data_count; i++){
                insert(temp->data[i]);
            }
            temp= temp->_next;
        }

    }      //copy other into this object

    bool is_valid(){

        for(int i = 0; i < data_count-1 ; i++){
            if(data[i] > data[i+1])
                return false;
        }

        if (is_leaf())
            return true;

        if (data[data_count-1] > subset[child_count-1]->data[0]){
            return false;
        }

        for(int i = 0 ; i < data_count; i ++){
            for(int k = 0; k <= i ; k++){
                for(int j = 0; j < subset[k]->data_count ; j++){
                    if(!(data[i] >= subset[k]->data[j]) ){
                        return false;
                    }
                }
            }
        }

        for(int i = 0; i < child_count ; i++){
            if (!subset[i]->is_valid())
                return false;
        }

        for(int i = 0; i < data_count ; i++){
            if (data[i] != subset[i+1]->get_smallest_node()->data[0])
                return false;
        }

        return true;
    }



    bool contains(const T& entry) const{

        int i = first_ge(data, data_count, entry);
        bool found = (i<data_count && data[i] == entry);

        if(!is_leaf()){
            if(found)
            return  subset[i+1]->contains(entry);
            else{
                 return  subset[i]->contains(entry);
            }
        }else {
            if(found)
                return true;
            else
               return false;
        }

        return false;
    }              //true if entry can be found in the array

    void print_list() const{

        const BPTree<T>* temp;
        temp = get_smallest_node();
        while (temp){
            print_array(temp->data,temp->data_count);
            cout<<endl;
            temp = temp->_next;
        }
    }

    T& get(const T& entry){
        if(!contains(entry))
            insert(entry);

        return get_exist(entry);

    }                     //return a reference to entry in the tree

   T& get_exist(const T& entry){

       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       if(!is_leaf() && found){
           return subset[i+1]->get(entry);
       }else {
           if(found)
               return data[i];
           else
               return  subset[i]->get(entry);
       }
    }

    const T& get(const T& entry) const{

        if(!contains(entry))
            insert(entry);

        get_exist(entry);
    }                     //return a reference to entry in the tree


    Iterator find(const T& entry){
        int i = first_ge(data, data_count, entry);
        bool found = (i<data_count && data[i] == entry);

        if(!is_leaf()){
            if(found)
            return  subset[i+1]->find(entry);
            else{
                 return  subset[i]->find(entry);
            }
        }else {
            if(found)
                return Iterator(this,i);
            else
               return Iterator(nullptr);
        }

        return Iterator(nullptr);
    }                    //return a pointer to this key. NULL if not there.

    int size() const{
        int i = data_count;
        for(int i =0 ; i < child_count ; i++){
            i += subset[i]->size();
        }
        return i+data_count;
    }                           //count the number of elements in the tree
    bool empty() const{
        return (child_count==0 && data_count == 0);
    }                         //true if the tree is empty


    void print_tree(int level = 0, ostream &outs=cout) const{

        int temp = child_count-1;

        if( temp >= 0 && subset[temp]->data[0] >= data[data_count-1]){
            subset[temp]->print_tree(level+1,outs);
            temp--;
        }

        for(int i = data_count -1; i >=0 ; i --){
            outs<<setw(level*5)<<" "<<data[i];


            outs<<"  c:"<<child_count<<" d:"<<data_count;

            outs<<endl;
            if(temp >= 0){
                subset[temp]->print_tree(level+1,outs);
                temp--;
            }
        }
    } //print a readable version of the tree


    friend ostream& operator<<(ostream& outs, const BPTree<T>& print_me){
        //print_me.print_tree(0, outs);
        print_me.print_list();
        return outs;
    }

    //private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                                   //true if duplicate keys may be inserted
    BPTree* _next;
    int data_count;                                 //number of data elements
    T data[MAXIMUM + 1];                            //holds the keys
    int child_count;                                //number of children
    BPTree* subset[MAXIMUM + 2];                     //subtrees

    bool is_leaf() const {return child_count==0;}   //true if this is a leaf node

    //insert element functions
    void loose_insert(const T& entry){
        int i = first_ge(data, data_count, entry);
        bool found = (i<data_count && data[i] == entry);

        if(is_leaf()){
            if(found){
                data[i] += entry;
            }else{
                ordered_insert(data,data_count,entry);
            }

        }else {
            if(found){
                subset[i+1]->loose_insert(entry);
                fix_excess(i+1);
            } else{
                subset[i]->loose_insert(entry);
                fix_excess(i);
            }
        }

    }              //allows MAXIMUM+1 data elements in the root

    void fix_excess(int i){
        T temp;
        if(subset[i]->data_count > MAXIMUM){
            insert_item(subset, i+1, child_count,new BPTree<T>());
            split(subset[i]->data,subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
            split(subset[i]->subset,subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);
            detach_item(subset[i]->data,subset[i]->data_count,temp);
            ordered_insert(data,data_count,temp);

            if(subset[i]->is_leaf()){
                insert_item(subset[i+1]->data,0,subset[i+1]->data_count,temp);
                BPTree<T>* temp = subset[i]->_next;
                subset[i]->_next = subset[i+1];
                subset[i+1]->_next = temp;
            }
        }
    }                         //fix excess of data elements in child i

    //remove element functions:
    void loose_remove(const T& entry){

        int i = first_ge(data, data_count, entry);
        bool found = (i<data_count && data[i] == entry);

        if(is_leaf()){
            if(found){
                T temp;
                delete_item(data,i,data_count,temp);
            }else{
                return;
            }
        }else{
            if(found){
                assert(i < child_count-1);

                subset[i+1]->loose_remove(entry);

                BPTree<T>* temp  = subset[i+1]->get_smallest_node();

                if( temp -> data_count >= MINIMUM){
                    data[i] = temp->data[0];

                }else if(temp->_next != nullptr){

                    data[i] = temp->_next->data[0];
                }

                fix_shortage(i+1);

            }else{
                subset[i]->loose_remove(entry);
                fix_shortage(i);
            }

        }
    }//allows MINIMUM-1 data elements in the root

    BPTree<T>*  fix_shortage(int i){

        if(subset[i]->data_count < MINIMUM){
            if(child_count > i+1 && subset[i+1]->data_count > MINIMUM){
                rotate_left(i+1);
            } else if (i-1 >=0  && subset[i-1]->data_count > MINIMUM){
                rotate_right(i-1);
            }else if(i-1 >= 0){
                merge_with_next_subset(i-1);
            }else{
                merge_with_next_subset(i);
            }
        }
        return subset[i];

    }                       //fix shortage of data elements in child i

    void rotate_left(int i){

        if((0 < i && i < child_count) && (subset[i]->data_count > MINIMUM)) {

            if(! subset[i]->is_leaf()) {

                T temp;
                delete_item(subset[i]->data, 0,subset[i]->data_count, temp);
                swap(temp,data[i-1]);
                attach_item(subset[i-1]->data, subset[i-1]->data_count,temp);

                BPTree<T>* b;
                if(subset[i]->child_count >0){
                    delete_item(subset[i]->subset, 0,subset[i]->child_count, b);
                    attach_item(subset[i-1]->subset, subset[i-1]->child_count, b);
                }
            }
            else{
                T temp;
                delete_item(subset[i]->data, 0,subset[i]->data_count, temp);
                attach_item(subset[i-1]->data, subset[i-1]->data_count,temp);
                data[i-1] = subset[i]->data[0];
            }


        }
    }                       //transfer one element LEFT from child i

    void rotate_right(int i){
        if((i < child_count - 1) && (subset[i]->data_count > MINIMUM)){

            if(! subset[i]->is_leaf()) {
                T temp;
                detach_item(subset[i]->data,subset[i]->data_count,temp);
                swap(temp,data[i]);
                insert_item(subset[i+1]->data,0, subset[i+1]->data_count,temp);

                BPTree<T>* b;
                if(subset[i]->child_count >0){
                    detach_item(subset[i]->subset, subset[i]->child_count, b);
                    insert_item(subset[i+1]->subset,0, subset[i+1]->child_count,b);
                }

            }else{
                T temp;
                detach_item(subset[i]->data,subset[i]->data_count,temp);
                data[i] = temp;
                insert_item(subset[i+1]->data,0, subset[i+1]->data_count,temp);
            }
        }   // If necessary, shift last subset of subset[i] to front of subset[i+1]

    }                       //transfer one element RIGHT from child i

    void merge_with_next_subset(int i){

        T temp;
        delete_item(data,i,data_count,temp);

        if(!subset[i]->is_leaf()){
            attach_item(subset[i]->data,subset[i]->data_count,temp);
        }

        merge(subset[i]->data,subset[i]->data_count,subset[i+1]->data, subset[i+1]->data_count);
        merge(subset[i]->subset,subset[i]->child_count,subset[i+1]->subset, subset[i+1]->child_count);
        BPTree<T>* b;
        delete_item(subset,i+1, child_count,b);
        if(subset[i]->is_leaf())
            subset[i]->_next= b->_next;
        delete b;

    }             //merge subset i with subset i+1

    void get_smallest(T& item){
        if(is_leaf()){
            item = data[0] ;
        } else{
            return subset[0]->get_smallest(item);
        }
    }

    BPTree<T>* get_smallest_node(){
        if(is_leaf()){
            return this;
        } else{
            return subset[0]->get_smallest_node();
        }
    }

    const BPTree<T>* get_smallest_node() const{
        if(is_leaf())
            return this;
        else
            return subset[0]->get_smallest_node();

    }

    Iterator begin(){
        Iterator it( get_smallest_node());
        return it;
    }

    Iterator end(){
        return Iterator(NULL);
    }
};


#endif // BPLUSTREE_H
