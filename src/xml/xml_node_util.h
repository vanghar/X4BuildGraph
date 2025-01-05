//
// Created by mike on 1/1/2025.
//

#ifndef XML_NODE_UTIL_H
#define XML_NODE_UTIL_H

#include <xercesc/dom/DOM.hpp>
#include <string>
#include <unordered_map>
#include <vector>

using namespace xercesc;
using namespace std;

/**
 * Converts the given XMLCh pointer to a standard string
 * @param xml_str
 * @return std::string equivalent of given XMLCh sequence.
 */
string xml_to_str(const XMLCh* xml_str);

/**
 * Gets name of given DOMNode.
 * @param dom_node
 * @return
 */
string get_node_name(const DOMNode& dom_node);

/**
 * Gets hash map of DOMNode children of given node, keyed by their tag names.
 * For example:
 * <foo>
 *   <bar/>
 *   <baz/>
 * </foo>
 * will return a map as follows:
 * {
 *   bar: bar_node,
 *   baz: baz_node,
 * }
 * @param dom_node
 * @return
 */
unordered_map<string,vector<DOMNode*>> get_child_nodes(const DOMNode& dom_node);

/**
 * Gets a vector of all immediate child nodes whose tag match the given name.
 * For example, given this XML:
 * <foo>
 *   <bar> <!-- bar_node_1 -->
 *     <baz/> <!-- baz_node_1 -->
 *   </bar>
 *   <bar> <!-- bar_node_2 -->
 *     <baz/> <!-- baz_node_2 -->
 *   </bar>
 * </foo>
 * get_named_child_nodes({bar_node1, bar_node2}, "baz") will return a vector as follows:
 * [ baz_node1, baz_node2 ]
 * @param dom_nodes
 * @param child_name
 * @return
 */
vector<DOMNode*> get_named_child_nodes(const vector<DOMNode*>& dom_nodes, const string& child_name);

/**
 * Gets a vector of all immediate child nodes whose tag match the given path, where
 * the path is expected to be separated with '/' characters.
 * For example, given this XML:
 * <foo> <!-- foo_node -->
 *   <bar> <!-- bar_node_1 -->
 *     <baz/> <!-- baz_node_1 -->
 *   </bar>
 *   <bar> <!-- bar_node_2 -->
 *     <baz/> <!-- baz_node_2 -->
 *   </bar>
 * </foo>
 * get_named_child_nodes({foo_node}, "bar/baz") will return a vector as follows:
 * [ baz_node1, baz_node2 ]
 * @param dom_nodes
 * @param node_path
 * @return
 */
vector<DOMNode*> get_named_nested_nodes(vector<DOMNode*> dom_nodes, const string& node_path);

#endif //XML_NODE_UTIL_H
