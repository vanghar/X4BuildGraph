//
// Created by mike on 1/2/2025.
//

#include "xml_parser.h"

#include <iostream>
#include <xercesc/util/XMLString.hpp>

namespace xercesc_3_3 {
    class XMLException;
}

unique_ptr<XmlParser> XmlParser::create(const string& xml_file) {
    xml_init();
    auto xml_parser = make_unique<XmlParser>();
    xml_parser->parser_init(xml_file);
    return xml_parser;
}

DOMElement& XmlParser::root_element() {
    return *_xml_parser->getDocument()->getDocumentElement();
}
