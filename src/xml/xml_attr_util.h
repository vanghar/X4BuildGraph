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

/**
 * Gets a DOMNode representing the attribute of the given node with the given name.
 * For example for XML:
 * <foo bar="baz"/>
 * get_named_attr(foo_node, "bar")
 * will return bar_node
 * @param dom_node
 * @param name
 * @return
 */
DOMNode* get_named_attr(const DOMNode& dom_node, const string& name);

/**
 * Gets the value of the given attribute node
 * For example for XML:
 * <foo bar="baz"/>
 * get_attr_value(bar_attr_node, "bar")
 * will return "baz"
 * @param dom_node
 * @return
 */
string get_attr_value(const DOMNode& dom_node);

/**
 * Gets the value of the attribute of the given node with the given name.
 * For example for XML:
 * <foo bar="baz"/>
 * get_named_attr_value(foo_node, "bar")
 * will return "baz"
 * @param domNode
 * @param name
 * @return
 */
string get_named_attr_value(const DOMNode& domNode, const string& name);

/**
 * Determines if the value of the given attribute node contains all the given values.
 * The attribute value is expected contain space separated strings.
 * @param dom_node
 * @param required_values
 * @return
 */
bool string_attr_contains_all(const DOMNode& dom_node, const vector<string>& required_values);

/**
 * Determines if the value of the given attribute node contains all the given values.
 * The attribute value is expected to contain comma separated strings with leading '['
 * and trailing ']' characters.
 * @param dom_node
 * @param required_values
 * @return
 */
bool array_attr_contains_all(const DOMNode& dom_node, const vector<string>& required_values);

#endif //XML_ATTR_UTIL_H
