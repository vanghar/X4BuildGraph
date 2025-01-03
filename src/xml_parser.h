//
// Created by mike on 1/2/2025.
//

#ifndef XML_PARSER_H
#define XML_PARSER_H
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMException.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>

#include "../util/xml_node_util.h"

using namespace std;
using namespace xercesc;

class XmlParser {
public:
    static unique_ptr<XmlParser> create(const string& file_path);
    DOMElement& root_element();

    // Prevent copy construction and assignment (per Meyers singleton pattern)
    XmlParser(const XmlParser &) = delete;
    XmlParser& operator=(const XmlParser &) = delete;

    XmlParser() {}
    ~XmlParser() = default;

    // XmlParser(): _xml_parser(nullptr), _err_handler(nullptr), _xml_doc(nullptr) {}
    //
    // ~XmlParser() {
    //     if (_xml_doc != nullptr)
    //         delete _xml_doc;
    //     if (_err_handler != nullptr)
    //         delete _err_handler;
    //     if (_xml_parser != nullptr)
    //         delete _xml_parser;
    // }
private:
    unique_ptr<XercesDOMParser> _xml_parser;
    unique_ptr<HandlerBase> _err_handler;

    static void xml_init() {
        try {
            XMLPlatformUtils::Initialize();
        }
        // TODO - rethrow this
        catch (const XMLException& toCatch) {
            auto message = xmlToStr(toCatch.getMessage());
            cout << "Error initializing XMLPlatformUtils! "
                << endl
                << message
                << endl;
            throw;
        }
    }

    void parser_init(const string& xml_file) {
        _xml_parser = make_unique<XercesDOMParser>();

        _xml_parser->setValidationScheme(XercesDOMParser::Val_Always);
        _xml_parser->setDoNamespaces(true);    // optional

        _err_handler = make_unique<HandlerBase>();
        _xml_parser->setErrorHandler(_err_handler.get());

        parse_document(xml_file);
    }

    void parse_document(const string& file_path) {
        try {
            // auto xmlFile = "/mnt/d/Games/Steam/steamapps/common/X4 Foundations/unpacked/libraries/wares.xml";
            _xml_parser->parse(file_path.c_str());
        }
        catch (const XMLException& toCatch) {
            auto message = xmlToStr(toCatch.getMessage());
            std::cout << "XMLException parsing file: '" << file_path << "' : " << message << endl;
            throw;
        }
        catch (const DOMException& toCatch) {
            auto message = xmlToStr(toCatch.msg);
            std::cout << "DOMException parsing file: '" << file_path << "' : " << message << endl;
            throw;
        }
        catch (...) {
            std::cout << "Exception parsing file: '" << file_path << "'" << endl;
            throw;
        }
    }
};



#endif //XML_PARSER_H
