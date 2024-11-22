#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include "../linked_list_functions/linkedlistfunctionsTemplated.h"

template <typename T> class Queue {
private:
    node<T>* _front;
    node<T>* _rear;
    int _size;
public:
    class Iterator {
    private:
        node<T>* _ptr;                          //pointer being encapsulated
    public:
        friend class Queue;                               //give access to list to access _ptr
        Iterator() { _ptr = NULL; }                          //default ctor
        Iterator(node<T>* p) {
            _ptr = p;
        }                            //Point Iterator to where
        //  p is pointing to
        T operator *() {
            return _ptr->_item;
        }    //dereference operator
        bool is_null() {
            return _ptr == nullptr;
        }            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left, const Iterator& right) { //true if left != right
            return left._ptr != right._ptr;
        }

        friend bool operator ==(const Iterator& left, const Iterator& right) { //true if left == right
            return left._ptr == right._ptr;
        }

        Iterator& operator++() {                         //member operator:++it
            // or ++it = new_value
            _ptr = _ptr->_next;
            return *this;
        }

        friend Iterator operator++(Iterator& it, int dont_use) {         //friend operator: it++
            assert(it != NULL && it._ptr != nullptr);
            ++it;
            return it;
        }
    };

    Queue() : _front(nullptr), _rear(nullptr), _size(0) {}

    Queue(const Queue<T>& copyMe) : Queue() {
        _rear = _copy_list<T>(_front, copyMe._front);
        _size = copyMe._size;
    }
    ~Queue() {
        _clear_list<T>(_front);
        _rear = nullptr;
    }
    Queue& operator=(const Queue<T>& RHS) {
        if (this->_front == RHS._front) return *this;

        _clear_list<T>(_front);
        _rear = _copy_list<T>(_front, RHS._front);
        _size = RHS._size;

        return *this;
    }

    bool empty() {
        const bool debug = false;
        if (debug) {
            cout << "MyQueue::empty() : _front = " << (_front == nullptr ? T()  : _front->_item) << endl;
            cout << "MyQueue::empty() : _rear = " << (_rear == nullptr ? T() : _rear->_item) << endl;
        }
        return _size == 0;
    }
    T front() {
        return _front->_item;
    }
    T back() {
        return _rear->_item;
    }

    void push(T item) {
        const bool debug = false;
        _rear = _insert_after<T>(_front, _rear, item);
        ++_size;
        if (debug) {
            cout << "MyQueue::push() : _front = " << (_front == nullptr ? T() : _front->_item)
                << " _rear = " << (_rear == nullptr ? T() : _rear->_item) << " _size = " << size() << endl;
        }
    }
    T pop() {
        T val = _delete_node<T>(_front, _front);
        if (_front == nullptr) _rear = nullptr;
        --_size;
        return val;
    }

    void clear() {
        while (!empty())
            this->pop();
    }

    Iterator begin() const {
        return static_cast<Iterator>(_front);
    }                                     //Iterator to the head node
    Iterator end() const {
        return nullptr;
    }                                       //Iterator to NULL
    void print_pointers() {
        cout << "Queue: Head->";
        Iterator it = begin();
        while (it != end()) {
            cout << "<-[" << (*it)->token_str() << "]-> ";
            it++;
        }
        cout << "|||" << endl;
    }
    int size() const { return _size; }
    friend ostream& operator << (ostream& outs, const Queue<T>& printMe) {
        outs << "Queue:";
        _print_list<T>(printMe._front);
        return outs;
    }
};

#endif