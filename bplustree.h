#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include<iostream>
#include<cassert>
#include<array_func.h>

using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}

        T operator *(){
            assert(key_ptr<it->data_count);
            return it->data[key_ptr];
        }

        //the reason we have 2 ++ is because that we can do operation such as
        //++i and i++
        // the differece between the two is that (for example)when we cout<< ++i inside the for loop ;
        //the sys operate ++ first and then cout. other the other hand when we do i++ inside the for loop,
        // it operates the cout first, and then ++;

        Iterator operator++(int un_used){// we need to store the og element somewhere and then incrementing it
            Iterator temp(it);
            ++it;
            return temp;
        }

        Iterator operator++(){


            if(key_ptr< it->data_count-1){
                key_ptr++;
            }else{
                it= it->next;
                key_ptr=0;
            }
            it = it->next;
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return (lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr)? true:false;
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return (lhs.it != rhs.it&& lhs.key_ptr != rhs.key_ptr)? true:false;
        }
        void print_Iterator(){
            it->print_tree();
        }
        bool is_null(){return !it;}
    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree(bool dups = false):dups_ok(dups),data_count(0),child_count(0),next(NULL){

    }
    //big three:
    BPlusTree(const BPlusTree<T>& other){
        data_count = 0;
        for(int i=0; i<MAXIMUM +2; i++){

            subset[i] = nullptr;
        }
        child_count = 0;
        next = nullptr;
        copy_tree(other);
    }
    ~BPlusTree(){
        clear_tree();
    }
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS){
        //check for self assignment
        if (this == &RHS) {
            return *this;
        }

        //delete previous space
        clear_tree();

        //copy stuff over and return *this
        copy_tree(RHS);

        return *this;
    }

    void insert(const T& entry){                //insert entry into the tree
        bool debug=false;
        if(debug){
            cout<<"(bplustree)insert entry"<<entry<<endl;
        }
        loose_insert(entry);

        if(data_count>MAXIMUM){
            BPlusTree<T>* temp = new BPlusTree<T>;
            copy_array(temp->data,data, temp->data_count,data_count);
            copy_array(temp->subset,subset, temp->child_count,child_count);

            data_count=0;
            child_count=1;
            subset[0]= temp;
            fix_excess(0);


        }
    }
    void remove(const T& entry){                //remove entry from the tree
        /*
         * ---------------------------------------------------------------------------------
         * Same as BTree:
         * Loose_remove the entry from this tree.
         * Shrink if you have to
         * ---------------------------------------------------------------------------------
         * once you return from loose_remove, the root (this object) may have no data and
         * only a single subset:
         * now, the tree must shrink:
         *
         * point a temporary pointer (shrink_ptr) and point it to this root's only subset
         * copy all the data and subsets of this subset into the root (through shrink_ptr)
         * now, the root contains all the data and poiners of it's old child.
         * now, simply delete shrink_ptr, and the tree has shrunk by one level.
         * Note, the root node of the tree will always be the same, it's the
         * child node we delete
         *
         *
         */
        bool debug = false ;
        if(debug){
            cout<<"remove called..."<<endl;
        }
        loose_remove(entry);

        if(data_count<MINIMUM && child_count >0){
            BPlusTree<T>* shrink_ptr= subset[0];
            copy_array(data,shrink_ptr->data, data_count,shrink_ptr->data_count);
            copy_array(subset,shrink_ptr->subset, child_count,shrink_ptr->child_count);

            //take care shrink_ptr child_count=0?;
            shrink_ptr->child_count=0;
            delete shrink_ptr;
            shrink_ptr= NULL;

        }



    }

    void clear_tree(){                          //clear this object (delete all nodes etc.)
        if (!is_leaf()) {
            for (int i = 0; i < child_count; ++i) {
                subset[i]->clear_tree();
                delete subset[i];
            }
            child_count = 0;
            data_count = 0;
        }
    }
    void copy_tree(const BPlusTree<T>& other){  //copy other into this object
        clear_tree();                           //clear tree
        const BPlusTree<T>*temp = other.get_smallest_node();      //starting from the smallest_node
        while(temp!= nullptr){                  //work the way up
            for (int i = 0; i<temp->data_count;i++){
                insert(temp->data[i]);
            }
            temp= temp->next;

        }


    }
    bool contains(const T& entry) const{       //true if entry can be found in the array
        int i;
        bool found;
        i = first_ge(data, data_count, entry);
        found = (i < data_count && data[i] == entry);
        if (found) {
            if (is_leaf()) {
                return found;
            } else {
                return subset[i + 1]->contains(entry);
            }
        } else {
            if (!is_leaf()) {
                return subset[i]->contains(entry);
            }
        }
        return found;


    }

    T& get(const T& entry){                     //return a reference to entry in the tree
        //If entry is not in the tree, add it to the tree
//        assert(contains(entry));
        if (!contains(entry)){
            insert(entry);
        }
        return get_existing(entry);



    }
    const T& get(const T& entry)const{          //return a reference to entry in the tree
        assert(contains(entry));
        bool debug = false;
        if(debug){
            cout<<"const get const being called "<<endl;
        }
        int i = first_ge(data, data_count, entry);
        if(data[i]== entry&&i<data_count){
            return data[i];
        }else{
            return subset[i]->get(entry);
        }



    }
    T& get_existing(const T& entry){            //return a reference to entry in the tree
//        assert(contains(entry));
        const bool debug = false;
        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);
        if (is_leaf()) {
            if (found) {
                return data[i];
            } else {
                if (debug) {
                    cout << "get_existing was called with nonexistent entry" << endl;
                }
                assert(found);
            }
        }
        if (found) {
            return subset[i + 1]->get(entry);
            //or just return true?
        } //inner node
        else {
            return subset[i]->get(entry);
        } //not found yet...
    }

    Iterator find(const T& entry){              //return an iterator to this key. NULL if not there.
        bool found; //todo change to iterator
        int i;
        if (!contains(entry)) {
            return NULL;
        }
        i = first_ge(data, data_count, entry);
        found = (i < data_count && data[i] == entry);
        if (is_leaf()) {
            if (found) {
                return &data[i];
            }
        } else {
            if (found) {
                return &data[i];
            } else {
                subset[i]->find(entry);
            }
        }
    }

    int size() const{                           //count the number of elements in the tree
        int count = 0;
        if (is_leaf()) {
            if (data_count > 0) {
                count++;
            }
        } else {
            for (int i = 0; i < child_count; ++i) {
                count += subset[i]->size();
            }
            count++;
        }
        return count;
    }
    bool empty() const{ return data_count= 0 ; }                       //true if the tree is empty


    void print_tree(int level = 0,
                    ostream &outs=cout) const{ //print a readable version of the tree
        if(!is_leaf())
        {
            subset[child_count-1]->print_tree(level+1,outs);

            for(int i=child_count-2; i>=0; i--){
                outs<<setw(level*15)<<data[i]<<endl;
                subset[i]->print_tree(level+1,outs);
            }
        }
        else{
            for(int i=data_count-1; i>=0; i--){
                outs<<setw(level*15)<<data[i]<<endl;
            }
        }




    }



    friend ostream& operator<<(ostream& outs,
                               const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid(){
            if (is_leaf())
                return true;
            else{
                //should check that every data[i] < data[i+1]
                for(int i = 0; i < data_count-1 ; i++){
                    if(data[i] > data[i+1])
                        return false;
                }

                //data[data_count-1] must be less than equal to
                //every subset[child_count-1]->data[ ]
                for(int i = 0; i < subset[child_count-1]->data_count; i++){
                    if (data[data_count-1] > subset[child_count-1]->data[i])
                        return false;
                }

                //every data[i] is greater than every subset[i]->data[ ]
                //B+Tree: Every data[i] is equal to subset[i+1]->smallest
                //Recursively validate every subset[i]
                for(int i = 0; i < data_count; i++){
                    for(int j = 0; j < subset[i]->data_count; j++){
                        if(data[i] < subset[i]->data[j])
                            return false;
                    }
                    T smallest;
                    subset[i+1]->get_smallest(smallest);
                    if(data[i] != smallest)
                        return false;
                    if (!subset[i]->is_valid())
                        return false;
                }
                return true;
            }
        }



    Iterator begin(){
        return Iterator(get_smallest_node());
    }
    Iterator end(){
        return Iterator(NULL);
    }


    private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                       //true if duplicate keys may be inserted
    int data_count;                     //number of data elements
    T data[MAXIMUM + 1];                //holds the keys
    int child_count;                    //number of children
    BPlusTree* subset[MAXIMUM + 2];     //subtrees
    BPlusTree* next;
    bool is_leaf() const{return child_count==0;}        //true if this is a leaf node
    T* find_ptr(const T& entry){}        //return a pointer to this key. NULL if not there.




    //insert element functions
    void loose_insert(const T& entry){ /*  //allows MAXIMUM+1 data elements in the root
       int i = first_ge(data, data_count, entry);
       bool found = (i<data_count && data[i] == entry);

       three cases:
         found
         a. found/leaf: deal with duplicates: call +
         b. found/no leaf: subset[i+1]->loose_insert(entry)
                           fix_excess(i+1) if there is a need

         ! found:
         c. !found / leaf : insert entry in data at position i
         c. !found / !leaf: subset[i]->loose_insert(entry)
                            fix_excess(i) if there is a need

            |   found          |   !found         |
      ------|------------------|------------------|-------
      leaf  |a. dups? +        | c: insert entry  |
            |                  |    at data[i]    |
      ------|------------------|------------------|-------
            | b.               | d.               |
            |subset[i+1]->     | subset[i]->      |
      !leaf | loose_insert(i+1)|  loose_insert(i) |
            |fix_excess(i+1)   | fix_excess(i)    |
            |                  |                  |
      ------|------------------|------------------|-------
    */
       bool debug= false;
        int i= first_ge(data, data_count, entry);
        bool found = (i<data_count && data[i] == entry);

        if(found){
            if(!is_leaf()){                                     //found but not leaf
                subset[i+1]->loose_insert(entry);
                if(subset[i+1]->data_count > MAXIMUM){
                    fix_excess(i+1);
                }
            }else{                                              //found but is leaf
                data[i]= data[i]+entry;

            }
        }else{
            if(!is_leaf()){                                     //not found , not leaf
                subset[i]->loose_insert(entry);
                if(subset[i]->data_count > MAXIMUM){
                    fix_excess(i);
                }

            }else{
                //not found is leaf
                if(debug){
                    cout<<"==bplus::loose_insert::ordered_insert======"<<endl;
                }
                ordered_insert(data,data_count,entry);
                //                insert_item(data,i, data_count,entry);
            }

        }
        //check

    }
    void fix_excess(int i){             //fix excess of data elements in child i
        //this node's child i has one too many items: 3 steps:
        //1. add a new subset at location i+1 of this node
        //2. split subset[i] (both the subset array and the data array) and move half into
        //      subset[i+1] (this is the subset we created in step 1.)
        //3. detach the last data item of subset[i] and bring it and insert
        //      it into this node's data[]
        //
        //Note that this last step may cause this node to have too many itesm.
        //  This is OK. This will be
        //  dealt with at the higher recursive level. (my parent will fix it!)
        //
        //NOTE: ODD/EVENNESS
        // when there is an excess, data_count will always be odd
        //  and child_count will always be even.
        //  so, splitting is always the same.

        //  000 B_PLUS_TREE
        //  if (subset[i]->is_leaf())
        //  transfer the middle entry to the right and...
        //  Deal with next pointers. just like linked list insert
        insert_item(subset, i+1, child_count, new BPlusTree<T>);

        split(subset[i]->data,  subset[i]->data_count,  subset[i+1]->data,  subset[i+1]->data_count);
        //move n/2 elements from the right of data1 and move to data2

        split(subset[i]->subset,  subset[i]->child_count,  subset[i+1]->subset,  subset[i+1]->child_count);
        //move the subset of the n/2 elements from the right of data1 and move to data2

        T bubble;
        detach_item(subset[i]->data,subset[i]->data_count,bubble);

        ordered_insert(data, data_count, bubble);

        if(subset[i]->is_leaf()){

            subset[i+1]->insert(bubble);
            BPlusTree<T>*temp= subset[i]->next;
            subset[i]->next= subset[i+1];
            subset[i+1]->next= temp;

        }


        //check

    }





    //remove element functions:
    void loose_remove(const T& entry){  //allows MINIMUM-1 data elements in the root

        /* four cases:
                   leaves:
                        a. not found: there is nothing to do
                        b. found    : just remove the target
                   non leaf:
                        c. not found: subset[i]->loose_remove, fix_shortage(i)
                        d. found    : subset[i+1]->loose_remove, fix_shortage(i+1) [...]
                            (No More remove_biggest)

                     |   !found               |   found                 |
               ------|------------------------|-------------------------|-------
               leaf  |  a: nothing            | b: delete               |
                     |     to do              |    target               |
               ------|------------------------|-------------------------|-------
               !leaf | c:                     | d: B_PLUS_TREE          |
                     |  [i]->  loose_remove   |   [i+1]-> loose_remove  |
                     |  fix_shortage(i)       | fix_shortage(i+1) [...] |
               ------|------------------------|-------------------------|-------


             */
        T found_entry;
        T replace;
        int j=0;
        int i = first_ge(data, data_count, entry);
        bool found= (i<data_count&&data[i]==entry);
        bool debug= false;
        if(debug){
            cout<<"loose remove called "<<endl;
        }
        if(is_leaf()){                                          //leaf
            if(found){                                          //leaf, found
                if(debug){
                    cout<<"found && is leaf"<<endl;
                }
                delete_item(data,i,data_count,found_entry);
            }else{                                              //leaf, !found
                if(debug){
                    cout<<"!found && is leaf"<<endl;
                }
                return;
            }

        }else{                                                  //not leaf, found
            if(found){
                if(debug){
                    cout<<"found && is !leaf"<<endl;
                }
                assert(i < child_count-1);
                //[d.] found key in an inner node:subset[i+1]->loose_remove

                /*
                 * Here's The Story:
                 * data[i] is the same as the item that we have deleted.
                 * so, now, it needs to be replaced by the current smallest key
                 *      in subset[i+1]
                 * Problem: After fix_shortage, data[i] could have moved to a different index(?)
                 *      or it could have sunk down to a lower level as a result of merge
                 *      we need to find this item and replace it.
                 *
                 *      Before fix_shortage: we cannot replace data[i] with subset[i+1]->smallest
                 *      before fix_excess because this smallest key could be the very entry
                 *      we are removing. So, we'd be replacing data[i] with entry. and this will
                 *      leave the deleted key in the inner nodes.
                 *
                 *      After fix_shortage: We have no way of knowing where data[i] key (which is
                 *      same as entry) will end up. It could move down one level by the time
                 *      fix_shortage returns.
                 *
                 *      Therefore it seems like we have no choice but to search for it in
                 *      data[ ] AND subset[i]->data[ ]
                 * Current Solution: Kind of messy:
                 *      After fix_shortage(i+1):
                 *      Search for entry in data[ ] and if found, replace it with smallest.
                 *      otherwise, search for it in subset[i]->data[ ]
                 *          and replace it. that's where the key (entry) will end up after
                 *          fix order returns (credit: Jiaruy Li)
                 *
                 */

                replace= find_next(entry);
                data[i] = replace;
                subset[i + 1]->loose_remove(entry);
                if(subset[ i + 1 ]->data_count<MINIMUM){
                    if(debug){
                        cout<<"subset[ i + 1 ]->data_count<MINIMUM...data_count:"<<subset[i+1]->data_count<<endl;
                    }
                    fix_shortage(i + 1);

                    if(debug){
                        cout<<"end of fix in loose remove"<<endl;
                    }
                }//story problem

                //remember. it's possible that data[i] will be gone by the time
                //      fix_shortage returns.
                //key was removed from subset[i+1]:
                //  1. shortage: find entry in data or subset[i+1]->data
                //              and replace it with subset[i+1]->smallest
                //  2. no shortage: replace data[i] with subset[i+1]->smallest
            }else{                                              //not leaf, !found yet
                if(debug){
                    cout<<"!found && !is_leaf..... recursive loose remove"<<endl;
                }

                subset[i]->loose_remove(entry);
                if(subset[i]->data_count<MINIMUM){
                    fix_shortage(i);

                }
            }
        }

    }
    T find_next(const T& entry){


        int i=first_ge(data, data_count, entry);
        T result;
        BPlusTree<T>* temp  = subset[i+1]->get_smallest_node();
        result= data[0];
        if( temp -> data_count > MINIMUM){
            result = temp->data[1];
        }else if(temp->next!= nullptr){
            result = temp->next->data[0];
        }
        return result;

    }

    BPlusTree<T>* fix_shortage(int i){//fix shortage of data elements in child i
        // and return the smallest key in this subtree
        /*
             * fix shortage in subtree i:
             * if child i+1 has more than MINIMUM,
             *          (borrow from right) transfer / rotate left(i+1)
             * elif child i-1 has more than MINIMUM,
             *          (borrow from left) transfer /rotate right(i-1)
             * elif there is a left sibling,
             *          merge with prev child: merge(i-1)
             * else
             *          merge with next (right) child: merge(i)
             *
             *
             * returns a pointer to the fixed_subset
             */
        bool debug= false;
        if(debug){
            cout<<"fix shortage called i:   "<<i<<endl;
        }

        if(child_count>i+1 && subset[i+1]->data_count> MINIMUM){            //i has to have right subset, so i<child_count-1
            if(debug){                                                      //if child i+1 has more than MINIMUM,
                cout<<"transfering left at (i+1)"<<i<<endl;                     //(borrow from right) transfer / rotate left(i+1)

            }

            transfer_left(i+1);

        }else if(i-1>=0 && i<child_count && subset[i-1]->data_count> MINIMUM ){//i has to have left subset, so can't be 0 and less then child_count
            if(debug){                                                      //child i-1 has more than MINIMUM,
                //(borrow from left) transfer /rotate right(i-1)

                cout<<"transfering right at (i-1)"<<i<<endl;                     //(borrow from right) transfer / rotate left(i+1)
            }



            transfer_right(i-1);

        }else if(i-1>=0){                                                      //there has to be a subset[i-1], so i>0

            if(debug){

                cout<<"merging (i-1) with"<<*(subset[i-1]->data)<<endl;
            }
            merge_with_next_subset(i-1);
        }else{
            if(debug){
                cout<<"merging (i) with"<<*(subset[i]->data)<<endl;
            }
            merge_with_next_subset(i);
        }

        return subset[i];
    }

    BPlusTree<T>* get_smallest_node(){

        if(!is_leaf()){

            return subset[0]->get_smallest_node();
        }else{

            return this;
        }

        //check
    }
    const BPlusTree<T>* get_smallest_node()const {

        if(!is_leaf()){

            return subset[0]->get_smallest_node();
        }else{

            return  this;
        }

        //check
    }

    void get_smallest(T& entry){

        //entry := leftmost leaf
        //replace entry with the left_most leaf of this tree
        // Keep looking in the first subtree (recursive)
        //  until you get to a leaf.
        // Then, replace entry with data[0] of the leaf node

        if(!is_leaf()){
            subset[0]->get_smallest(entry);

        }else{
            entry= data[0];
        }
        //check

    }
    void get_biggest(T& entry){        //entry := rightmost leaf //remove the biggest dick in the world
        if(!is_leaf()){
            subset[child_count-1]->get_biggest(entry);

        }else{
            entry= data[data_count-1];
        }
        //check

    }
    void transfer_left(int i){          //transfer one element LEFT from child i

        bool debug= false;

        if(debug){
            cout<<"enter transfer left"<<endl;
        }


        if(!(subset[i]->is_leaf())){
            if(debug){
                cout<<"the subset is not a leaf , rotating left"<<endl;
            }
            T temp;
                   delete_item(subset[i]->data, 0,subset[i]->data_count, temp);
                   swap(temp,data[i-1]);
                   attach_item(subset[i-1]->data, subset[i-1]->data_count,temp);
                   BPlusTree<T>* b;
                   if(subset[i]->child_count >0){
                       delete_item(subset[i]->subset, 0,subset[i]->child_count, b);
                       attach_item(subset[i-1]->subset, subset[i-1]->child_count, b);
                   }
        } else{
            if(debug){
                cout<<"the subset is a leaf , do the work"<<endl;
            }

            T entry;
            delete_item(subset[i]->data,0,subset[i]->data_count,entry);// taking the deleted stuff into entry
            attach_item(subset[i-1]->data,subset[i-1]->data_count,entry);//and place it into the end of data.


            data[i-1]= subset[i]->data[0];


        }
    }

    void transfer_right(int i){         //transfer one element RIGHT from child i

        bool debug= false;
        if(debug){
            cout<<"enter transfer right"<<endl;
        }
        if(!subset[i]->is_leaf()){
            if(debug){
                cout<<"the subset is not a leaf , rotating right"<<endl;
            }

            T temp;
                    detach_item(subset[i]->data,subset[i]->data_count,temp);
                    swap(temp,data[i]);
                    insert_item(subset[i+1]->data,0, subset[i+1]->data_count,temp);
                    BPlusTree<T>* b;
                    if(subset[i]->child_count >0){
                        detach_item(subset[i]->subset, subset[i]->child_count, b);
                        insert_item(subset[i+1]->subset,0, subset[i+1]->child_count,b);
                    }
        }else{
            if(debug){
                cout<<"the subset is a leaf , do the work"<<endl;
            }

            T entry;

            detach_item(subset[i]->data,subset[i]->data_count,entry);        //taking the deleted stuff at the end and place it into the entry
            data[i]=entry;
            insert_item(subset[i+1]->data,0, subset[i+1]->data_count,entry);

        }

    }

    BPlusTree<T> *merge_with_next_subset(int i){    //merge subset i with subset i+1
        T temp;
        bool debug= false;

        BPlusTree<T> *tempPtr;
        if(debug){
            cout<<"called merge with next subset "<<endl;
        }

            //*  Merge subset[i] with subset [i+1] with data[i] in the middle
            //1. remove data[i] from this object
            delete_item(data, i, data_count, temp);

            //2. append it to child[i]->data
            if(!(subset[i]->is_leaf())){

                attach_item(subset[i]->data, subset[i]->data_count, temp);
            }
            //3. Move all data items from subset[i+1]->data to subset[i]->data
            merge(subset[i]->data, subset[i]->data_count, subset[i + 1]->data, subset[i + 1]->data_count);

            //4. Move all subset pointers from subset[i+1]->subset to subset[i]->subset
            merge(subset[i]->subset, subset[i]->child_count, subset[i + 1]->subset, subset[i + 1]->child_count);

            subset[i]->next = subset[i+1]->next;

            //5. delete subset[i+1] (store in a temp ptr)
            delete_item(subset, i + 1, child_count, tempPtr);

            //6. delete temp ptr
            tempPtr->child_count = 0;

            delete tempPtr;


            return subset[i];


    }

};


#endif // BPLUSTREE_H
