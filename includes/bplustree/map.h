#ifndef MAP_H
#define MAP_H
#include <iostream>
#include <string>
#include "pair.h"
#include "bplustree.h"

using namespace std;

template <typename K, typename V>
class Map {
public:
    typedef BPlusTree<Pair<K, V>> map_base;

    class Iterator {
    public:
        friend class Map;
        Iterator() {
            _it = nullptr;
        }
        Iterator(typename map_base::Iterator it) {
            _it = it;
        }
        Iterator operator++() {
            ++_it;
            return *this;
        }
        Iterator operator++(int unused) {
            Iterator hold = *this;
            ++(*this);
            return hold;
        }
        Pair<K, V> operator*() {
        if (!_it.is_null()) {
            return *_it;
        }
        return Pair<K, V>();
        }
        friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }
        friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }

    private:
        typename map_base::Iterator _it;
    };

    Map(){

    }

    // Iterators
    Iterator begin(){
        return Iterator(map.begin());
    }
    Iterator end(){
        return Iterator(map.end());
    }

    // Capacity
    int size() const {
        return map.size();
    }
    bool empty() const {
        return map.empty();
    }

    // Element Access
    V& operator[](const K& key) {
        // if (!map.contains(Pair<K, V>(key, V()))){
        //     map.insert(Pair<K, V>(key, V())); 
        // }
        return map.get(key).value;
    }
    V& at(const K& key) {
        return map.get_existing(key).value;
    }
    const V& at(const K& key) const {
        return map.get(key).value;
    }

    // Modifiers
    void insert(const K& k, const V& v){
        map.insert(Pair<K, V>(k, v));
    }
    void erase(const K& key) {
        map.remove(key);
    }
    void clear() {
        map.clear_tree();
    }

    // Operations
    Iterator find(const K& key) {
        return Iterator(map.find(key));
    }
    bool contains(const Pair<K, V>& target) const {
        return map.contains(target);
    }
    int count(const K& key) {
        return -1;
    }
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:
    Iterator lower_bound(const K& num) {
        return Iterator(map.lower_bound(num));
    }
    Iterator upper_bound(const K& num) {
        return Iterator(map.upper_bound(num));
    }
    Iterator equal_range( K num) {
        return Iterator(map.equal_range(num));
    }
    bool is_valid(){
        return map.is_valid();
    }

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me) {
        outs << print_me.map << endl;
        return outs;
    }

private:
    int key_count;
    BPlusTree<Pair<K, V>> map;
};

#endif
