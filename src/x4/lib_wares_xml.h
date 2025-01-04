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
    static LibWaresXml create(const string& file_path);
    [[nodiscard]] std::unordered_map<string,EconomyWare> extract_economy_wares() const;

private:
    XmlParser _xml_parser;
    explicit LibWaresXml(XmlParser&& xml_parser);
};


#endif //LIB_WARES_XML_H
