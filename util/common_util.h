//
// Created by mike on 1/1/2025.
//

#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include <iostream>
#include <unordered_set>

using namespace std;

template <typename T>
std::unordered_set<T> to_hash_set(vector<T> values) {
    unordered_set<T> unique_vals(values.begin(), values.end());
    return unique_vals;
}

template<typename K, typename V>
bool contains_key(unordered_map<K,V> map, K key) {
    return find(map.begin(), map.end(), key) != map.end();
}

// Copied from here:
// https://stackoverflow.com/questions/60118288/why-stdmap-does-not-have-a-findornull-method
template<typename T, typename K>
auto get_optional(T&& map, K&& key)
{
    auto it = map.find(std::forward<K>(key));
    if (it == map.end()) return optional<typename std::decay<T>::type::mapped_type>{};
    return optional<typename std::decay<T>::type::mapped_type>{it->second};
}

#endif //COMMON_UTIL_H
