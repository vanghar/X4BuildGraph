//
// Created by mike on 1/3/2025.
//

#include "lib_modules_xml.h"
#include "../xml/xml_node_util.h"
#include "../xml/xml_attr_util.h"

/*******************************************************************************
* Helper functions
*******************************************************************************/
bool is_production_module(const DOMNode& dom_node) {
    const auto tags_attr = get_named_attr(dom_node, "tags");
    return tags_attr ? array_attr_contains_all(*tags_attr, {"production", "module"}) : false;
}

bool is_storage_module(const DOMNode& dom_node) {
    const auto tags_attr = get_named_attr(dom_node, "tags");
    return tags_attr ? array_attr_contains_all(*tags_attr, {"storage", "module"}) : false;
}

ProductionModule to_production_module(const DOMNode& dom_node) {
    auto prod_module = ProductionModule();
    prod_module.set_id(get_named_attr_value(dom_node, "id"));
    auto category_node = get_child_nodes(dom_node)["category"][0];
    auto refined_product_name = get_named_attr_value(*category_node, "ware");
    prod_module.set_product_name(refined_product_name);
    return prod_module;
}

unordered_map<string,ProductionModule> extract_production_modules_internal(const DOMElement& dom_element) {
    auto child_nodes = get_child_nodes(dom_element);

    // Protobuf types here will serve as inner messages in EconomyWare messages
    // so need to be stored as pointers.
    // This is because the "set_allocated" methods used to assign
    // inner protobuf fields transfer ownership of, and subsequently
    // free, the original pointer. If pointer types are not used
    // you end up with double free errors.
    std::unordered_map<string, ProductionModule> production_modules;

    for (auto child_node : child_nodes["module"]) {
        if (is_production_module(*child_node)) {
            auto prod_module = to_production_module(*child_node);
            production_modules[prod_module.id()] = prod_module;
        }
    }

    return production_modules;
}

/*******************************************************************************
* Class methods
*******************************************************************************/
LibModulesXml::LibModulesXml(XmlParser&& xml_parser) : _xml_parser(std::move(xml_parser)) {}

LibModulesXml LibModulesXml::create(const string& file_path) {
    auto xml_parser = XmlParser::create(file_path);
    LibModulesXml modules_xml(std::move(xml_parser));
    return modules_xml;
}

unordered_map<string,ProductionModule> LibModulesXml::extract_production_modules() const {
    return extract_production_modules_internal(_xml_parser.root_element());
}
