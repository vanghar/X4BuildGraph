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

inline string xmlToStr(const XMLCh* xmlStr) {
    // Convert XMLCh to std::string
    auto cStr = XMLString::transcode(xmlStr);
    std::string stdStr(cStr);
    XMLString::release(&cStr);
    return stdStr;
}

inline string getNodeName(const DOMNode* domNode) {
    auto xmlName = domNode->getNodeName();
    return xmlToStr(xmlName);
}

inline unordered_map<string,vector<DOMNode*>> getChildNodes(const DOMNode* domNode) {
    unordered_map<string,vector<DOMNode*>> childNodes;
    auto subNodes = domNode->getChildNodes();
    for (int i=0; i<subNodes->getLength(); ++i) {
        auto subNode = subNodes->item(i);
        auto nodeName = getNodeName(subNode);
        childNodes[nodeName].push_back(subNode);
    }
    return childNodes;
}

inline vector<DOMNode*> getNamedChildNodes(const vector<DOMNode*>& domNodes, const string& childName) {
    vector<DOMNode*> namedNodes;
    for (auto domNode : domNodes) {
        auto namedChildNodes = getChildNodes(domNode)[childName];
        namedNodes.insert(namedNodes.end(), namedChildNodes.begin(), namedChildNodes.end());
        return namedNodes;
    }
    return namedNodes;
}

// // Requires a '/' separated node path
inline vector<DOMNode*> getNamedNestedNodes(vector<DOMNode*> domNodes, string nodePath) {
    vector<string> pathParts;
    split(pathParts, nodePath, boost::is_any_of("/"));
    for (const auto& nodeName : pathParts) {
        domNodes = getNamedChildNodes(domNodes, nodeName);
    }
    return domNodes;
}

#endif //XML_NODE_UTIL_H
