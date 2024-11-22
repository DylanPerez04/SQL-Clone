#ifndef MAP_H
#define MAP_H

#include "bplustree.h"
#include <iostream>

using namespace std;

template <typename K, typename V> struct Pair {
    K key;
    V value;

    Pair(const K& k = K(), const V& v = V()) : key(k), value(v) {}
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me) {
        const bool debug = false;
        outs << print_me.key << ":" << print_me.value;
        return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key == rhs.key;
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key < rhs.key;
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key > rhs.key;
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return (lhs.key < rhs.key || lhs.key == rhs.key);
    }
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        assert(lhs.key == rhs.key);
        return Pair<K, V>(lhs.key, rhs.value);
    }
};

template <typename K, typename V> class Map {
private:
    int key_count;
    BPlusTree<Pair<K, V> > map{true};
public:
    typedef BPlusTree<Pair<K, V>> map_base;
    class Iterator {
    private:
        typename map_base::Iterator _it;
    public:
        friend class Map;
        Iterator(typename map_base::Iterator it = NULL) : _it(it) {}
        
        // postfix (i++)
        Iterator operator ++(int unused) { 
            return _it++; 
        }
        // prefix (++i)
        Iterator operator ++() {
            return ++_it;
        }
        Pair<K, V> operator *() {
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }
    };

    Map() : key_count(0) {}
    //  Iterators
    Iterator begin() {
        return Map<K, V>::Iterator(map.begin());
    }
    Iterator end() {
        return Map<K, V>::Iterator(map.end());
    }

    //  Capacity
    int size() const {
        return map.size();
    }
    bool empty() const {
        return map.empty();
    }

    //  Element Access
    V& operator[](const K& key) {
        return map.get(Pair<K, V>(key)).value;
    }
    V& at(const K& key) {
        return map.get_existing(Pair<K, V>(key)).value;
    }
    const V& at(const K& key) const {
        return map.get(Pair<K, V>(key)).value;
    }


    //  Modifiers
    void insert(const K& k, const V& v) {
        map.insert(Pair<K, V>(k, v));
    }
    void erase(const K& key) {
        map.remove(Pair<K, V>(key));
    }
    void clear() {
        map.clear_tree();
    }
    V get(const K& key) {
        return map.get(key).value;
    }

    //  Operations:
    Iterator find(const K& key) {
        return Map::Iterator(map.find(Pair<K, V>(key)));
    }
    bool contains(const Pair<K, V>& target) const {
        return map.contains(target);
    }

    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:

    int count(const K& key) {
        return -1;
    }

    Iterator lower_bound(const K& key) {
        return Map<K, V>::Iterator(map.lower_bound(Pair<K, V>(key)));
    }

    Iterator upper_bound(const K& key) {
        return Map<K, V>::Iterator(map.upper_bound(Pair<K, V>(key)));
    }

    Iterator equal_range(const K& key) {
        return NULL;
    }

    bool is_valid() { return map.is_valid(); }

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me) {
        outs << print_me.map << endl;
        return outs;
    }
};


#endif