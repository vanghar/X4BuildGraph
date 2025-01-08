//
// Created by mike on 1/4/2025.
//

#ifndef LIBRARY_WARES_H
#define LIBRARY_WARES_H

#include <string>
#include <unordered_map>
#include "../../gen/x4_types.pb.h"
#include <pugixml.hpp>

using namespace std;

class LibraryWares {
public:
    static LibraryWares create(const string &file_path);

    const unordered_map<string, RefinedProduct>& get_refined_products();

    const unordered_map<string, RawMaterial>& get_raw_materials();

    const unordered_map<string, GenericModule>& get_generic_modules();

private:
    LibraryWares();

    void populate_collections(const pugi::xml_node& root_node);

    vector<RequiredWare> get_required_wares(const pugi::xml_node& product_node);

    unordered_map<string, RefinedProduct> refined_products;
    unordered_map<string, RawMaterial> raw_materials;
    unordered_map<string, GenericModule> generic_modules;
};



#endif //LIBRARY_WARES_H
