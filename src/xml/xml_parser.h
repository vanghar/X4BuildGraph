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
    /**
     * Creates an initialized XmlParser that can be used to create XML document
     * trees using a wrapped XercesDOMParser.
     */
    static XmlParser create();

    /**
     * Loads the document tree for the given XML file.
     * @param file_path - full path to XML file to be loaded.
     * @return DOMNode of document tree root.
     */
    [[nodiscard]] DOMNode& load_file(const string& file_path) const;

    XmlParser(XmlParser&& other) noexcept;
    XmlParser& operator=(XmlParser&&) noexcept;
private:
    // Wrapped in unique_ptr so that it will be released when XmlParser goes out of scope
    unique_ptr<XercesDOMParser> _xml_parser;

    XmlParser() = default;
    explicit XmlParser(unique_ptr<XercesDOMParser>&& xml_parser);
};

#endif //XML_PARSER_H
