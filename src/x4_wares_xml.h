//
// Created by mike on 1/2/2025.
//

#ifndef X4_WARES_XML_H
#define X4_WARES_XML_H
#include <string>
#include <unordered_map>
#include "../gen/ware_types.pb.h"
#include "xml/xml_parser.h"

using namespace std;
using namespace xercesc;

class X4_WaresXml {
public:
    static X4_WaresXml create(const string& file_path);
    [[nodiscard]] std::unordered_map<string,EconomyWare> extract_economy_wares() const;

private:
    XmlParser _xml_parser;
    // TODO - move impl to cpp
    explicit X4_WaresXml(XmlParser&& xml_parser) : _xml_parser(std::move(xml_parser)) {}
};


#endif //X4_WARES_XML_H
