//
// Created by mike on 1/1/2025.
//

#ifndef XML_NODE_UTIL_H
#define XML_NODE_UTIL_H

#include <xercesc/dom/DOM.hpp>
#include <boost/algorithm/string.hpp>
#include <ranges>

using namespace xercesc;
using namespace std;

inline string xml_to_str(const XMLCh* xml_str) {
    auto c_str = XMLString::transcode(xml_str);
    std::string std_str(c_str);
    XMLString::release(&c_str);
    return std_str;
}

inline string get_node_name(const DOMNode& dom_node) {
    auto xml_name = dom_node.getNodeName();
    return xml_to_str(xml_name);
}

inline unordered_map<string,vector<DOMNode*>> get_child_nodes(const DOMNode& dom_node) {
    unordered_map<string,vector<DOMNode*>> child_nodes;
    auto sub_nodes = dom_node.getChildNodes();
    for (int i=0; i<sub_nodes->getLength(); ++i) {
        auto sub_node = sub_nodes->item(i);
        auto node_name = get_node_name(*sub_node);
        child_nodes[node_name].push_back(sub_node);
    }
    return child_nodes;
}

inline vector<DOMNode*> get_named_child_nodes(const vector<DOMNode*>& dom_nodes, const string& child_name) {
    vector<DOMNode*> named_nodes;
    for (auto dom_node : dom_nodes) {
        auto named_child_nodes = get_child_nodes(*dom_node)[child_name];
        named_nodes.insert(named_nodes.end(), named_child_nodes.begin(), named_child_nodes.end());
        return named_nodes;
    }
    return named_nodes;
}

// // Requires a '/' separated node path
inline vector<DOMNode*> get_named_nested_nodes(vector<DOMNode*> dom_nodes, string node_path) {
    vector<string> path_parts;
    split(path_parts, node_path, boost::is_any_of("/"));
    for (const auto& node_name : path_parts) {
        dom_nodes = get_named_child_nodes(dom_nodes, node_name);
    }
    return dom_nodes;
}

#endif //XML_NODE_UTIL_H
