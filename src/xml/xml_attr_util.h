//
// Created by mike on 1/1/2025.
//

#ifndef XML_ATTR_UTIL_H
#define XML_ATTR_UTIL_H

#include <string>
#include <vector>
#include <xercesc/dom/DOM.hpp>

using namespace xercesc;
using namespace std;

DOMNode* get_named_attr(const DOMNode& dom_node, const string& name);
string get_attr_value(const DOMNode& dom_node);
string get_named_attr_value(const DOMNode& domNode, const string& name);
bool string_attr_contains_all(const DOMNode& dom_node, const vector<string>& required_values);
bool array_attr_contains_all(const DOMNode& dom_node, const vector<string>& required_values);

#endif //XML_ATTR_UTIL_H
