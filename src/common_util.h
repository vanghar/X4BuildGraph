//
// Created by mike on 1/1/2025.
//

#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include <iostream>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <ranges>

using namespace std;

template <typename T>
std::unordered_set<T> to_hash_set(vector<T> values) {
    unordered_set<T> unique_vals(values.begin(), values.end());
    return std::move(unique_vals);
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
    if (it == map.end()) return optional<typename std::decay_t<T>::mapped_type>{};
    return optional<typename std::decay_t<T>::mapped_type>{it->second};
}

inline vector<string> split_delimited(const string& raw_value, const string& separator) {
    vector<string> attr_values;
    auto split_view = views::split(raw_value, separator);
    for (const auto& token : split_view) {
        attr_values.emplace_back(token.begin(), token.end());
    }
    return std::move(attr_values);
}

template<typename T>
void delete_at(vector<T> elements, const int index) {
    if (index >= 0 && index < elements.size()) {
        elements.erase(elements.begin() + index);
    }
}

#endif //COMMON_UTIL_H
