//
// Created by mike on 1/4/2025.
//

#include "assets_structures.h"

#include "../xml/xml_parser.h"
#include <filesystem>
#include "../xml/xml_node_util.h"
#include "../xml/xml_attr_util.h"
#include "enum_helper.h"

namespace fs = std::filesystem;

/*******************************************************************************
* Helper functions
*******************************************************************************/

// assets/structures/storage/macros
// macros/macro.name
// macros/macro/component.ref
// macros/macro/properties/cargo.max
StorageModule to_storage_module(const DOMNode &root_node) {
    auto macro_node = get_child_nodes(root_node)["macro"][0];
    const auto component_node = get_child_nodes(*macro_node)["component"][0];
    const auto cargo_node = get_named_nested_nodes({macro_node}, "properties/cargo")[0];

    auto module_id = get_named_attr_value(*component_node, "ref");
    auto macro_name = get_named_attr_value(*macro_node, "name");
    const auto storage_type = to_storage_type(get_named_attr_value(*cargo_node, "tags"));
    const auto storage_capacity = stoi(get_named_attr_value(*cargo_node, "max"));

    StorageModule storage_module;
    storage_module.set_module_id(module_id);
    storage_module.set_macro_name(macro_name);
    storage_module.set_storage_type(storage_type);
    storage_module.set_storage_capacity(storage_capacity);

    return storage_module;
}

// assets/structures/production/macros
// macros/macro.name
// macros/macro/component.ref
// macros/macro/properties/production.wares
// macros/macro/properties/workforce.max
ProductionModule to_production_module(const DOMNode &root_node) {
    auto macro_node = get_child_nodes(root_node)["macro"][0];
    const auto component_node = get_child_nodes(*macro_node)["component"][0];
    const auto production_node = get_named_nested_nodes({macro_node}, "properties/production")[0];
    const auto workforce_node = get_named_nested_nodes({macro_node}, "properties/workforce")[0];

    auto module_id = get_named_attr_value(*component_node, "ref");
    auto macro_name = get_named_attr_value(*macro_node, "name");
    auto product_name = get_named_attr_value(*production_node, "wares");
    auto workforce = stoi(get_named_attr_value(*workforce_node, "max"));

    ProductionModule production_module;
    production_module.set_module_id(module_id);
    production_module.set_macro_name(macro_name);
    production_module.set_product_name(product_name);
    production_module.set_workforce(workforce);

    return production_module;
}

// assets/structures/dock/macros
// macros/macro.name
// macros/macro/component.ref
DockModule to_dock_module(const DOMNode &root_node) {
    const auto macro_node = get_child_nodes(root_node)["macro"][0];
    const auto component_node = get_child_nodes(*macro_node)["component"][0];

    auto module_id = get_named_attr_value(*component_node, "ref");
    auto macro_name = get_named_attr_value(*macro_node, "name");
    const auto dock_module_type = to_dock_module_type(get_named_attr_value(*macro_node, "class"));

    DockModule dock_module;
    dock_module.set_module_id(module_id);
    dock_module.set_macro_name(macro_name);
    dock_module.set_dock_type(dock_module_type);

    return dock_module;
}

vector<string> get_macro_file_paths(const string &dir_path) {
    vector<string> file_paths;
    for (const auto &entry: fs::directory_iterator(dir_path)) {
        if (entry.is_regular_file()) {
            const string file_path = entry.path();
            if (file_path.ends_with("_macro.xml")) {
                file_paths.push_back(file_path);
            }
        }
    }
    return file_paths;
}

/*******************************************************************************
* Class methods
*******************************************************************************/

const unordered_map<string, StorageModule>& AssetsStructures::get_storage_modules() {
    return storage_modules;
}

const unordered_map<string, ProductionModule>& AssetsStructures::get_production_modules() {
    return production_modules;
}

const unordered_map<string, DockModule>& AssetsStructures::get_dock_modules() {
    return dock_modules;
}

AssetsStructures AssetsStructures::create(const string &unpack_root_path) {
    const auto xml_parser = XmlParser::create();
    AssetsStructures assets_structures;
    assets_structures.populate_collections(xml_parser, unpack_root_path);
    return assets_structures;
}

AssetsStructures::AssetsStructures() = default;

void AssetsStructures::populate_collections(const XmlParser &xml_parser, const string &unpack_root_path) {
    for (const auto &storage_path: get_macro_file_paths(unpack_root_path + "/assets/structures/storage/macros")) {
        auto &root_node = xml_parser.load_file(storage_path);
        auto storage_module = to_storage_module(root_node);
        storage_modules[storage_module.module_id()] = storage_module;
    }
    for (const auto &production_path: get_macro_file_paths(unpack_root_path + "/assets/structures/production/macros")) {
        auto &root_node = xml_parser.load_file(production_path);
        auto production_module = to_production_module(root_node);
        production_modules[production_module.module_id()] = production_module;
    }
    for (const auto &dock_path: get_macro_file_paths(unpack_root_path + "/assets/structures/dock/macros")) {
        auto &root_node = xml_parser.load_file(dock_path);
        auto dock_module = to_dock_module(root_node);
        dock_modules[dock_module.module_id()] = dock_module;
    }
}
