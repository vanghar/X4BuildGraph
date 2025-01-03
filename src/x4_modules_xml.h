//
// Created by mike on 1/2/2025.
//

#ifndef X4_MODULES_XML_H
#define X4_MODULES_XML_H
#include <string>
#include <unordered_map>

#include "xml/xml_parser.h"
#include "../gen/ware_types.pb.h"

using namespace std;

class X4_ModulesXml {
public:
    static X4_ModulesXml create(const string& file_path);
    [[nodiscard]] unordered_map<string,ProductionModule> extract_production_modules() const;
private:
    XmlParser _xml_parser;
    // TODO - move impl to cpp
    explicit X4_ModulesXml(XmlParser&& xml_parser) : _xml_parser(std::move(xml_parser)) {}
};



#endif //X4_MODULES_XML_H
