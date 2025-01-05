//
// Created by mike on 1/2/2025.
//

#include "xml_parser.h"
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "xml_node_util.h"
#include <iostream>

using namespace std;

/*******************************************************************************
* Helper functions
*******************************************************************************/
/**
 * Initializes Xerces XML platform utils. This function may be called multiple
 * times, but it must be called at least once before attempting to parse XML files
 * using a XercesDOMParser instance.
 */
void xml_init() {
    try {
        XMLPlatformUtils::Initialize();
    } catch (const XMLException &toCatch) {
        auto message = xml_to_str(toCatch.getMessage());
        cout << "Error initializing XMLPlatformUtils! "
                << endl
                << message
                << endl;
        throw;
    }
}

/**
 * Creates a document tree for the given document with the given XercesDOMParser.
 * @param file_path - full path to XML
 * @param dom_parser - XercesDOMParser to load the document tree into
 */
void parse_document(const string &file_path, XercesDOMParser &dom_parser) {
    auto xml_file_path = XMLString::transcode(file_path.c_str());
    const LocalFileInputSource inputSource(xml_file_path);

    try {
        dom_parser.parse(inputSource);
    } catch (const XMLException &toCatch) {
        auto message = xml_to_str(toCatch.getMessage());
        cout << "XMLException parsing file: '" << file_path << "' : " << message << endl;
        throw;
    }
    catch (const DOMException &toCatch) {
        auto message = xml_to_str(toCatch.msg);
        cout << "DOMException parsing file: '" << file_path << "' : " << message << endl;
        throw;
    }
    catch (...) {
        cout << "Exception parsing file: '" << file_path << "'" << endl;
        throw;
    }

    XMLString::release(&xml_file_path);
}

/*******************************************************************************
* Class methods
*******************************************************************************/
XmlParser XmlParser::create() {
    xml_init();

    HandlerBase err_handler;
    // XercesDOMParser dom_parser;
    auto dom_parser = std::make_unique<XercesDOMParser>();
    dom_parser->setValidationScheme(XercesDOMParser::Val_Auto);
    dom_parser->setDoNamespaces(true); // optional
    dom_parser->setErrorHandler(&err_handler);

    XmlParser xml_parser(std::move(dom_parser));
    return xml_parser;
}

DOMNode& XmlParser::load_file(const string &file_path) const {
    // Clear all memory associated with prior parses first
    // _xml_parser->resetDocumentPool();
    parse_document(file_path, *_xml_parser);
    return *_xml_parser->getDocument()->getDocumentElement();
}

XmlParser::XmlParser(unique_ptr<XercesDOMParser> &&xml_parser) : _xml_parser(std::move(xml_parser)) {
}

XmlParser::XmlParser(XmlParser &&other) noexcept : _xml_parser(std::move(other._xml_parser)) {
}

XmlParser &XmlParser::operator=(XmlParser &&) noexcept {
    return *this;
}
