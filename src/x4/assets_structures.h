//
// Created by mike on 1/4/2025.
//

#ifndef ASSETS_STRUCTURES_H
#define ASSETS_STRUCTURES_H
#include <string>
#include <unordered_map>
#include "../../gen/x4_types.pb.h"
#include <pugixml.hpp>

using namespace std;

// This class will need some parser reset behavior to avoid being too heavyweight
class AssetsStructures {
public:
    static AssetsStructures create(const string &unpack_root_path);

    const unordered_map<string, StorageModule> &get_storage_modules();

    const unordered_map<string, ProductionModule> &get_production_modules();

    const unordered_map<string, DockModule> &get_dock_modules();

    const unordered_map<string, ConnectionModule> &get_connection_modules();

    const unordered_map<string, DefenceModule> &get_defence_modules();

    const unordered_map<string, HabitatModule> &get_habitat_modules();

private:
    AssetsStructures();

    void populate_collections(const string &unpack_root_path);

    void add_storage_module(const pugi::xml_node &root_node);

    void add_production_module(const pugi::xml_node &root_node);

    void add_dock_module(const pugi::xml_node &root_node);

    void add_connection_module(const pugi::xml_node &root_node);

    void add_habitat_module(const pugi::xml_node &root_node);

    void add_defence_module(const pugi::xml_node &root_node);

    unordered_map<string, StorageModule> storage_modules;
    unordered_map<string, ProductionModule> production_modules;
    unordered_map<string, DockModule> dock_modules;
    unordered_map<string, ConnectionModule> connection_modules;
    unordered_map<string, DefenceModule> defence_modules;
    unordered_map<string, HabitatModule> habitat_modules;
};


#endif //ASSETS_STRUCTURES_H
