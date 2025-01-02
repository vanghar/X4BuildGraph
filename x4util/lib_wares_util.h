//
// Created by mike on 1/1/2025.
//

#ifndef LIB_WARES_UTIL_H
#define LIB_WARES_UTIL_H

#include "../util/xml_attr_util.h"
#include "../util/xml_node_util.h"
#include "../util/common_util.h"
#include "../gen/ware_types.pb.h"
#include <google/protobuf/text_format.h>

const string WARE_NODE_NAME = "ware";
const string TAGS_ATTRIBUTE_NAME = "tags";

using namespace std;

inline bool isWare(const DOMNode& domNode) {
    // TODO - possibly doing too much work if this XMLCh* is cached/interned in some way?
    const auto nodeName = xmlToStr(domNode.getNodeName());
    return nodeName == WARE_NODE_NAME;
}

inline bool isRawMaterial(const DOMNode& domNode) {
    if (! isWare(domNode))
        return false;
    const auto tagsAttr = getNamedAttr(domNode, TAGS_ATTRIBUTE_NAME);
    const vector<string> required_tags = {"economy", "mineral"};
    return tagsAttr ? attrContainsAll(*tagsAttr, required_tags) : false;
}

inline bool isRefinedProduct(const DOMNode& domNode) {
    if (! isWare(domNode))
        return false;
    const auto tagsAttr = getNamedAttr(domNode, TAGS_ATTRIBUTE_NAME);
    const vector<string> required_tags = {"economy", "container"};
    return tagsAttr ? attrContainsAll(*tagsAttr, required_tags) : false;
}

inline StorageType toStorageType(const string& storageType) {
    if (storageType == "liquid")
        return STORAGE_TYPE_GAS; // TODO convert 'GAS' to 'LIQUID' then use string hacks for this
    if (storageType == "solid")
        return STORAGE_TYPE_SOLID;
    if (storageType == "container")
        return STORAGE_TYPE_CONTAINER;
    return STORAGE_TYPE_UNSPECIFIED;
}

// TODO - memory leak?
inline void toRawMaterial(const DOMNode& domNode, RawMaterial* material) {
    material->set_id(getNamedAttrValue(domNode, "id"));
    material->set_storage_type(toStorageType(getNamedAttrValue(domNode, "transport")));
    material->set_volume(stoi(getNamedAttrValue(domNode, "volume")));
}

inline void toRefinedProduct(const DOMNode& domNode, RefinedProduct* product) {
     product->set_id(getNamedAttrValue(domNode, "id"));
     product->set_storage_type(toStorageType(getNamedAttrValue(domNode, "transport")));
     product->set_volume(stoi(getNamedAttrValue(domNode, "volume")));
     // Required wares need to be looked up after the extraction pass is complete
}

inline vector<string> getRequiredWareNames(DOMNode* wareNode) {
    vector<string> requiredWareNames;
    auto requiredWareNodes = getNamedNestedNodes({wareNode}, "production/primary/ware");
    for (const auto& reqWareNode: requiredWareNodes) {
        auto wareName = getNamedAttrValue(*reqWareNode, "ware");
        requiredWareNames.push_back(wareName);
    }
    return requiredWareNames;
}

inline unordered_map<string,EconomyWare> extractEconomyWares(const DOMElement* domElement) {
    auto childNodes = getChildNodes(domElement);

    std::unordered_map<string, RawMaterial*> rawMaterials;
    std::unordered_map<string, RefinedProduct*> refinedProducts;
    std::unordered_map<string,DOMNode*> refinedProductNodes;

    for (auto childNode : childNodes["ware"]) {
        if (isRawMaterial(*childNode)) {
            auto rawMaterial = new RawMaterial(); // TODO - memory leak
            toRawMaterial(*childNode, rawMaterial);
            rawMaterials[rawMaterial->id()] = rawMaterial;
        }
        else if (isRefinedProduct(*childNode)) {
            auto refinedProduct = new RefinedProduct();
            toRefinedProduct(*childNode, refinedProduct);
            auto id =refinedProduct->id();
            refinedProducts[id] = refinedProduct;
            // TODO - use ID instead of looking up attribute again
            refinedProductNodes[id] = childNode;
        }
    }
    // Now we can process the dependencies for refined products
    // production/primary/ware subNodes with "ware" attribute value
    for (auto& [name, product_node] : refinedProductNodes) {
        auto refined_product = refinedProducts[name];
        auto reqWareNames = getRequiredWareNames(product_node);
        for (const auto& reqWareName: reqWareNames) {
            refined_product->add_required_ware_ids(reqWareName);
        }
    }

    std::string jsonString;
    unordered_map<string,EconomyWare> economyWares;
    for (auto kvPair : rawMaterials) {
        EconomyWare economyWare;
        auto val = kvPair.second;
        economyWare.set_allocated_raw_material(val);
        economyWares[kvPair.first] = economyWare;
        google::protobuf::TextFormat::PrintToString(*val, &jsonString);
        cout <<  jsonString << endl;
    }
    for (auto kvPair : refinedProducts) {
        EconomyWare economyWare;
        auto val = kvPair.second;
        economyWare.set_allocated_refined_product(val);
        economyWares[kvPair.first] = economyWare;
        google::protobuf::TextFormat::PrintToString(*val, &jsonString);
        cout <<  jsonString << endl;
    }

    return economyWares;
}

#endif //LIB_WARES_UTIL_H
