//
// Created by mike on 1/3/2025.
//

#include "lib_wares_xml.h"

#include <string>
#include <vector>
#include <xercesc/dom/DOMNode.hpp>

#include "../common_util.h"
#include "../xml/xml_node_util.h"
#include "../xml/xml_attr_util.h"

using namespace std;
using namespace xercesc;

/*******************************************************************************
* Helper functions
*******************************************************************************/
StorageType to_storage_type(const string &storage_type) {
    if (storage_type == "liquid")
        return STORAGE_TYPE_GAS; // TODO convert 'GAS' to 'LIQUID' then use string hacks for this
    if (storage_type == "solid")
        return STORAGE_TYPE_SOLID;
    if (storage_type == "container")
        return STORAGE_TYPE_CONTAINER;
    return STORAGE_TYPE_UNSPECIFIED;
}

RawMaterial to_raw_material(const DOMNode &dom_node) {
    auto raw_material = RawMaterial();
    raw_material.set_material_id(get_named_attr_value(dom_node, "id"));
    raw_material.set_storage_type(to_storage_type(get_named_attr_value(dom_node, "transport")));
    raw_material.set_volume(stoi(get_named_attr_value(dom_node, "volume")));
    return raw_material;
}

RefinedProduct to_refined_product(const DOMNode &dom_node) {
    auto refined_product = RefinedProduct();
    refined_product.set_product_id(get_named_attr_value(dom_node, "id"));
    refined_product.set_storage_type(to_storage_type(get_named_attr_value(dom_node, "transport")));
    refined_product.set_volume(stoi(get_named_attr_value(dom_node, "volume")));
    return refined_product;
}

GenericModule to_simple_module(const DOMNode &dom_node) {
    auto generic_module = GenericModule();
    generic_module.set_module_id(get_named_attr_value(dom_node, "id"));
    auto component_node = get_child_nodes(dom_node)["component"][0];
    generic_module.set_macro_name(get_named_attr_value(*component_node, "ref"));
    return generic_module;
}

bool is_raw_material(const DOMNode &dom_node) {
    const auto tags_attr = get_named_attr(dom_node, "tags");
    return tags_attr ? string_attr_contains_all(*tags_attr, {"economy", "mineral"}) : false;
}

bool is_refined_product(const DOMNode &dom_node) {
    const auto tags_attr = get_named_attr(dom_node, "tags");
    return tags_attr ? string_attr_contains_all(*tags_attr, {"economy", "container"}) : false;
}

bool is_module(const DOMNode &dom_node) {
    const auto tags_attr = get_named_attr(dom_node, "tags");
    return tags_attr ? string_attr_contains_all(*tags_attr, {"module"}) : false;
}

/*******************************************************************************
* Class methods
*******************************************************************************/
LibWaresXml LibWaresXml::create(const string &file_path) {
    auto xml_parser = XmlParser::create(file_path);
    LibWaresXml lib_wares_xml;
    lib_wares_xml.populate_collections(xml_parser.root_element());
    return lib_wares_xml;
}

LibWaresXml::LibWaresXml() = default;

void LibWaresXml::populate_collections(const DOMElement &dom_element) {
    auto child_nodes = get_child_nodes(dom_element);
    std::unordered_map<string, DOMNode *> refined_product_nodes;
    for (auto child_node: child_nodes["ware"]) {
        if (is_raw_material(*child_node)) {
            auto raw_material = to_raw_material(*child_node);
            raw_materials[raw_material.material_id()] = raw_material;
        } else if (is_refined_product(*child_node)) {
            auto refined_product = to_refined_product(*child_node);
            refined_products[refined_product.product_id()] = refined_product;
            refined_product_nodes[refined_product.product_id()] = child_node;
        } else if (is_module(*child_node)) {
            auto generic_module = to_simple_module(*child_node);
            generic_modules[generic_module.module_id()] = generic_module;
        }
    }

    // Populate dependencies for refined products
    // production/primary/ware subNodes with "ware" attribute value
    for (auto &[name, product_node]: refined_product_nodes) {
        auto required_wares = get_required_wares(*product_node);
        auto refined_product = refined_products[name];
        for (const auto& required_ware: required_wares) {
            *refined_product.add_required_wares() = required_ware;
        }
        refined_products[name] = refined_product;
    }
}

vector<RequiredWare> LibWaresXml::get_required_wares(DOMNode &ware_node) {
    vector<RequiredWare> required_wares;
    auto required_ware_nodes = get_named_nested_nodes({&ware_node}, "production/primary/ware");
    for (const auto &req_ware_node: required_ware_nodes) {
        auto required_ware = RequiredWare();
        auto ware_name = get_named_attr_value(*req_ware_node, "ware");
        auto req_amount = std::stoi(get_named_attr_value(*req_ware_node, "amount"));
        required_ware.set_required_ware_id(ware_name);
        required_ware.set_amount(req_amount);

        if (const auto maybe_raw_material = get_optional(raw_materials, ware_name)) {
            required_ware.set_ware_type(WARE_TYPE_RAW_MATERIAL);
            required_wares.push_back(required_ware);
        } else if (const auto maybe_refined_product = get_optional(refined_products, ware_name)) {
            required_ware.set_ware_type(WARE_TYPE_REFINED_PRODUCT);
            required_wares.push_back(required_ware);
        }
    }
    return required_wares;
}

unordered_map<string, RefinedProduct> LibWaresXml::get_refined_products() {
    return refined_products;
}

unordered_map<string, RawMaterial> LibWaresXml::get_raw_materials() {
    return raw_materials;
}

unordered_map<string, GenericModule> LibWaresXml::get_generic_modules() {
    return generic_modules;
}
