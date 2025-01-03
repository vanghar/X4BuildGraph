//
// Created by mike on 1/1/2025.
//

#ifndef XML_ATTR_UTIL_H
#define XML_ATTR_UTIL_H

#include <xercesc/dom/DOM.hpp>
#include <boost/algorithm/string.hpp>
#include <ranges>
#include "common_util.h"
#include "xml_node_util.h"

using namespace xercesc;
using namespace std;
using namespace boost;


inline DOMNode* get_named_attr(const DOMNode& dom_node, const string& name) {
    auto node_attrs = dom_node.getAttributes();
    auto attr_name = XMLString::transcode(name.c_str());
    auto named_item = node_attrs->getNamedItem(attr_name);
    XMLString::release(&attr_name);
    return named_item;
}

inline string get_attr_value(const DOMNode& dom_node) {
    auto node_value = dom_node.getNodeValue();
    return xml_to_str(node_value);
}

inline string get_named_attr_value(const DOMNode& domNode, const string& name) {
    const auto namedAttr = get_named_attr(domNode, name);
    return get_attr_value(*namedAttr);
}

inline vector<string> get_delimited_values(const DOMNode& dom_node, const string& separator) {
    auto node_value = dom_node.getNodeValue();
    auto value_str = xml_to_str(node_value);
    vector<string> attrValues;
    split(attrValues, value_str, boost::is_any_of(separator));
    return attrValues;
}

// TODO - assumes that values are separated by space
inline bool attr_contains_all(const DOMNode& dom_node, const vector<string>& required_values) {
    auto delim_values = get_delimited_values(dom_node, " ");
    auto unique_values = to_hash_set(delim_values);
    for (const auto& val : required_values) {
        if (ranges::find(unique_values, val) == unique_values.end())
            return false;
    }
    return true;
}

#endif //XML_ATTR_UTIL_H
