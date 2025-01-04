//
// Created by mike on 1/3/2025.
//

#ifndef LIB_MODULE_GROUPS_XML_H
#define LIB_MODULE_GROUPS_XML_H

#include <string>
#include <unordered_map>
#include "../../gen/ware_types.pb.h"
#include "../xml/xml_parser.h"

class LibModuleGroupsXml {
public:
    static LibModuleGroupsXml create(const string& file_path);
    [[nodiscard]] std::unordered_map<string,GenericModule> extract_generic_modules() const;

private:
    XmlParser _xml_parser;
    // TODO - move impl to cpp
    explicit LibModuleGroupsXml(XmlParser&& xml_parser);
};



#endif //LIB_MODULE_GROUPS_XML_H
