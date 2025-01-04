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

string xml_to_str(const XMLCh* xml_str);
string get_node_name(const DOMNode& dom_node);
unordered_map<string,vector<DOMNode*>> get_child_nodes(const DOMNode& dom_node);
vector<DOMNode*> get_named_child_nodes(const vector<DOMNode*>& dom_nodes, const string& child_name);
vector<DOMNode*> get_named_nested_nodes(vector<DOMNode*> dom_nodes, const string& node_path);

#endif //XML_NODE_UTIL_H
