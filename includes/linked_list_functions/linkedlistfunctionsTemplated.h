/** @file linkedlistfunctionsTemplated.h
* 
* Defines linked list helper methods utilized exclusively by the Stack and Queue objects.
*/

#ifndef LINKED_LIST_FUNCTIONS_TEMPLATED_H
#define LINKED_LIST_FUNCTIONS_TEMPLATED_H

#include "../node/node.h"

/**
* Sequentially prints out the `_item` of each node pointer within a 
* linked list of nodes.
*
* @tparam T The datatype of the value stored in node
* @param head The head of the linked list to be printed
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
* Sequentially prints the `_item` of each node pointer within a linked list of nodes 
* from the last element to the first element.
*
* @tparam T The datatype of the value stored in node
* @param head The head of the linked list to be printed
*/
template <typename T> void _print_list_backwards(node<T>* head) {
    const bool debug = false;
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
* Sequentially searches a linked list, beginning at @p head, for 
* a node with an `_item` field equivalent to @p key .
*
* @tparam T The datatype of the value stored in node
* @param head The head of the linked list to be searched
* @param key The value of the `_item` field of the node being searched for
* @returns The address of the node whose `_item` field is equal to key; nullptr otherwise
*/
template <typename T> node<T>* _search_list(node<T>* head, T key) {
    if (head == nullptr) return nullptr;
    if (head->_item == key) return head;
    return _search_list<T>(head->_next, key);
}

/**
* Inserts a new node, with `_item` equal to @p insert_this, as the head of the linked list 
* that begins with @p head. The former head of the mutated linked list becomes the second node. 
*
* @tparam T The datatype of the value stored in node
* @param[in, out] head The head of the linked list to be mutated
* @param[in] insert_this The item to be passed to the new node
* @returns The address of the new node inserted
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
* Inserts a new node, with `_item` equal to @p insert_this, into the linked list that
* begins with @p head, immediately after the @p after_this. 
* 
* If either @p head or @p after_this is `nullptr`, then _insert_head() is simply called,
* passing @p head and @p insert_this.
* 
* @see _insert_head()
*
* @tparam T The datatype of the value stored in node
* @param[in, out] head The head of the linked list to be mutated
* @param[in] after_this A pointer to the node that the new node will be inserted after
* @param[in] insert_this The item to be passed to the new node
* @returns The address of the new node inserted; nullptr if after_this is not found in head linked list
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

/// insert before ptr: insert head if marker null
/**
* Inserts a new node, with `_item` equal to @p insert_this, into the linked list that
* begins with @p head, immediately before @p before_this.
*
* If either @p head or @p before_this is `nullptr`, then _insert_head() is simply called,
* passing @p head and @p insert_this.
*
* @see _insert_head()
*
* @tparam T The datatype of the value stored in node
* @param[in, out] head The head of the linked list to be mutated
* @param[in] before_this A pointer to the node that the new node will be inserted before
* @param[in] insert_this The item to be passed to the new node
* @returns The address of the new node inserted; nullptr if before_this is not found in head linked list
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
}

/// ptr to previous node
/**
* Retrieves a pointer to the node immediately before @p prev_to_this
* (`prev_to_this->_prev` is often simpler to use as node is doubly-linked and a struct with no private members)
* 
* @tparam T The datatype of the value stored in node
* @param prev_to_this A pointer to the node whose `_prev` field is being retrieved.
* @returns The address of the node immediately before @p prev_to_this; nullptr if @p prev_to_this is nullptr
*/
template <typename T> node<T>* _previous_node(node<T>* prev_to_this) {
    if (prev_to_this == nullptr) return nullptr;
    return prev_to_this->_prev;
}

/// delete, return item
/**
* Removes @p delete_this from the linked list that begins with @p head . 
*
* @tparam T The datatype of the value stored in node
* @param[in, out] head The head of the linked list to be mutated
* @param[in] delete_this A pointer to the node being removed from the linked list
* @returns The value of `_item` held by the removed node
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

/// duplicate the list
/**
* Creates a deep copy of the linked list that begins with @p head.
* 
* @tparam T The datatype of the value stored in node
* @param head The head of the linked list being copied
* @returns A pointer to the head node of a deep-copy linked list of @p head
*/
template <typename T> node<T>* _copy_list(node<T>* head) {
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

/// duplicates the list, returns pointer to last node in dest...
/**
* Creates a deep copy of the linked list that begins with @p src and sets @p dest equal to the head node
* of the copy.
*
* @tparam T The datatype of the value stored in node
* @param[in, out] dest The pointer reference to set equal to the address of the head node of the copy
* @param[in] src A pointer to the head of the linked list to copy
* @returns A pointer to the last node of the deep copy; nullptr if @p dest is not equal to `nullptr`, or if @p src is equal to `nullptr`
*/
template <typename T> node<T>* _copy_list(node<T>*& dest, node<T>* src) {
    if (dest != nullptr || src == nullptr) return nullptr;

    node<T>* last_node = nullptr;
    for (node<T>* _next = src; _next != nullptr; _next = _next->_next)
        last_node = _insert_after<T>(dest, last_node, _next->_item);

    return last_node;
}

/// delete all the nodes
/**
* Clears all nodes within the linked list that begins with @p head.
* 
* @tparam T The datatype of the value stored in node
* @param[in, out] head The head of the linked list to be cleared
*/
template <typename T> void _clear_list(node<T>*& head) {
    if (head == nullptr) return;
    _delete_node<T>(head, head);
    _clear_list<T>(head);
}

/// _item at this position
/**
* Retrieves a reference to the value held by the node within the linked list that begins with @p head
* at index @p pos within the linked list.
* 
* @tparam T The datatype of the value stored in node
* @param head The head of the linked list being searched
* @param pos The index of the desired node within the linked list
* @returns A reference to the `_item` field of the node at index @p pos
*/
template <typename T> T& _at(node<T>* head, int pos) {
    assert(head != nullptr);
    assert(pos >= 0); // Assert pos isn't negative
    if (pos == 0) return head->_item;
    return _at<T>(head->_next, pos - 1);
}

#endif