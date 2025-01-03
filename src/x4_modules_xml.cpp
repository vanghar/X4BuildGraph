//
// Created by mike on 1/2/2025.
//

#include "x4_modules_xml.h"
#include "xml/xml_node_util.h"
#include "xml/xml_attr_util.h"

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

void to_production_module(const DOMNode& dom_node, ProductionModule& material) {
    material.set_id(get_named_attr_value(dom_node, "id"));
    auto category_node = get_child_nodes(dom_node)["category"][0];
    auto refined_product_name = get_named_attr_value(*category_node, "ware");
    material.set_product_name(refined_product_name);
}


unordered_map<string,ProductionModule> extract_production_modules_internal(DOMElement& dom_element) {
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
            auto prod_module = ProductionModule();
            to_production_module(*child_node, prod_module);
            production_modules[prod_module.id()] = prod_module;
        }
        // TODO - still need macro name

    }

    return production_modules;
}

/*******************************************************************************
* Class methods
*******************************************************************************/
X4_ModulesXml X4_ModulesXml::create(const string& file_path) {
    auto xml_parser = XmlParser::create(file_path);
    X4_ModulesXml modules_xml(std::move(xml_parser));
    return modules_xml;
}

