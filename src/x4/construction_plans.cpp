//
// Created by mike on 1/7/2025.
//

#include "construction_plans.h"

#include "../common_util.h"
#include "../xml/xml_attr_util.h"
#include "../../gen/x4_types.pb.h"

/**
 * Although the construction plans XML looks quite simple, there are actually quite a lot
 * of details that need to be considered.
 * It describes not only the module build order, but is also a graph of interconnectedness
 * between modules in a station.
 *
 * In addition to its position in the build list, the following properties of each "entry"
 * may need to be updated:
 * - entry.index
 * - entry.connection
 *   - may need to be removed if the entry is now the first module
 *   - may need to be added if the entry was previously the first module
 * - predecessor:
 *   - may need to be removed if the entry is now the first module
 *   - may need to be added if the entry was previously the first module
 *   - this node indicates:
 *     - the index of the module this entry is connected to
 *     - the connection point to which its "entry.connection" property is linked
 *
 * offset/position nodes will always remain unchanged.
 *
 * The algorithm here attempts to optimize the order of construction to make a factory productive
 * as early as possible.
 *
 * 1. The first priorities are docks or piers so that miners and traders/transports can dock with
 * the station.
 * 2. Next priority is any storage that is needed for the materials the station requires or produces
 * 3. Next are production modules. These are ordered based on the information taken from the wares
 * and structures XML configuration. This allows them to be built in the correct order so that a
 * newly operational production module can be assured of having prerequisites already being
 * produced, assuming there are any modules in the station that produce the required inputs.
 *
 * Further optimizations to consider:
 * - Minimize build time to productive capability: This requires build time information from
 *   data files, which can be combined with shortest/least weight graph path algorithms to come
 *   up with an optimal order. This will be considered here.
 *
 * - Optimized output: There are 2 aspects to this:
 *   - For stations with multiple instances of the same kind of production module, this will
 *     consider how many downstream modules can be served by existing upstream output capacity
 *     before a new upstream module needs to be constructed.
 *   - For modules whose productivity is improved by habitation, consider the build time and
 *     habitation capacity to determine position in the build cycle. Habitation build time
 *     and order is possibly affected by food or water production capacity.
 * These optimizations will be considered later.
 *
 * Habitation and defence modules are currently not prioritized so they will be scheduled to
 * be built last.
 */

BuildEntry to_build_entry(const xml_node &entry_node) {
    BuildEntry build_entry;
    build_entry.set_entry_index(entry_node.attribute("index").as_int());
    build_entry.set_macro_name(entry_node.attribute("macro").as_string());
    build_entry.set_local_connection(entry_node.attribute("connection").as_string());
    if (auto predecessor_node = entry_node.child("predecessor")) {
        build_entry.set_predecessor_index(predecessor_node.attribute("index").as_int());
        build_entry.set_predecessor_connection(predecessor_node.attribute("connection").as_string());
    }
    const auto offset_node = entry_node.child("offset");
    const auto position_node = offset_node.child("position");

    const auto entry_position = build_entry.mutable_position();
    entry_position->set_x_coord(position_node.attribute("x").as_double());
    entry_position->set_y_coord(position_node.attribute("y").as_double());
    entry_position->set_z_coord(position_node.attribute("z").as_double());

    if (const auto rotation_node = offset_node.child("rotation")) {
        const auto entry_rotation = build_entry.mutable_rotation();
        if (const auto pitch = rotation_node.attribute("pitch").as_double())
            entry_rotation->set_pitch(pitch);
        if (const auto yaw = rotation_node.attribute("yaw").as_double())
            entry_rotation->set_yaw(yaw);
        if (const auto roll = rotation_node.attribute("roll").as_double())
            entry_rotation->set_roll(roll);
    }
    return std::move(build_entry);
}

// TODO - macro order seems too simplistic because we need to know about the specific
//   prerequisites for each type of module so we can ensure we build a shortest path
//   that accounts for them at each stage. 
void reorder_plans(const string source_file, const vector<string> macro_order) {
    xml_document plans_doc;
    plans_doc.load_file(source_file.c_str());
    const auto &root_node = plans_doc.document_element();

    xml_document new_plans_doc;
    auto new_root_node = append_shallow_copy(new_plans_doc, root_node);

    for (const auto &plan_node: root_node.children("plan")) {
        unordered_map<string, vector<BuildEntry>> entry_node_map;
        for (const auto &entry_node: plan_node.children("entry")) {
            auto build_entry = to_build_entry(entry_node);
            entry_node_map[build_entry.macro_name()].push_back(std::move(build_entry));
        }

        // Now have a map of modules keyed by macro name
        // 1. Build a graph of the station, indexing nodes by macro name
        // 2. apply Dijkstra to the station graph to build sets of shortest paths between required groups
        //    of nodes


        vector<xml_node> ordered_entries;
        // NOTE: We only care about the first of each type of node! This gets the factory running
        // as quickly as possible
        for (auto macro_name: macro_order) {
            if (auto maybe_node_list = get_optional(entry_node_map, macro_name)) {
                auto node_list = maybe_node_list.value();
                auto first_entry = node_list[0];
                delete_at(node_list, 0);
                ordered_entries.push_back(first_entry);
            }
        }
        // Now add any other remaining entries into the ordered_entries list
        for (auto &[macro_name, node_entries]: entry_node_map) {
            ordered_entries.insert(ordered_entries.end(), node_entries.begin(), node_entries.end());
        }

        // Now remap the indexes in the nodes
        unordered_map<int, int> reindexing_order;
        for (int i = 0; i < ordered_entries.size(); i++) {
            auto entry_node = ordered_entries[i];
            auto index_attr = entry_node.attribute("index");
            auto new_value = i + 1;
            reindexing_order[index_attr.as_int()] = new_value;
            index_attr.set_value(i + 1);
        }

        // And remap the predecessor indexes
        // - The entire plan will have to be built as a graph first, which means we need to read connection
        // details from the module configuration to determine what is connected to what
        // Positions won't change, but reordering the graph will mean figuring out the connection hierarchy
        // as well as the module order.
        // index_modA.conn_snap1 <--> index_modB.conn_snap6
        // Optimal build order becomes a shortest path problem, so Dijkstra

        for (int i = 0; i < ordered_entries.size(); i++) {
            auto entry_node = ordered_entries[i];
            auto predecessor_node = entry_node.child("predecessor");
            if (predecessor_node) {
            }
            auto index_attr = entry_node.attribute("index");
            auto new_value = i + 1;
            reindexing_order[index_attr.as_int()] = new_value;
            index_attr.set_value(i + 1);
        }

        auto new_plan_node = append_shallow_copy(new_root_node, plan_node);
        for (auto entry_node: ordered_entries) {
            new_plan_node.append_copy(entry_node);
        }
    }
    if (!new_plans_doc.save_file("new_plans.xml", "  ")) {
        cout << "Failed to save" << endl;
    }
}
