//
// Created by mike on 1/4/2025.
//

#include "library_wares.h"
#include "enum_helper.h"
#include "../common_util.h"
#include "../xml/xml_attr_util.h"

using namespace pugi;

/*******************************************************************************
* Helper functions
*******************************************************************************/
bool is_raw_material(const xml_node& xml_node) {
    const auto tags_attr = xml_node.attribute("tags");
    return tags_attr ? string_attr_contains_all(tags_attr, {"economy", "mineral"}) : false;
}

bool is_refined_product(const xml_node& xml_node) {
    const auto tags_attr = xml_node.attribute("tags");
    return tags_attr ? string_attr_contains_all(tags_attr, {"economy", "container"}) : false;
}

bool is_module(const xml_node& xml_node) {
    const auto tags_attr = xml_node.attribute("tags");
    return tags_attr ? string_attr_contains_all(tags_attr, {"module"}) : false;
}

RawMaterial to_raw_material(const xml_node& xml_node) {
    const auto material_id = xml_node.attribute("id").as_string();
    const auto storage_type = to_storage_type(xml_node.attribute( "transport").as_string());
    const auto volume = xml_node.attribute("volume").as_int();

    RawMaterial raw_material;
    raw_material.set_material_id(material_id);
    raw_material.set_storage_type(storage_type);
    raw_material.set_volume(volume);
    return std::move(raw_material);
}

RefinedProduct to_refined_product(const xml_node& xml_node) {
    const auto production_node = xml_node.child("production");
    const auto effect_node = production_node.child("effects").child("effect");

    const auto product_id = xml_node.attribute("id").as_string();
    const auto storage_type = to_storage_type(xml_node.attribute( "transport").as_string());
    const auto volume = xml_node.attribute("volume").as_int();
    const auto production_time = production_node.attribute("time").as_int();
    const auto production_amount = production_node.attribute("amount").as_int();
    const auto workforce_factor = effect_node.attribute("product").as_float();

    RefinedProduct refined_product;
    refined_product.set_product_id(product_id);
    refined_product.set_storage_type(storage_type);
    refined_product.set_unit_volume(volume);
    refined_product.set_production_time(production_time);
    refined_product.set_production_amount(production_amount);
    refined_product.set_workforce_factor(workforce_factor);
    return std::move(refined_product);
}

GenericModule to_generic_module(const xml_node& xml_node) {
    const auto production_node = xml_node.child("production");
    const auto component_node = xml_node.child("component");

    const auto module_id = xml_node.attribute("id").as_string();
    const auto production_time = production_node.attribute("time").as_int();
    const auto macro_name = component_node.attribute("ref").as_string();

    GenericModule generic_module;
    generic_module.set_module_id(module_id);
    generic_module.set_macro_name(macro_name);
    generic_module.set_production_time(production_time);
    return std::move(generic_module);
}

/*******************************************************************************
* Class methods
*******************************************************************************/
LibraryWares LibraryWares::create(const string &unpack_root_path) {
    xml_document doc;
    doc.load_file((unpack_root_path + "/libraries/wares.xml").c_str());
    LibraryWares library_wares;
    library_wares.populate_collections(doc.document_element());
    return library_wares;
}

LibraryWares::LibraryWares() = default;

void LibraryWares::populate_collections(const xml_node& root_node) {
    auto ware_nodes = root_node.children("ware");
    std::unordered_map<string, xml_node> refined_product_nodes;
    for (auto& ware_node : ware_nodes) {
        if (is_raw_material(ware_node)) {
            auto raw_material = to_raw_material(ware_node);
            raw_materials[raw_material.material_id()] = raw_material;
        } else if (is_refined_product(ware_node)) {
            auto refined_product = to_refined_product(ware_node);
            refined_products[refined_product.product_id()] = refined_product;
            refined_product_nodes[refined_product.product_id()] = ware_node;
        }
    }

    // Populate dependencies for refined products
    // production/primary/ware subNodes with "ware" attribute value
    for (auto &[name, product_node]: refined_product_nodes) {
        auto required_wares = get_required_wares(product_node);
        auto& refined_product = refined_products[name];
        for (const auto &required_ware: required_wares) {
            *refined_product.add_required_wares() = required_ware;
        }
    }
}

vector<RequiredWare> LibraryWares::get_required_wares(const xml_node& product_node) {
    vector<RequiredWare> required_wares;
    const auto req_ware_nodes = product_node.child("production").child("primary").children("ware");
    for (const auto &req_ware_node: req_ware_nodes) {
        // The xpath select gets the values rather than the nodes
        auto required_ware = RequiredWare();
        auto ware_name = req_ware_node.attribute("ware").as_string();
        auto req_amount = req_ware_node.attribute("amount").as_int();
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
