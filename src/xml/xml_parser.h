//
// Created by mike on 1/2/2025.
//

#ifndef XML_PARSER_H
#define XML_PARSER_H
#include <memory>
#include <string>
#include <xercesc/parsers/XercesDOMParser.hpp>

using namespace std;
using namespace xercesc;

// This could be modified to include a reset method to allow the dom parser to be reset and reused
class XmlParser {
public:
    static XmlParser create(const string& file_path);
    [[nodiscard]] DOMElement& root_element() const;

    // TODO - move to cpp implementation
    XmlParser(XmlParser&& other) noexcept : _xml_parser(std::move(other._xml_parser)) {}
    XmlParser& operator=(XmlParser&&) noexcept { return *this; }

private:
    // This needs to be wrapped in unique_ptr because the XercesDOMParser copy constructor
    // is deleted, so the value can't be stored as a reference.
    unique_ptr<XercesDOMParser> _xml_parser;

    static void xml_init();
    static void parse_document(const string& file_path, XercesDOMParser& dom_parser);

    XmlParser() = default;
    // TODO - move to cpp implementation
    explicit XmlParser(unique_ptr<XercesDOMParser>&& xml_parser) : _xml_parser(std::move(xml_parser)) {}
};

#endif //XML_PARSER_H
