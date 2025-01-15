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
StorageModule to_storage_module(const xml_node &root_node) {
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
ProductionModule to_production_module(const xml_node &root_node) {
    const auto macro_node = root_node.child("macro");
    const auto component_node = macro_node.child("component");
    const auto props_node = macro_node.child("properties");
    const auto production_node = props_node.child("production");
    const auto workforce_node = props_node.child("workforce");

    auto module_id = component_node.attribute("ref").as_string();
    auto macro_name = macro_node.attribute("name").as_string();
    auto product_name = production_node.attribute("wares").as_string();
    auto workforce_max = workforce_node.attribute("max").as_int();

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
DockModule to_dock_module(const xml_node &root_node) {
    const auto macro_node = root_node.child("macro");
    const auto component_node = macro_node.child("component");

    auto module_id = component_node.attribute("ref").as_string();
    auto macro_name = macro_node.attribute("name").as_string();
    const auto dock_module_type = to_dock_module_type(macro_node.attribute("class").as_string());

    DockModule dock_module;
    dock_module.set_module_id(module_id);
    dock_module.set_macro_name(macro_name);
    dock_module.set_dock_type(dock_module_type);

    return dock_module;
}

// TODO - quite a lot of copy/pasting here
ConnectionModule to_connection_module(const xml_node &root_node) {
    const auto macro_node = root_node.child("macro");
    const auto component_node = macro_node.child("component");

    auto module_id = component_node.attribute("ref").as_string();
    auto macro_name = macro_node.attribute("name").as_string();

    ConnectionModule connection_module;
    connection_module.set_module_id(module_id);
    connection_module.set_macro_name(macro_name);

    return connection_module;
}

DefenceModule to_defence_module(const xml_node &root_node) {
    const auto macro_node = root_node.child("macro");
    const auto component_node = macro_node.child("component");

    auto module_id = component_node.attribute("ref").as_string();
    auto macro_name = macro_node.attribute("name").as_string();

    DefenceModule defence_module;
    defence_module.set_module_id(module_id);
    defence_module.set_macro_name(macro_name);

    return defence_module;
}

HabitatModule to_habitat_module(const xml_node &root_node) {
    const auto macro_node = root_node.child("macro");
    const auto component_node = macro_node.child("component");

    auto module_id = component_node.attribute("ref").as_string();
    auto macro_name = macro_node.attribute("name").as_string();

    HabitatModule habitat_module;
    habitat_module.set_module_id(module_id);
    habitat_module.set_macro_name(macro_name);

    return habitat_module;
}

BuildModule to_build_module(const xml_node &root_node) {
    const auto macro_node = root_node.child("macro");
    const auto component_node = macro_node.child("component");
    const auto props_node = macro_node.child("properties");
    const auto workforce_node = props_node.child("workforce");

    auto module_id = component_node.attribute("ref").as_string();
    auto macro_name = macro_node.attribute("name").as_string();
    auto workforce_max = workforce_node.attribute("max").as_int();

    BuildModule build_module;
    build_module.set_module_id(module_id);
    build_module.set_macro_name(macro_name);
    build_module.set_workforce_max(workforce_max);

    return build_module;
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

const unordered_map<string, StorageModule> &AssetsStructures::get_storage_modules() {
    return storage_modules;
}

const unordered_map<string, ProductionModule> &AssetsStructures::get_production_modules() {
    return production_modules;
}

const unordered_map<string, DockModule> &AssetsStructures::get_dock_modules() {
    return dock_modules;
}

const unordered_map<string, ConnectionModule> &AssetsStructures::get_connection_modules() {
    return connection_modules;
}

const unordered_map<string, DefenceModule> &AssetsStructures::get_defence_modules() {
    return defence_modules;
}

const unordered_map<string, HabitatModule> &AssetsStructures::get_habitat_modules() {
    return habitat_modules;
}

const unordered_map<string, BuildModule> & AssetsStructures::get_build_modules() {
    return build_modules;
}

AssetsStructures AssetsStructures::create(const string &unpack_root_path) {
    AssetsStructures assets_structures;
    assets_structures.populate_collections(unpack_root_path);
    return assets_structures;
}

AssetsStructures::AssetsStructures() = default;

void AssetsStructures::add_storage_module(const xml_node &root_node) {
    auto storage_module = to_storage_module(root_node);
    storage_modules[storage_module.module_id()] = storage_module;
}

void AssetsStructures::add_production_module(const xml_node &root_node) {
    auto production_module = to_production_module(root_node);
    production_modules[production_module.module_id()] = production_module;
}

void AssetsStructures::add_dock_module(const xml_node &root_node) {
    auto dock_module = to_dock_module(root_node);
    dock_modules[dock_module.module_id()] = dock_module;
}

void AssetsStructures::add_connection_module(const pugi::xml_node &root_node) {
    auto connection_module = to_connection_module(root_node);
    connection_modules[connection_module.module_id()] = connection_module;
}

void AssetsStructures::add_habitat_module(const pugi::xml_node &root_node) {
    auto habitat_module = to_habitat_module(root_node);
    habitat_modules[habitat_module.module_id()] = habitat_module;
}

void AssetsStructures::add_defence_module(const pugi::xml_node &root_node) {
    auto defence_module = to_defence_module(root_node);
    defence_modules[defence_module.module_id()] = defence_module;
}

void AssetsStructures::add_build_module(const pugi::xml_node &root_node) {
    auto build_module = to_build_module(root_node);
    build_modules[build_module.module_id()] = build_module;
}

void AssetsStructures::populate_collections(const string &unpack_root_path) {
    using MethodPtr = void (AssetsStructures::*)(const xml_node&);
    unordered_map<string, MethodPtr> populator_lookup = {
        {"/assets/structures/storage/macros", &AssetsStructures::add_storage_module},
        {"/assets/structures/production/macros", &AssetsStructures::add_production_module},
        {"/assets/structures/dock/macros", &AssetsStructures::add_dock_module},
        {"/assets/structures/connectionmodules/macros", &AssetsStructures::add_connection_module},
        {"/assets/structures/habitat/macros", &AssetsStructures::add_habitat_module},
        {"/assets/structures/defence/macros", &AssetsStructures::add_defence_module},
        {"/assets/structures/buildmodule/macros", &AssetsStructures::add_build_module},
// processingmodule not included if scrap required
    };

    // const auto self = this;

    for (const auto &[macro_sub_dir, adder_fn]: populator_lookup) {
        const auto macro_file_dir = unpack_root_path + macro_sub_dir;
        for (const auto &macro_file: get_macro_file_paths(macro_file_dir)) {
            xml_document doc;
            doc.load_file(macro_file.c_str());
            (this->*adder_fn)(doc.document_element());
        }
    }
}
