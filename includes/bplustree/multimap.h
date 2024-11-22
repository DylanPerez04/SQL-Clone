#ifndef MULTIMAP_H
#define MULTIMAP_H

#include <iostream>
#include "bplustree.h"

using namespace std;

template <typename K, typename V> struct MPair {
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
     //--------------------------------------------------------------------------------
    MPair(const K& k = K()) : key(k) {}
    MPair(const K& k, const V& v) : MPair(k) {
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist) : MPair(k) {
        value_list = vlist;
    }
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me) {
        outs << print_me.key << ": [";
        for (int i = 0; i < print_me.value_list.size(); i++)
            outs << print_me.value_list[i] << " ";
        outs << "]";
        return outs;
    }

    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key == rhs.key;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key < rhs.key;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key == rhs.key || lhs.key < rhs.key;
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key > rhs.key;
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        MPair<K, V> mpair(lhs.key, lhs.value_list);
        for (int i = 0; i < rhs.value_list.size(); i++)
            mpair.value_list += rhs.value_list[i];

        return mpair;
    }
};

template <typename K, typename V> class MMap {
private:
    BPlusTree<MPair<K, V> > mmap{ true };
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator {
    private:
        typename map_base::Iterator _it;
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it = NULL) : _it(it) {}
        // postfix (i++)
        Iterator operator ++(int unused) {
            return _it++;
        }
        // prefix (++i)
        Iterator operator ++() {
            return ++_it;
        }
        MPair<K, V> operator *() {
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }
    };

    MMap() {}

    //  Iterators
    Iterator begin() {
        return MMap<K, V>::Iterator(mmap.begin());
    }
    Iterator end() {
        return MMap<K, V>::Iterator(mmap.end());
    }

    //  Capacity
    int size() const {
        return mmap.size();
    }
    bool empty() const {
        return mmap.empty();
    }

    //  Element Access
    const vector<V>& operator[](const K& key) const {
        return mmap.get(MPair<K, V>(key)).value_list;
    }
    vector<V>& operator[](const K& key) {
        return mmap.get(MPair<K, V>(key)).value_list;
    }
    vector<V>& at(const K& key) {
        return mmap.get_existing(MPair<K, V>(key)).value_list;
    }

    //  Modifiers
    void insert(const K& k, const V& v) {
        mmap.insert(MPair<K, V>(k, v));
    }
    void erase(const K& key) {
        mmap.remove(MPair<K, V>(key));
    }
    void clear() {
        mmap.clear_tree();
    }

    //  Operations:
    bool contains(const K& key) const {
        return mmap.contains(MPair<K, V>(key));
    }
    vector<V>& get(const K& key) {
        return mmap.get(MPair<K, V>(key)).value_list;
    }

    Iterator find(const K& key) {
        return MMap<K, V>::Iterator(mmap.find(MPair<K, V>(key)));
    }

    int count(const K& key) {
        return -1;
    }

    Iterator lower_bound(const K& key) {
        return MMap<K, V>::Iterator(mmap.lower_bound(MPair<K, V>(key)));
    }

    Iterator upper_bound(const K& key) {
        return MMap<K, V>::Iterator(mmap.upper_bound(MPair<K, V>(key)));
    }

    Iterator equal_range(const K& key) {
        return NULL;
    }

    bool is_valid() {
        return mmap.is_valid();
    }

    void print_lookup(ostream& outs = cout) {
        /// Find biggest key string for setw() formatting
        size_t _width = 0;
        Iterator it = begin();
        for (it; it != end(); it++)
            _width = ((*it).key.size() > _width ? (*it).key.size() : _width);

        it = begin();
        for (it; it != end(); it++) {
            outs << setw(_width) << (*it).key << " : " << setw(3) << "";
            for (string v : (*it).value_list) {
                outs << v << " ";
            }
            outs << endl;
        }
    }

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me) {
        outs << print_me.mmap << endl;
        return outs;
    }
};

#endif