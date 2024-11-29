#ifndef LINKED_LIST_FUNCTIONS_TEMPLATED_H
#define LINKED_LIST_FUNCTIONS_TEMPLATED_H

#include "../node/node.h"

/**
 *   Prints the contents of each node pointer of a linked list
 *
 *   @param head - The head of the linked list to be printed
*/
template <typename T> void _print_list(node<T>* head) {
    if (head == nullptr || head->_prev == nullptr)
        cout << "Head->";

    if (head != nullptr) cout << *head << " ";

    if (head == nullptr || head->_next == nullptr) {
        cout << "|||" << endl;
        return;
    }

    _print_list<T>(head->_next);
}

/**
 *    Prints the contents of each node pointer of a linked list starting
 *    from the last element in the linked list.
 *   
 *    Uses node->_prev to traverse
 *    backwards, so this is a great method for debugging _prev node
 *    insertion.
 *
 *    @param head - The head of the linked list to be printed
*/
template <typename T> void _print_list_backwards(node<T>* head) {
    const bool debug = false;
    // |||<-Head or ||| <-[10]-><-Head

    if (debug) {
        node<T>* last_node = nullptr;
        for (node<T>* _next = head; _next != nullptr; _next = _next->_next) {
            last_node = _next;
        }

        cout << "|||";
        for (node<T>* _prev = last_node; _prev != nullptr; _prev = _prev->_prev)
            cout << " " << * _prev;
        cout << "<-Head" << endl;
    }
    else {
        if (head == nullptr || head->_next == nullptr)
            cout << "|||";

        if (head != nullptr) {
            if (head->_next != nullptr) _print_list_backwards<T>(head->_next);
            cout << " <-" << *head;
            if (head->_prev == nullptr)
                cout << "<-Head" << endl;
            return;
        }

        cout << "<-Head" << endl;
    }
}

/**
 *    Searches a linked list starting from #param head for a node pointer
 *    with a field _item that is equal to #param key.
 *
 *    @param head - The head of the linked list to be searched
 *    @param key - The value being searched for within each nodes' contents
 *    @returns The address of the node who's field _item == key; nullptr otherwise
*/
template <typename T> node<T>* _search_list(node<T>* head, T key) {
    if (head == nullptr) return nullptr;
    if (head->_item == key) return head;
    return _search_list<T>(head->_next, key);
}

/**
 * 
 *    Inserts #param insert_this--as a new node in the heap--as the new head to 
 *    the linked list #param head.
 *
 *    Head MUST be the actual head of a linked list, ie head->_prev must equal nullptr.
 *
 *    @param head - The head of the linked list to be updated to the new node
 *    @param insert_this - The value to be passed to the new node
 *    @returns The address to the new node in the heap that was inserted
*/
template <typename T> node<T>* _insert_head(node<T>*& head, T insert_this) {
    bool debug = false;

    // Create a node with _item = insert_this, _next = head, _prev = NULL
    if (debug) cout << endl << "Inserting " << insert_this << endl;
    node<T>* _node = new node<T>(insert_this, head);

    if (head != nullptr)
        head->_prev = _node;

    head = _node;
    return head;
}

/**
* 
*    Inserts #param insert_this--as a new node in the heap--in the linked list starting with
*    the node pointer #param head after the node #param after_this. Calls _insert_head if #param head 
*    or #param after_this is nullptr
*
*    @param head - The head of the linked list
*    @param after_this - The node address that the new node is inserted after
*    @param insert_this - The value to be passed to the new node
*    @returns The address of the new node inserted; nullptr if after_this is not found in head linked list
*/
template <typename T> node<T>* _insert_after(node<T>*& head, node<T>* after_this, T insert_this) {
    
    if (head == nullptr || after_this == nullptr) return _insert_head<T>(head, insert_this);

    // loop to check after_this is part of head linked list
    for (node<T>* _n = head; _n != after_this; _n = _n->_next)
        if (_n == nullptr) return nullptr;

    node<T>* _node = new node<T>(insert_this, after_this->_next, after_this);

    if (after_this->_next != nullptr) after_this->_next->_prev = _node;
    after_this->_next = _node;

    return _node;
}

//insert before ptr: insert head if marker null

/**
*    Inserts #param insert_this--as a new node in the heap--in the linked list starting with
*    the node pointer #param head before the node #param before_this. Calls _insert_head if #param head
*    or #param before_this is nullptr
*
*    @param head - The head of the linked list
*    @param before_this - The node address that the new node is inserted before
*    @param insert_this - The value to be passed to the new node
*    @returns The address of the new node inserted; nullptr if before_this is not found in head linked list
*/
template <typename T> node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this) {
    
    if (head == nullptr || before_this == nullptr) return _insert_head<T>(head, insert_this);

    // loop to check after_this is part of head linked list
    for (node<T>* _n = head; _n != before_this; _n = _n->_next)
        if (_n == nullptr) return nullptr;

    node<T>* _node = new node<T>(insert_this, before_this, before_this->_prev);

    if (before_this->_prev != nullptr) before_this->_prev->_next = _node;
    before_this->_prev = _node;

    if (head == before_this) head = _node;

    return _node;
    
    //bool debug = false;
    //if (head == nullptr || before_this == nullptr) return _insert_head(head, insert_this);

    //if (debug) cout << "_insert_before() : Pointing to " << *head << endl;
    //if (head == before_this) {
    //    if (debug) cout << "_insert_before() : Inserting " << insert_this << endl;
    //    return _insert_head(head, insert_this);
    //}
    //return _insert_before(head->_next, before_this, insert_this);
}

