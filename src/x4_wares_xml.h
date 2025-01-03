//
// Created by mike on 1/2/2025.
//

#ifndef X4_WARES_XML_H
#define X4_WARES_XML_H
#include <string>
#include <unordered_map>
#include <xercesc/dom/DOMNode.hpp>
#include "../gen/ware_types.pb.h"
#include "xml_parser.h"

using namespace std;
using namespace xercesc;

class X4_WaresXml {
public:
    static X4_WaresXml create(const string& file_path);
    std::unordered_map<string,EconomyWare> extract_economy_wares() const;

private:
    inline static const string WARE_NODE_NAME = "ware";
    inline static const std::string TAGS_ATTRIBUTE_NAME ="tags";

    static bool is_ware(const DOMNode& dom_node);
    static bool is_raw_material(const DOMNode& dom_node);
    static bool is_refined_product(const DOMNode& dom_node);
    static std::unordered_map<string,EconomyWare> _extract_economy_wares(DOMElement& dom_element);

    XmlParser _xml_parser;

    // X4_WaresXml() = default;
    explicit X4_WaresXml(XmlParser&& xml_parser) : _xml_parser(std::move(xml_parser)) {}
};


#endif //X4_WARES_XML_H
