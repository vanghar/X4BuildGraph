//
// Created by mike on 1/1/2025.
//

#ifndef XML_ATTR_UTIL_H
#define XML_ATTR_UTIL_H

#include <xercesc/dom/DOM.hpp>
#include <boost/algorithm/string.hpp>
#include <ranges>
#include "common_util.h"

using namespace xercesc;
using namespace std;
using namespace boost;


inline DOMNode* get_named_attr(const DOMNode& domNode, const string& name) {
    auto nodeAttrs = domNode.getAttributes();
    auto attrName = XMLString::transcode(name.c_str());
    auto namedItem = nodeAttrs->getNamedItem(attrName);
    XMLString::release(&attrName);
    return namedItem;
}

inline string getAttrValue(const DOMNode& domNode) {
    auto valueStr = XMLString::transcode(domNode.getNodeValue());
    std::string retval(valueStr);
    XMLString::release(&valueStr);
    return retval;
}

inline string get_named_attr_value(const DOMNode& domNode, const string& name) {
    const auto namedAttr = get_named_attr(domNode, name);
    return getAttrValue(*namedAttr);
}

inline vector<string> getDelimitedValues(const DOMNode& domNode, const string& separator) {
    auto valueStr = XMLString::transcode(domNode.getNodeValue());
    vector<string> attrValues;
    split(attrValues, valueStr, boost::is_any_of(separator));
    XMLString::release(&valueStr);
    return attrValues;
    // unordered_set<string> uniqueVals(attrValues.begin(), attrValues.end());
    // return uniqueVals;
}

// TODO - assumes that values are separated by space
inline bool attr_contains_all(const DOMNode& domNode, const vector<string>& requiredValues) {
    auto delimValues = getDelimitedValues(domNode, " ");
    auto uniqueValues = toHashSet(delimValues);
    for (const auto& val : requiredValues) {
        if (ranges::find(uniqueValues, val) == uniqueValues.end())
            return false;
    }
    return true;
}

#endif //XML_ATTR_UTIL_H
