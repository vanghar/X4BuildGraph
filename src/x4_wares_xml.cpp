//
// Created by mike on 1/2/2025.
//

#include "x4_wares_xml.h"

#include <string>
#include <vector>
#include <xercesc/dom/DOMNode.hpp>
#include "xml/xml_node_util.h"
#include "xml/xml_attr_util.h"

using namespace std;
using namespace xercesc;

/*******************************************************************************
* Helper functions
*******************************************************************************/
StorageType to_storage_type(const string& storage_type) {
    if (storage_type == "liquid")
        return STORAGE_TYPE_GAS; // TODO convert 'GAS' to 'LIQUID' then use string hacks for this
    if (storage_type == "solid")
        return STORAGE_TYPE_SOLID;
    if (storage_type == "container")
        return STORAGE_TYPE_CONTAINER;
    return STORAGE_TYPE_UNSPECIFIED;
}

void to_raw_material(const DOMNode& dom_node, RawMaterial& material) {
    material.set_id(get_named_attr_value(dom_node, "id"));
    material.set_storage_type(to_storage_type(get_named_attr_value(dom_node, "transport")));
    material.set_volume(stoi(get_named_attr_value(dom_node, "volume")));
}

void to_refined_product(const DOMNode& dom_node, RefinedProduct& product) {
    product.set_id(get_named_attr_value(dom_node, "id"));
    product.set_storage_type(to_storage_type(get_named_attr_value(dom_node, "transport")));
    product.set_volume(stoi(get_named_attr_value(dom_node, "volume")));
    // Required wares need to be looked up after the extraction pass is complete
}

vector<string> get_required_ware_names(xercesc_3_3::DOMNode* ware_node) {
    vector<string> required_ware_names;
    auto required_ware_nodes = get_named_nested_nodes({ware_node}, "production/primary/ware");
    for (const auto& req_ware_node: required_ware_nodes) {
        auto ware_name = get_named_attr_value(*req_ware_node, "ware");
        required_ware_names.push_back(ware_name);
    }
    return required_ware_names;
}

bool is_ware(const DOMNode& dom_node) {
    const auto node_name = xml_to_str(dom_node.getNodeName());
    return node_name == "ware";
}

bool is_raw_material(const DOMNode& dom_node) {
    if (! is_ware(dom_node))
        return false;
    const auto tags_attr = get_named_attr(dom_node, "tags");
    return tags_attr ? string_attr_contains_all(*tags_attr, {"economy", "mineral"}) : false;
}

bool is_refined_product(const DOMNode& dom_node) {
    if (! is_ware(dom_node))
        return false;
    const auto tags_attr = get_named_attr(dom_node, "tags");
    return tags_attr ? string_attr_contains_all(*tags_attr, {"economy", "container"}) : false;
}


unordered_map<string,EconomyWare> extract_economy_wares_internal(DOMElement& dom_element) {
    auto child_nodes = get_child_nodes(dom_element);

    // Protobuf types here will serve as inner messages in EconomyWare messages
    // so need to be stored as pointers.
    // This is because the "set_allocated" methods used to assign
    // inner protobuf fields transfer ownership of, and subsequently
    // free, the original pointer. If pointer types are not used
    // you end up with double free errors.
    std::unordered_map<string, RawMaterial*> raw_materials;
    std::unordered_map<string, RefinedProduct*> refined_products;
    std::unordered_map<string,DOMNode*> refined_product_nodes;

    // TODO - this can be simplified by just building the EconomyWare types immediately
    for (auto child_node : child_nodes["ware"]) {
        if (is_raw_material(*child_node)) {
            auto raw_material = new RawMaterial();
            to_raw_material(*child_node, *raw_material);
            raw_materials[raw_material->id()] = raw_material;
        }
        else if (is_refined_product(*child_node)) {
            auto refined_product = new RefinedProduct();
            to_refined_product(*child_node, *refined_product);
            auto id =refined_product->id();
            refined_products[id] = refined_product;
            // TODO - use ID instead of looking up attribute again
            refined_product_nodes[id] = child_node;
        }
    }
    // Now we can process the dependencies for refined products
    // production/primary/ware subNodes with "ware" attribute value
    for (auto& [name, product_node] : refined_product_nodes) {
        auto refined_product = refined_products[name];
        auto req_ware_names = get_required_ware_names(product_node);
        for (const auto& req_ware_name: req_ware_names) {
            refined_product->add_required_ware_ids(req_ware_name);
        }
    }

    unordered_map<string,EconomyWare> economy_wares;
    for (auto& [name, raw_material] : raw_materials) {
        auto economy_ware = EconomyWare();
        economy_ware.set_allocated_raw_material(raw_material);
        economy_wares[name] = economy_ware;
    }
    for (auto& [name, refined_product] : refined_products) {
        EconomyWare economy_ware;
        economy_ware.set_allocated_refined_product(refined_product);
        economy_wares[name] = economy_ware;
    }

    return economy_wares;
}

/*******************************************************************************
* Class methods
*******************************************************************************/
X4_WaresXml X4_WaresXml::create(const string& file_path) {
    auto xml_parser = XmlParser::create(file_path);
    X4_WaresXml wares_xml(std::move(xml_parser));
    return wares_xml;
}

unordered_map<string,EconomyWare> X4_WaresXml::extract_economy_wares() const {
    return extract_economy_wares_internal(_xml_parser.root_element());
}
