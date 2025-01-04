//
// Created by mike on 1/3/2025.
//

#ifndef LIB_WARES_XML_H
#define LIB_WARES_XML_H

#include <string>
#include <unordered_map>
#include "../../gen/ware_types.pb.h"
#include "../xml/xml_parser.h"

using namespace std;
using namespace xercesc;

class LibWaresXml {
public:
    static LibWaresXml create(const string &file_path);

    unordered_map<string, RefinedProduct> get_refined_products();

    unordered_map<string, RawMaterial> get_raw_materials();

    unordered_map<string, GenericModule> get_generic_modules();

private:
    LibWaresXml();

    void populate_collections(const DOMElement &dom_element);

    vector<RequiredWare> get_required_wares(DOMNode &product_node);

    unordered_map<string, RefinedProduct> refined_products;
    unordered_map<string, RawMaterial> raw_materials;
    unordered_map<string, GenericModule> generic_modules;
};

#endif //LIB_WARES_XML_H
