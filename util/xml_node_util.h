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

inline string xml_to_str(const XMLCh* xmlStr) {
    // Convert XMLCh to std::string
    auto cStr = XMLString::transcode(xmlStr);
    std::string stdStr(cStr);
    XMLString::release(&cStr);
    return stdStr;
}

inline string get_node_name(const DOMNode& domNode) {
    auto xmlName = domNode.getNodeName();
    return xml_to_str(xmlName);
}

inline unordered_map<string,vector<DOMNode*>> get_child_nodes(const DOMNode& domNode) {
    unordered_map<string,vector<DOMNode*>> childNodes;
    auto subNodes = domNode.getChildNodes();
    for (int i=0; i<subNodes->getLength(); ++i) {
        auto subNode = subNodes->item(i);
        auto nodeName = get_node_name(*subNode);
        childNodes[nodeName].push_back(subNode);
    }
    return childNodes;
}

inline vector<DOMNode*> get_named_child_nodes(const vector<DOMNode*>& domNodes, const string& childName) {
    vector<DOMNode*> namedNodes;
    for (auto domNode : domNodes) {
        auto namedChildNodes = get_child_nodes(*domNode)[childName];
        namedNodes.insert(namedNodes.end(), namedChildNodes.begin(), namedChildNodes.end());
        return namedNodes;
    }
    return namedNodes;
}

// // Requires a '/' separated node path
inline vector<DOMNode*> get_named_nested_nodes(vector<DOMNode*> domNodes, string nodePath) {
    vector<string> pathParts;
    split(pathParts, nodePath, boost::is_any_of("/"));
    for (const auto& nodeName : pathParts) {
        domNodes = get_named_child_nodes(domNodes, nodeName);
    }
    return domNodes;
}

#endif //XML_NODE_UTIL_H
