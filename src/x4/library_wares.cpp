//
// Created by mike on 1/4/2025.
//

#include "library_wares.h"
#include "../xml/xml_attr_util.h"
#include "enum_helper.h"
#include "../common_util.h"
#include "../xml/xml_node_util.h"

/*******************************************************************************
* Helper functions
*******************************************************************************/
bool is_raw_material(const DOMNode &dom_node) {
    const auto tags_attr = get_named_attr(dom_node, "tags");
    return tags_attr ? string_attr_contains_all(*tags_attr, {"economy", "mineral"}) : false;
}

bool is_refined_product(const DOMNode &dom_node) {
    const auto tags_attr = get_named_attr(dom_node, "tags");
    return tags_attr ? string_attr_contains_all(*tags_attr, {"economy", "container"}) : false;
}

RawMaterial to_raw_material(const DOMNode &dom_node) {
    const auto material_id = get_named_attr_value(dom_node, "id");
    const auto storage_type = to_storage_type(get_named_attr_value(dom_node, "transport"));
    const auto volume = stoi(get_named_attr_value(dom_node, "volume"));

    RawMaterial raw_material;
    raw_material.set_material_id(material_id);
    raw_material.set_storage_type(storage_type);
    raw_material.set_volume(volume);
    return raw_material;
}

RefinedProduct to_refined_product(const DOMNode &dom_node) {
    const auto product_id = get_named_attr_value(dom_node, "id");
    const auto storage_type = to_storage_type(get_named_attr_value(dom_node, "transport"));
    const auto volume = stoi(get_named_attr_value(dom_node, "volume"));

    RefinedProduct refined_product;
    refined_product.set_product_id(product_id);
    refined_product.set_storage_type(storage_type);
    refined_product.set_volume(volume);
    return refined_product;
}

/*******************************************************************************
* Class methods
*******************************************************************************/
LibraryWares LibraryWares::create(const string &unpack_root_path) {
    auto xml_parser = XmlParser::create();
    const auto &root_element = xml_parser.load_file(unpack_root_path + "/libraries/wares.xml");
    LibraryWares library_wares;
    library_wares.populate_collections(root_element);
    return library_wares;
}

LibraryWares::LibraryWares() = default;

void LibraryWares::populate_collections(const DOMNode &root_element) {
    auto child_nodes = get_child_nodes(root_element);
    std::unordered_map<string, DOMNode *> refined_product_nodes;
    for (const auto child_node: child_nodes["ware"]) {
        if (is_raw_material(*child_node)) {
            auto raw_material = to_raw_material(*child_node);
            raw_materials[raw_material.material_id()] = raw_material;
        } else if (is_refined_product(*child_node)) {
            auto refined_product = to_refined_product(*child_node);
            refined_products[refined_product.product_id()] = refined_product;
            refined_product_nodes[refined_product.product_id()] = child_node;
        }
    }

    // Populate dependencies for refined products
    // production/primary/ware subNodes with "ware" attribute value
    for (auto &[name, product_node]: refined_product_nodes) {
        auto required_wares = get_required_wares(*product_node);
        auto& refined_product = refined_products[name];
        for (const auto &required_ware: required_wares) {
            *refined_product.add_required_wares() = required_ware;
        }
    }
}

vector<RequiredWare> LibraryWares::get_required_wares(DOMNode &ware_node) {
    vector<RequiredWare> required_wares;
    auto required_ware_nodes = get_named_nested_nodes({&ware_node}, "production/primary/ware");
    for (const auto &req_ware_node: required_ware_nodes) {
        auto required_ware = RequiredWare();
        auto ware_name = get_named_attr_value(*req_ware_node, "ware");
        auto req_amount = std::stoi(get_named_attr_value(*req_ware_node, "amount"));
        required_ware.set_required_ware_id(ware_name);
        required_ware.set_amount(req_amount);

        if (const auto maybe_raw_material = get_optional(get_raw_materials(), ware_name)) {
            required_ware.set_ware_type(WARE_TYPE_RAW_MATERIAL);
            required_wares.push_back(required_ware);
        } else if (const auto maybe_refined_product = get_optional(refined_products, ware_name)) {
            required_ware.set_ware_type(WARE_TYPE_REFINED_PRODUCT);
            required_wares.push_back(required_ware);
        }
    }
    return required_wares;
}

const unordered_map<string, RefinedProduct>& LibraryWares::get_refined_products() {
    return refined_products;
}

const unordered_map<string, RawMaterial>& LibraryWares::get_raw_materials() {
    return raw_materials;
}

const unordered_map<string, GenericModule>& LibraryWares::get_generic_modules() {
    return generic_modules;
}
