//
// Created by mike on 1/3/2025.
//

#include "xml_attr_util.h"
#include "../common_util.h"

#include <ranges>

bool value_contains_all(const string& attr_value, const vector<string>& required_values, const string& separator) {
    const auto attr_values = split_delimited(attr_value, separator);
    auto unique_values = to_hash_set(attr_values);
    for (const auto& val : required_values) {
        if (ranges::find(unique_values, val) == unique_values.end())
            return false;
    }
    return true;
}

// TODO - support for multi space separators
// Assumes strings in the format "val1 val2"
bool string_attr_contains_all(const xml_attribute& xml_attr, const vector<string>& required_values) {
    string str_value(xml_attr.as_string());
    return value_contains_all(str_value, required_values, " ");
}

// TODO - better support for separator with or without spaces
// Assumings string in the format "[val1, val2]"
bool array_attr_contains_all(const xml_attribute& xml_attr, const vector<string>& required_values) {
    string str_value(xml_attr.as_string());
    // We could do this more optimially by manipulating the char* array, but not important here
    const auto trimmed = str_value.substr(1, str_value.size() - 2);
    return value_contains_all(trimmed, required_values, ", ");
}


xml_node append_shallow_copy(xml_node& target_node, const xml_node& original_node) {
    auto shallow_copy = target_node.append_child(original_node.name());
    shallow_copy.set_value(original_node.value());
    for (auto attr = original_node.first_attribute(); attr; attr = attr.next_attribute()) {
        shallow_copy.append_attribute(attr.name()) = attr.value();
    }
    return std::move(shallow_copy);
}
