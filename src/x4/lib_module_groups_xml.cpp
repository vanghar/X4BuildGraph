//
// Created by mike on 1/3/2025.
//

#include "lib_module_groups_xml.h"
#include <string>
#include <vector>
#include <xercesc/dom/DOMNode.hpp>
#include "../xml/xml_node_util.h"
#include "../xml/xml_attr_util.h"

// /*******************************************************************************
// * Helper functions
// *******************************************************************************/
// GenericModule to_generic_module(const DOMNode& group_node) {
//     GenericModule generic_module;
//     generic_module.set_module_id(get_named_attr_value(group_node, "name"));
//     const auto select_node = get_child_nodes(group_node)["select"][0];
//     auto macro_name = get_named_attr_value(*select_node, "macro");
//     generic_module.set_macro_name(macro_name);
//     return generic_module;
// }
//
// unordered_map<string,GenericModule> extract_generic_modules_internal(const DOMElement& dom_element) {
//     auto child_nodes = get_child_nodes(dom_element);
//     std::unordered_map<string, GenericModule> generic_modules;
//     for (auto child_node : child_nodes["group"]) {
//         auto generic_module = to_generic_module(*child_node);
//         generic_modules[generic_module.module_id()] = generic_module;
//     }
//     return generic_modules;
// }
//
// /*******************************************************************************
// * Class methods
// *******************************************************************************/
// LibModuleGroupsXml::LibModuleGroupsXml(XmlParser&& xml_parser) : _xml_parser(std::move(xml_parser)) {}
//
// LibModuleGroupsXml LibModuleGroupsXml::create(const string& file_path) {
//     auto xml_parser = XmlParser::create(file_path);
//     LibModuleGroupsXml module_groups_xml(std::move(xml_parser));
//     return module_groups_xml;
// }
//
// unordered_map<string,GenericModule> LibModuleGroupsXml::extract_generic_modules() const {
//     return extract_generic_modules_internal(_xml_parser.root_element());
// }