//ptr to previous node
/**
* 
* @tparam T - The type of value stored by node
* @tparam prev_to_this - The node address used to return the address to the node before it in the linked list
* 
* @returns The address to the previous node of #param prev_to_this; nullptr if #param prev_to_this is nullptr
*/
template <typename T> node<T>* _previous_node(node<T>* prev_to_this) {
    if (prev_to_this == nullptr) return nullptr;
    return prev_to_this->_prev;
}

//delete, return item
/**
* 
* Removes the address #param delete_this from the linked list that begins with the value held in #param head.
*
* @tparam T - The type of value stored by node
* @tparam prev_to_this - The node address used to return the address to the node before it in the linked list
*
* @returns The address to the previous node of #param prev_to_this; nullptr if #param prev_to_this is nullptr
*/
template <typename T> T _delete_node(node<T>*& head, node<T>* delete_this) {
    const bool debug = false;

    bool in_linked_list = false;
    // loop to check delete_this is part of head linked list
    for (node<T>* _n = head; _n != nullptr;  _n = _n->_next) {
        if (_n == delete_this) {
            in_linked_list = true;
            break;
        }
    }

    assert(in_linked_list);

    if (debug) {
        cout << "_delete_node() : " << endl
            << "head ----" << endl
            << "\t" << (head->_prev == nullptr ? T() : *head->_prev) << " " << *head << " " << (head->_next == nullptr ? T() : *head->_next) << endl
            << "\t" << head->_prev << " " << head << " " << head->_next << endl
            << "delete_this ----" << endl
            << "\t" << (delete_this->_prev == nullptr ? T() : *delete_this->_prev) << " " << *delete_this << " " << (delete_this->_next == nullptr ? T() : *delete_this->_next) << endl
            << "\t" << delete_this->_prev << " " << delete_this << " " << delete_this->_next << endl;
    }

    T _return_item = delete_this->_item;
    if (delete_this->_prev != nullptr) delete_this->_prev->_next = delete_this->_next;
    if (delete_this->_next != nullptr) delete_this->_next->_prev = delete_this->_prev;

    if (head == delete_this)
        head = delete_this->_next;

    delete_this->_prev = nullptr;
    delete_this->_next = nullptr;

    delete delete_this;
    delete_this = nullptr; // probably unnecessary but not sure

    if (debug) {
        cout << "_delete_node() : Deleted delete_this... " << endl
            << "head ----" << endl
            << "\t" << (head == nullptr ? T() : head->_prev == nullptr ? T() : *head->_prev) << " " << (head == nullptr ? T() : *head) << " " << (head == nullptr ? T() : head->_next == nullptr ? T() : *head->_next) << endl
            << "\t" << (head == nullptr ? head : head->_prev) << " " << head << " " << (head == nullptr ? head : head->_next) << endl
            <<"delete_this ----" << endl
            << "\t" << (delete_this == nullptr ? T() : delete_this->_prev == nullptr ? T() : *delete_this->_prev) << " " << (delete_this == nullptr ? T() : *delete_this) << " " << (delete_this == nullptr ? T() : delete_this->_next == nullptr ? T() : *delete_this->_next) << endl
            << "\t" << (delete_this == nullptr ? delete_this : delete_this->_prev) << " " << delete_this << " " << (delete_this == nullptr ? delete_this : delete_this->_next) << endl;

    }
    return _return_item;
}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head) {
    const bool debug = false;
    if (head == nullptr) return nullptr;

    node<T>* _copy = nullptr;
    node<T>* _copy_head = nullptr;
    for (node<T>* _next = head; _next != nullptr; _next = _next->_next) {
        if (debug) cout << "_copy_list() : Copying " << *_next << endl;

        _insert_after<T>(_copy, _copy, _next->_item);
        if (_copy->_prev == nullptr) _copy_head = _copy; // Once _copy is initialize with first node, set _copy_head to address of new node
        if (_copy->_next != nullptr) _copy = _copy->_next; // Make _copy point to _next to _insert_after that node

        if (debug) cout << "_copy pointing to : " << *_copy << endl;
        if (debug) {
            cout << "_copy_list() : New copy list..." << endl;
            _print_list<T>(_copy_head);
        }
    }
    return _copy_head;
}

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T> node<T>* _copy_list(node<T>*& dest, node<T>* src) {
    if (dest != nullptr || src == nullptr) return nullptr;

    node<T>* last_node = nullptr;
    for (node<T>* _next = src; _next != nullptr; _next = _next->_next)
        last_node = _insert_after<T>(dest, last_node, _next->_item);

    return last_node;
}

//delete all the nodes
template <typename T> void _clear_list(node<T>*& head) {
    if (head == nullptr) return;
    _delete_node<T>(head, head);
    _clear_list<T>(head);
}

//_item at this position
template <typename T> T& _at(node<T>* head, int pos) {
    assert(head != nullptr);
    assert(pos >= 0); // Assert pos isn't negative
    if (pos == 0) return head->_item;
    return _at<T>(head->_next, pos - 1);
}

#endif