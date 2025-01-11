//
// Created by mike on 1/4/2025.
//

#include "assets_structures.h"

#include <filesystem>
#include "enum_helper.h"

namespace fs = std::filesystem;
using namespace std;
using namespace pugi;

/*******************************************************************************
* Helper functions
*******************************************************************************/

// assets/structures/storage/macros
// macros/macro.name
// macros/macro/component.ref
// macros/macro/properties/cargo.max
StorageModule to_storage_module(const xml_node& root_node) {
    const auto macro_node = root_node.child("macro");
    const auto component_node = macro_node.child("component");
    const auto props_node = macro_node.child("properties");
    const auto cargo_node = props_node.child("cargo");

    auto module_id = component_node.attribute("ref").as_string();
    auto macro_name = macro_node.attribute("name").as_string();
    const auto storage_type = to_storage_type(cargo_node.attribute("tags").as_string());
    const auto storage_capacity = cargo_node.attribute("max").as_int();

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
ProductionModule to_production_module(const xml_node& root_node) {
    const auto macro_node = root_node.child("macro");
    const auto component_node = macro_node.child("component");
    const auto props_node = macro_node.child("properties");
    const auto production_node = props_node.child("production");
    const auto workforce_node = props_node.child("workforce");

    auto module_id = component_node.attribute("ref").as_string();
    auto macro_name = macro_node.attribute("name").as_string();
    auto product_name = production_node.attribute("wares").as_string();
    auto workforce_max = workforce_node.attribute( "max").as_int();

    ProductionModule production_module;
    production_module.set_module_id(module_id);
    production_module.set_macro_name(macro_name);
    production_module.set_product_name(product_name);
    production_module.set_workforce_max(workforce_max);

    return production_module;
}

// assets/structures/dock/macros
// macros/macro.name
// macros/macro/component.ref
DockModule to_dock_module(const xml_node& root_node) {
    const auto macro_node = root_node.child("macro");
    const auto component_node = macro_node.child("component");

    auto module_id = component_node.attribute("ref").as_string();
    auto macro_name = macro_node.attribute("name").as_string();
    const auto dock_module_type = to_dock_module_type(macro_node.attribute( "class").as_string());

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
    AssetsStructures assets_structures;
    assets_structures.populate_collections(unpack_root_path);
    return assets_structures;
}

AssetsStructures::AssetsStructures() = default;

void AssetsStructures::populate_collections(const string &unpack_root_path) {
    for (const auto &storage_path: get_macro_file_paths(unpack_root_path + "/assets/structures/storage/macros")) {
        xml_document doc;
        doc.load_file(storage_path.c_str());
        auto storage_module = to_storage_module(doc.document_element());
        storage_modules[storage_module.module_id()] = storage_module;
    }
    for (const auto &production_path: get_macro_file_paths(unpack_root_path + "/assets/structures/production/macros")) {
        xml_document doc;
        doc.load_file(production_path.c_str());
        auto production_module = to_production_module(doc.document_element());
        production_modules[production_module.module_id()] = production_module;
    }
    for (const auto &dock_path: get_macro_file_paths(unpack_root_path + "/assets/structures/dock/macros")) {
        xml_document doc;
        doc.load_file(dock_path.c_str());
        auto dock_module = to_dock_module(doc.document_element());
        dock_modules[dock_module.module_id()] = dock_module;
    }
}
