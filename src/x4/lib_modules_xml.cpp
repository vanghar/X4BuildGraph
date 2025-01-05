//
// Created by mike on 1/3/2025.
//

#include "lib_modules_xml.h"
#include "../xml/xml_node_util.h"
#include "../xml/xml_attr_util.h"

// // TODO - in general for this class, it's more useful to pull the information directly
// // from the macro definition files.
// /*******************************************************************************
// * Helper functions
// *******************************************************************************/
// bool is_production_module(const DOMNode& dom_node) {
//     const auto tags_attr = get_named_attr(dom_node, "tags");
//     return tags_attr ? array_attr_contains_all(*tags_attr, {"production", "module"}) : false;
// }
//
// bool is_storage_module(const DOMNode& dom_node) {
//     const auto tags_attr = get_named_attr(dom_node, "tags");
//     return tags_attr ? array_attr_contains_all(*tags_attr, {"storage", "module"}) : false;
// }
//
// ProductionModule to_production_module(const DOMNode& dom_node) {
//     auto prod_module = ProductionModule();
//     prod_module.set_module_id(get_named_attr_value(dom_node, "id"));
//     auto category_node = get_child_nodes(dom_node)["category"][0];
//     auto refined_product_name = get_named_attr_value(*category_node, "ware");
//     prod_module.set_product_name(refined_product_name);
//     return prod_module;
// }
//
// // TODO - Details about capacity and what's stored is in the macro definitions, so this is not that useful
// // under assets/structures/storage/macros/<macro_name>.xml
// StorageModule to_storage_module(const DOMNode& dom_node) {
//     auto storage_module = StorageModule();
//     storage_module.set_module_id(get_named_attr_value(dom_node, "id"));
//     return storage_module;
// }
//
// DockModule to_dock_module(const DOMNode& dom_node) {
//     auto dock_module = DockModule();
//     dock_module.set_module_id(get_named_attr_value(dom_node, "id"));
//     return dock_module;
// }
//
// /*******************************************************************************
// * Class methods
// *******************************************************************************/
// LibModulesXml LibModulesXml::create(const string& file_path) {
//     auto xml_parser = XmlParser::create(file_path);
//     LibModulesXml lib_modules_xml;
//     lib_modules_xml.populate_collections(xml_parser.root_element());
//     return lib_modules_xml;
// }
//
// LibModulesXml::LibModulesXml() = default;
//
// void LibModulesXml::populate_collections(const DOMElement &dom_element) {
//     auto child_nodes = get_child_nodes(dom_element);
//     std::unordered_map<string, DOMNode *> refined_product_nodes;
//     for (auto child_node: child_nodes["module"]) {
//         if (is_storage_module(*child_node)) {
//             auto storage_module = to_storage_module(*child_node);
//             storage_modules[storage_module.module_id()] = storage_module;
//         } else if (is_production_module(*child_node)) {
//             auto prod_module = to_production_module(*child_node);
//             production_modules[prod_module.module_id()] = prod_module;
//         } else if (is_dock_module(*child_node)) {
//             auto dock_module = to_dock_module(*child_node);
//             dock_modules[dock_module.module_id()] = dock_module;
//         }
//     }
// }
//
// unordered_map<string, StorageModule> LibModulesXml::get_storage_modules() {
//     return storage_modules;
// }
//
// unordered_map<string, ProductionModule> LibModulesXml::get_production_modules() {
//     return production_modules;
// }
//
// unordered_map<string, DockModule> LibModulesXml::get_dock_modules() {
//     return dock_modules;
// }