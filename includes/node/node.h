/**
* @file node.h
* 
* Defines the node object utilized exlusively by Stack and Queue
*/

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <cassert>

using namespace std;

/**
* Doubly-linked node object exclusively utilized by Stack and Queue
* 
* @tparam T - The datatype of the value stored in the node
*/
template <class T> struct node {
    T _item; ///< The value stored by the node
    node<T>* _next; ///< A pointer to the next node in the linked list
    node<T>* _prev; ///< A pointer to the previous node in the linked list

    /**
    * Constructs a new node object that holds @p item as the stored value, setting @p next as 
    * the next node and @p prev as the previous node.
    * 
    * @param item The value for the node to store
    * @param next The next node in the linked list
    * @param prev The previous node in the linked list
    */
    node(const T& item = T(), node<T>* next = NULL, node<T>* prev = NULL) :
        _item(item),
        _next(next),
        _prev(prev) {}

    /**
    * Outputs the contents of the linked list beginning with @p print_me into @p outs.
    * 
    * @param[in, out] outs The ostream object to insert the output
    * @param[in] print_me The head of the linked list to output
    * @returns An ostream object with the formatted values of the entire linked list inserted
    */
    friend ostream& operator << (ostream& outs, const node<T>& print_me) {
        outs << "<-[" << print_me._item << "]->";
        return outs;
    }
};

#endif