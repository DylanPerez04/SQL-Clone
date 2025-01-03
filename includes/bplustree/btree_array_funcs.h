#ifndef BTREE_ARRAY_FUNCS_H
#define BTREE_ARRAY_FUNCS_H

#include <iostream>
#include <cassert>
#include <vector>

using namespace std;

//return the larger of the two items
template <class T> T maximal(const T& a, const T& b) {
    return a >= b ? a : b; // NOTE: >= might cause bugs
}

//swap the two items
template <class T> void swap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

/**
* Return index of the largest item in data
* precondition: #param data must be sorted
*
* @returns index of max value in #param data
*/
template <class T> int index_of_maximal(T data[], int n) {
    return n - 1;
}

/**
* Insert entry into the sorted array data with length n.
* Inserts #param entry regardless of whether #param data already contains the element.
*/
template <class T> void ordered_insert(T data[], int& n, T entry) {
    assert(n >= 0);
    // binary search loop to find index to insert
    int min = 0, max = n - 1, i = 0;
    while (min < max) {
        i = (min + max) / 2;
        if (entry == data[i]) {
            break;
        }
        else if (entry > data[i]) {
            min = i + 1;
        }
        else if (entry < data[i]) {
            max = i - 1;
            if (max == min) i++;
        }
    }
    insert_item(data, i, n, entry);
}

//return the index of the first element in data that is not less than entry
template <class T> int first_ge(const T data[], int n, const T& entry) {
    for (int i = 0; i < n; i++)
        if (data[i] > entry || data[i] == entry) return i;
    return n;
}

//append entry to the right of data
template <class T> void attach_item(T data[], int& n, const T& entry) {
    data[n++] = entry;
}

//insert entry at index i in data
template <class T> void insert_item(T data[], int i, int& n, T entry) {
    assert(n >= 0 && i <= n);

    for (int j = n; j > i; j--)
        data[j] = data[j - 1];

    data[i] = entry;
    n++;
}

//delete item at index i and place it in entry
template <class T> void delete_item(T data[], int i, int& n, T& entry) {
    assert(i >= 0 && i < n);

    entry = data[i];
    for (int x = i; x < n - 1; x++)
        data[x] = data[x + 1];
    n--;

    //if (i >= 0 && i < n) {
    //    entry = data[i];
    //    for (i; i < n; i++)
    //        data[i] = data[i + 1];
    //    n--;
    //}
}

//remove the last element in data and place it in entry
template <class T> void detach_item(T data[], int& n, T& entry) {
    assert(n > 0);
    delete_item(data, n - 1, n, entry);
}


//append data2 to the right of data1
template <class T> void merge(T data1[], int& n1, T data2[], int& n2) {
    assert(n1 >= 0 && n2 >= 0);

    for (int i = n1; i < n1 + n2; i++)
        data1[i] = data2[i - n1];
    n1 += n2;
    n2 = 0;
}

//move n/2 elements from the right of data1 and move to data2
template <class T> void split(T data1[], int& n1, T data2[], int& n2) {
    assert(n1 >= 0);

    T item;
    for (int i = n1 / 2; i > 0; i--) {
        detach_item(data1, n1, item);
        insert_item(data2, 0, n2, item);
    }
}

//copy src[] into dest[]
template <class T> void copy_array(T dest[], const T src[], int& dest_size, int src_size) {
    assert(src_size >= 0);

    for (int i = 0; i < src_size; i++)
        dest[i] = src[i];
    dest_size = src_size;
}

//print array data
template <class T> void print_array(const T data[], int n, int pos = 0) {
    assert(n >= 0);

    for (int i = pos; i < n; i++)
        cout << data[i] << "|";
    cout << endl;
}

//item > all data[i]
template <class T> bool is_gt(const T data[], int n, const T& item) {
    assert(n >= 0);

    for (int i = 0; i < n; i++)
        if (item <= data[i]) return false;
    return true;
}

//item <= all data[i]
template <class T> bool is_le(const T data[], int n, const T& item) {
    return !is_gt(item);
}

//-------------- Vector Extra operators: ---------------------

//print vector list
template <typename T> ostream& operator <<(ostream& outs, const vector<T>& list) {
    for (int i = 0; i < list.size(); i++)
        outs << list.at(i) << " ";
    outs << endl;
    return outs;
}

//list.push_back addme
template <typename T> vector<T>& operator +=(vector<T>& list, const T& addme) {
    list.push_back(addme);
    return list;
}

#endif