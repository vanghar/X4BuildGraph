//
// Created by mike on 1/3/2025.
//

#include "xml_attr_util.h"
#include "../common_util.h"

#include <ranges>
#include <boost/algorithm/string.hpp>
#include "xml_node_util.h"

using namespace boost;

DOMNode* get_named_attr(const DOMNode& dom_node, const string& name) {
    const auto node_attrs = dom_node.getAttributes();
    auto attr_name = XMLString::transcode(name.c_str());
    const auto named_item = node_attrs->getNamedItem(attr_name);
    XMLString::release(&attr_name);
    return named_item;
}

string get_attr_value(const DOMNode& dom_node) {
    const auto node_value = dom_node.getNodeValue();
    return xml_to_str(node_value);
}

string get_named_attr_value(const DOMNode& domNode, const string& name) {
    const auto named_attr = get_named_attr(domNode, name);
    return get_attr_value(*named_attr);
}

bool value_contains_all(const string& attr_value, const vector<string>& required_values, const string& separator) {
    const auto attr_values = split_delimited(attr_value, separator);
    auto unique_values = to_hash_set(attr_values);
    for (const auto& val : required_values) {
        if (ranges::find(unique_values, val) == unique_values.end())
            return false;
    }
    return true;
}

// Assumes strings in the format "val1 val2"
bool string_attr_contains_all(const DOMNode& dom_node, const vector<string>& required_values) {
    const auto attr_value = get_attr_value(dom_node);
    return value_contains_all(attr_value, required_values, " ");
}

// Assumings string in the format "[val1, val2]"
bool array_attr_contains_all(const DOMNode& dom_node, const vector<string>& required_values) {
    const auto attr_value = get_attr_value(dom_node);
    // Chop off leading and trailing square brackets
    const auto trimmed = attr_value.substr(1, attr_value.size() - 2);
    return value_contains_all(trimmed, required_values, ", ");
}
