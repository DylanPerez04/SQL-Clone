#ifndef MY_STACK_H
#define MY_STACK_H

#include "../linked_list_functions/linkedlistfunctionsTemplated.h"

template <typename ITEM_TYPE> class Stack {
private:
    node<ITEM_TYPE>* _top;
    int _size;
public:
    class Iterator {
    private:
        node<ITEM_TYPE>* _ptr;    //pointer being encapsulated
    public:
        friend class Stack;                 //give access to list to access _ptr
        Iterator() { _ptr = nullptr; }            //default ctor
        Iterator(node<ITEM_TYPE>* p) {
            _ptr = p;
        }      //Point Iterator to where
        //  p is pointing to
        ITEM_TYPE operator *() {
            return _ptr->_item;
        }    //dereference operator
        bool is_null() { return _ptr == nullptr; }            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left, const Iterator& right) { //true if left != right
            return left._ptr != right._ptr;
        }

        friend bool operator ==(const Iterator& left, const Iterator& right) { //true if left == right
            return left._ptr == right._ptr;
        }

        Iterator& operator++() {                        //member operator:++it
            // or ++it = new_value
            _ptr = _ptr->_next;
            return *this;
        }

        friend Iterator operator++(Iterator& it, int dont_use) { //friend operator: it++
            assert(it._ptr != nullptr);
            ++it;
            return it;
        }
    };

    Stack() : _top(nullptr), _size(0) {}
    Stack(const Stack<ITEM_TYPE>& copyMe) : Stack() {
        _copy_list<ITEM_TYPE>(_top, copyMe._top);
        _size = copyMe._size;
    }
    ~Stack() {
        _clear_list<ITEM_TYPE>(_top);
    }
    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS) {
        if (this->_top == RHS._top) return *this;

        _clear_list<ITEM_TYPE>(_top);
        _copy_list<ITEM_TYPE>(_top, RHS._top);
        _size = RHS._size;

        return *this;
    }
    ITEM_TYPE top() {
        return _top->_item;
    }
    bool empty() {
        return _size == 0;
    }
    void push(ITEM_TYPE item) {
        const bool debug = false;
        _insert_head<ITEM_TYPE>(_top, item);
        ++_size;

        if (debug) {
            cout << "pushed " << item << "... _size = " << _size << endl;
            _print_list(_top);
            _print_list_backwards(_top);

            int index = 0;
            for (node<ITEM_TYPE>* n = _top; n != nullptr; n = n->_next)
                cout << index++ << ". addresses " << endl << "\t" << n->_prev << " " << n << " " << n->_next
                << endl << "   values " << endl << "\t" << (n->_prev == nullptr ? node<ITEM_TYPE>() : *n->_prev)
                << *n << (n->_next == nullptr ? node<ITEM_TYPE>() : *n->_next) << endl;

            cout << endl << "-------------" << endl << endl;
        }
    }

    ITEM_TYPE pop() {
        const bool debug = false;
        // _delete_node will throw exception if _top == nullptr (size == 0)
        ITEM_TYPE item = _delete_node<ITEM_TYPE>(_top, _top);
        --_size; 

        if (debug) {
            cout << "popped " << item << "... _size = " << _size << endl;
            _print_list(_top);
            _print_list_backwards(_top);

            int index = 0;
            for (node<ITEM_TYPE>* n = _top; n != nullptr; n = n->_next)
                cout << index++ << ". addresses " << endl << "\t" << n->_prev << " " << n << " " << n->_next
                << endl << "   values " << endl << "\t" << (n->_prev == nullptr ? node<ITEM_TYPE>() : *n->_prev)
                << *n << (n->_next == nullptr ? node<ITEM_TYPE>() : *n->_next) << endl;

            cout << endl << "-------------" << endl << endl;
        }
        return item;
    }
    friend ostream& operator << (ostream& outs, const Stack<ITEM_TYPE>& printMe) {
        outs << "Stack:";
        _print_list<ITEM_TYPE>(printMe._top);
        return outs;
    }
    Iterator begin() const {
        return Stack<ITEM_TYPE>::Iterator(_top);
    }                   //Iterator to the head node
    Iterator end() const {
        return Stack<ITEM_TYPE>::Iterator(nullptr);
    }                     //Iterator to NULL
    int size() const { return _size; }
};

#endif