#include <iostream>
#include <ranges>
#include <unordered_map>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <fstream>
#include <format>

#include "src/x4/assets_structures.h"
#include "src/x4/library_wares.h"
// #include <boost/graph/graph_traits.hpp>
// #include <boost/graph/topological_sort.hpp>

#include "src/x4/construction_plans.h"

using namespace std;
using namespace boost;

// const google::protobuf::Arena ARENA;

// TODO - read file locations from command line


LibraryWares get_wares(const string& unpack_root_path) {
    return LibraryWares::create(unpack_root_path);
}

AssetsStructures get_modules(const string& unpack_root_path) {
    return AssetsStructures::create(unpack_root_path);
}

typedef adjacency_list<vecS, vecS, directedS, property<boost::vertex_name_t, std::string> > Graph;


/**
 * Fetches the dependency graph of raw materials and produced wares
 */
Graph get_wares_graph(LibraryWares &library_wares) {

    std::unordered_map<string, unsigned long> vertices;
    Graph g;
    // Add raw materials first
    for (const auto &material_name: library_wares.get_raw_materials() | views::keys) {
        auto v1 = add_vertex(material_name, g);
        vertices[material_name] = v1;
    }
    for (const auto &product_name: library_wares.get_refined_products() | views::keys) {
        auto v1 = add_vertex(product_name, g);
        vertices[product_name] = v1;
    }

    for (const auto &[product_name, refined_product]: library_wares.get_refined_products()) {
        for (const auto &req_ware: refined_product.required_wares()) {
            auto req_name = req_ware.required_ware_id();
            auto source_vertex = vertices[req_name];
            auto target_vertex = vertices[product_name];
            auto log_msg = format("{}({}) -> {}({})", source_vertex, req_name, target_vertex, product_name);
            cout << log_msg << endl;
            add_edge(source_vertex, target_vertex, g);
        }
    }
    std::ofstream dot_file("wares_graph.dot");
    write_graphviz(dot_file, g, make_label_writer(get(vertex_name, g)));
    return g;
}

vector<string> get_module_order(AssetsStructures& assets_structures, LibraryWares& library_wares) {
    std::unordered_map<string, unsigned long> vertices;
    Graph g;


    //
    //
    //
    // // Add raw materials first
    // for (const auto &material_name: library_wares.get_raw_materials() | views::keys) {
    //     auto v1 = add_vertex(material_name, g);
    //     vertices[material_name] = v1;
    // }
    // TODO - this is more useful keyed by macro name
    // This contains a mapping of produced ware to N vertices for modules that produce that ware
    std::unordered_map<string, vector<unsigned long>> wares_to_module_vertices;
    for (const auto &[module_name, production_module]: assets_structures.get_production_modules()) {
        auto v1 = add_vertex(module_name, g);
        vertices[module_name] = v1;
        wares_to_module_vertices[production_module.product_name()].push_back(v1);
    }

    // // Now perform a topological sort on the wares graph
    // typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
    // vector<Vertex> wares_vertex_list;
    // topological_sort(wares_graph, wares_vertex_list);
    //
    // for (const auto wares_vertex : wares_vertex_list) {
    //     auto wares_vertex_name = get(vertex_name, g, wares_vertex);
    //
    // }

    for (const auto &[product_name, refined_product]: library_wares.get_refined_products()) {
        auto consumer_vertices = wares_to_module_vertices[product_name];
        for (const auto &req_ware: refined_product.required_wares()) {
            auto req_name = req_ware.required_ware_id();
            auto producer_vertices = wares_to_module_vertices[req_name];
            for (const auto target_vertex : consumer_vertices) {
                for (const auto source_vertex : producer_vertices) {
                    auto log_msg = format("{}({}) -> {}({})", source_vertex, req_name, target_vertex, product_name);
                    cout << log_msg << endl;
                    add_edge(source_vertex, target_vertex, g);
                }
            }
        }
    }

    std::ofstream dot_file("modules_graph.dot");
    write_graphviz(dot_file, g, make_label_writer(get(vertex_name, g)));


}

/**
 * Using the wares graph, creates a module graph representing the order
 * in which modules should be built to create a productive factory at the
 * earliest possible build stage.
 *
 * The logic (to be used later) for this is as follows:
 * Considering only modules that are already defined in the plan:
 * 1. Prioritize building 1 dock and 1 pier
 * 2. Prioritize building 1 of each type of required storage
 * 3. Prioritize building 1 of each type of production module in the order
 *    required to start production ie raw materials->refined->high tech->ship etc
 *    The tier is not relevant here, it only considers dependencies as derived
 *    from the X4 .xml files.
 */
void generateModuleGraph() {
}

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or
// click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // TODO - get from command line
    const auto& unpack_root_path = "/mnt/d/Games/Steam/steamapps/common/X4 Foundations/unpacked";
    auto wares_data = get_wares(unpack_root_path);
    auto modules_data = get_modules(unpack_root_path);
    // auto wares_graph = get_wares_graph(wares_data);
    get_module_order(modules_data, wares_data);
    reorder_plans("/mnt/d/mike/constructionplans.xml", )
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.
