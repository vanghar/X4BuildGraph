//
// Created by mike on 1/3/2025.
//

#ifndef LIB_MODULES_XML_H
#define LIB_MODULES_XML_H
#include <string>
#include <unordered_map>

#include "../xml/xml_parser.h"
#include "../../gen/ware_types.pb.h"


class LibModulesXml {
public:
    static LibModulesXml create(const string& file_path);
    [[nodiscard]] unordered_map<string,ProductionModule> extract_production_modules() const;
private:
    XmlParser _xml_parser;
    explicit LibModulesXml(XmlParser&& xml_parser);
};



#endif //LIB_MODULES_XML_H
