//
// Created by mike on 1/1/2025.
//

#ifndef XML_ATTR_UTIL_H
#define XML_ATTR_UTIL_H

#include <string>
#include <vector>
#include <pugixml.hpp>

using namespace std;
using namespace pugi;

/**
 * Determines if the value of the given attribute node contains all the given values.
 * The attribute value is expected contain space separated strings.
 * @param dom_attr
 * @param required_values
 * @return
 */
bool string_attr_contains_all(const xml_attribute& xml_attr, const vector<string>& required_values);

/**
 * Determines if the value of the given attribute node contains all the given values.
 * The attribute value is expected to contain comma separated strings with leading '['
 * and trailing ']' characters.
 * @param dom_attr
 * @param required_values
 * @return
 */
bool array_attr_contains_all(const xml_attribute& xml_attr, const vector<string>& required_values);

xml_node append_shallow_copy(xml_node& target_node, const xml_node& original_node);

#endif //XML_ATTR_UTIL_H
