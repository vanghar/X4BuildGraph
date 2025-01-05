#include <iostream>
#include <optional>
#include <ranges>
#include <unordered_map>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <fstream>
#include <format>

#include "src/x4/assets_structures.h"
#include "src/x4/library_wares.h"
#include "src/xml/xml_parser.h"

using namespace xercesc;
using namespace std;
using namespace boost;
#include <boost/graph/graph_traits.hpp>


// const google::protobuf::Arena ARENA;

// TODO - read file locations from command line


LibraryWares get_wares(const string& unpack_root_path) {
    return LibraryWares::create(unpack_root_path);
}

AssetsStructures get_modules(const string& unpack_root_path) {
    return AssetsStructures::create(unpack_root_path);
}

/**
 * Fetches the dependency graph of raw materials and produced wares
 */
void get_wares_graph(LibraryWares &library_wares) {
    typedef adjacency_list<vecS, vecS, directedS, property<boost::vertex_name_t, std::string> > Graph;

    std::unordered_map<string, unsigned long> vertices;
    Graph g;
    // Add raw materials first
    for (const auto &[material_name, raw_material]: library_wares.get_raw_materials()) {
        auto v1 = add_vertex(material_name, g);
        vertices[material_name] = v1;
    }
    for (const auto &[product_name, refined_product]: library_wares.get_refined_products()) {
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
    std::ofstream dot_file("graph.dot");
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
    get_wares_graph(wares_data);
    XMLPlatformUtils::Terminate();
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.
