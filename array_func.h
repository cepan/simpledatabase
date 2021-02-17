#ifndef ARRAY_FUNC_H
#define ARRAY_FUNC_H


#include <iostream>
#include <cmath>
using namespace std;

template <class T>
int first_ge(const T data[ ], int n, const T& entry);
template <class T>
void insert_item(T data[ ], int i, int& n, T entry);

template <class T>
T maximal(const T& a, const T& b){
    return (a>b)? a:b;
}                      //return the larger of the two items

template <class T>
int index_of_maximal(T data[ ], int n){
    T temp = data[0];
    int index = 0;

    for(int i = 0; i < n ; i++){
        if(data[i] > temp){
            temp = data[i];
            index = i;
        }
    }
    return index;
}                 //return index of the largest item in data

template <class T>
void ordered_insert(T data[ ], int& n, T entry){

    int i = first_ge(data,n,entry);
    insert_item(data,i,n, entry);

}        //insert entry into the sorted array
//data with length n

template <class T>
int first_ge(const T data[ ], int n, const T& entry){

    int i = 0;
    for( ; i< n; i ++){
        if(data[i] >= entry){
            break;
        }
    }
    return i;

}   //return the first element in data that is
//not less than entry

template <class T>
void attach_item(T data[ ], int& n, const T& entry){
    data[n++] = entry;
}    //append entry to the right of data

template <class T>
void insert_item(T data[ ], int i, int& n, T entry){
    T temp = T();

    for(int j = i ; j <= n ;  j++){
        swap(data[j],temp);
    }
    n++;
    data[i] = entry;

}    //insert entry at index i in data

template <class T>
void detach_item(T data[ ], int& n, T& entry){
    if(n >= 1){
        entry = data[n-1];
        data[n-1] = T();
        n--;
    }

}          //remove the last element in data and place
//it in entry

template <class T>
void delete_item(T data[ ], int i, int& n, T& entry){

    if(n >= 1){
        entry = data[i];
        for(int j = i ; j < n ;  j ++){
            data[j] = data[j+1];
        }
        n --;
    }

}   //delete item at index i and place it in entry

template <class T>
void merge(T data1[ ], int& n1, T data2[ ], int& n2){
    for(int i = 0; i < n2; i ++){
        data1[n1] = data2[i];
        data2[i] =  T();
        n1++;
    }

    n2 = 0;
}   //append data2 to the right of data1

template <class T>
void split(T data1[ ], int& n1, T data2[ ], int& n2){
    int size = ceil(n1/2.0);
    for(int i = size ; i < n1; i ++){
        data2[n2] = data1[i];
        data1[i] = T();
        n2++;
    }
    n1 = size;

}   //move n/2 elements from the right of data1
//and move to data2

template <class T>
void copy_array(T dest[], const T src[],
                int& dest_size, int src_size){
    dest_size = 0;
    for(int i = 0; i < src_size ; i ++){
        dest[i] = src[i];
        dest_size = i+1;
    }
}              //copy src[] into dest[]

template <class T>
void print_array(const T data[], int n, int pos = 0){ // pos =  -1 for some flag
    for(int pos = 0 ; pos < n ; pos ++){
        cout<<data[pos]<<" ";
    }
}  //print array data

template <class T>
void clearArr( T data[], int& n){
    for(int pos = 0 ; pos < n ; pos ++){
        data[pos] = T();
    }
    n = 0;
}  //print array da

template <class T>
bool is_gt(const T data[], int n, const T& item){
    return item > data[n-1];
}       //item > all data[i]

template <class T>
bool is_le(const T data[], int n, const T& item){
    return item <= data[n-1];
}       //item <= all data[i]






//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list){
    for (int i=0; i<list.size(); i++){
        cout<<list[i]<<" ";
    }
    return outs;
} //print vector list

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme){

    list.push_back(addme);



    return list;


} //list.push_back addme



#endif // ARRAY_FUNC_H
