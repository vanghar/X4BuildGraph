//
// Created by mike on 1/2/2025.
//

#include "xml_parser.h"
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "../util/xml_node_util.h"

XmlParser XmlParser::create(const string& xml_file) {
    xml_init();

    HandlerBase err_handler;
    auto dom_parser = std::make_unique<XercesDOMParser>();
    dom_parser->setValidationScheme(XercesDOMParser::Val_Always);
    dom_parser->setDoNamespaces(true);    // optional
    dom_parser->setErrorHandler(&err_handler);

    parse_document(xml_file, *dom_parser);

    XmlParser xml_parser(std::move(dom_parser));
    return xml_parser;
}

DOMElement& XmlParser::root_element() const {
    return *_xml_parser->getDocument()->getDocumentElement();
}

void XmlParser::xml_init() {
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        auto message = xml_to_str(toCatch.getMessage());
        cout << "Error initializing XMLPlatformUtils! "
            << endl
            << message
            << endl;
        throw;
    }
}

void XmlParser::parse_document(const string& file_path, XercesDOMParser& dom_parser) {
    auto xml_file_path = XMLString::transcode(file_path.c_str());
    const LocalFileInputSource inputSource(xml_file_path);

    try {
        dom_parser.parse(inputSource);
    }
    catch (const XMLException& toCatch) {
        auto message = xml_to_str(toCatch.getMessage());
        std::cout << "XMLException parsing file: '" << file_path << "' : " << message << endl;
        throw;
    }
    catch (const DOMException& toCatch) {
        auto message = xml_to_str(toCatch.msg);
        std::cout << "DOMException parsing file: '" << file_path << "' : " << message << endl;
        throw;
    }
    catch (...) {
        std::cout << "Exception parsing file: '" << file_path << "'" << endl;
        throw;
    }

    XMLString::release(&xml_file_path);
}
