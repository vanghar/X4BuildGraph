//
// Created by mike on 1/4/2025.
//

#ifndef ASSETS_STRUCTURES_H
#define ASSETS_STRUCTURES_H
#include <string>
#include <unordered_map>
#include "../../gen/x4_types.pb.h"
#include "../xml/xml_parser.h"

using namespace std;

// This class will need some parser reset behavior to avoid being too heavyweight
class AssetsStructures {
public:
    static AssetsStructures create(const string &unpack_root_path);

    const unordered_map<string, StorageModule>& get_storage_modules();

    const unordered_map<string, ProductionModule>& get_production_modules();

    const unordered_map<string, DockModule>& get_dock_modules();
private:
    AssetsStructures();

    void populate_collections(const XmlParser &xml_parser, const string &unpack_root_path);

    // assets/structures/storage/macros
    // macros/macro.name
    // macros/macro/component.ref
    // macros/macro/properties/cargo.max
    unordered_map<string, StorageModule> storage_modules;
    // assets/structures/production/macros
    // macros/macro.name
    // macros/macro/component.ref
    // macros/macro/properties/production.wares
    // macros/macro/properties/workforce.max
    unordered_map<string, ProductionModule> production_modules;
    // assets/structures/dock/macros
    // macros/macro.name
    // macros/macro/component.ref
    unordered_map<string, DockModule> dock_modules;
    // TODO habitat modules, defence modules
};


#endif //ASSETS_STRUCTURES_H
