//
// Created by mike on 1/7/2025.
//

#include "construction_plans.h"

#include "../common_util.h"
#include "../xml/xml_attr_util.h"

void reorder_plans(const string& source_file, const vector<string&> macro_order) {
    xml_document plans_doc;
    plans_doc.load_file(source_file.c_str());
    const auto& root_node = plans_doc.document_element();

    xml_document new_plans_doc;
    auto new_root_node = append_shallow_copy(new_plans_doc, root_node);

    for (const auto& plan_node : root_node.children("plan")) {
        unordered_map<string,xml_node&> entry_node_map;
        for (const auto& entry_node : plan_node.children("entry")) {
            auto macro_name = entry_node.attribute("name").as_string();
            entry_node_map[macro_name] = entry_node;
        }
        vector<xml_node&> ordered_entries;
        // NOTE: We only care about the first of each type of node! This gets the factory running
        // as quickly as possible
        for (auto macro_name : macro_order) {
            if (auto maybe_node_list = get_optional(entry_node_map[macro_name])) {
                auto first_entry = maybe_node_list[0];
                delete_at(maybe_node_list, 0);
                ordered_entries.push_back(first_entry);
            }
        }
        // Now add any other remaining entries into the ordered_entries list
        for (auto &[macro_name, node_entries] : entry_node_map) {
            ordered_entries.insert(ordered_entries.end(), node_entries.begin(), node_entries.end());
        }

        // Now remap the indexes in the nodes
        for (int i=0; i<ordered_entries.size(); i++) {
            auto entry_node = ordered_entries[i];
            auto index_attr = entry_node.attribute("index");
            index_attr.set_value(i+1);
        }

        auto new_plan_node = append_shallow_copy(new_root_node, plan_node);
        for (auto entry_node : ordered_entries) {
            new_plan_node.append_copy(entry_node);
        }
    }
    if (!new_plans_doc.save_file("new_plans.xml")) {
        cout << "Failed to save" << endl;
    }
}