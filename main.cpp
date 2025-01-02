#include <iostream>
#include <optional>
#include <ranges>
#include <unordered_map>

#include "x4util/lib_wares_util.h"
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
// #include <boost/algorithm/string.hpp>
// #include <google/protobuf/text_format.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <fstream>
#include <format>
//
using namespace xercesc;
using namespace std;
// using namespace boost;
#include <boost/graph/graph_traits.hpp>


// const google::protobuf::Arena ARENA;

// TODO - read file locations from command line

// template <typename T>
// std::unique_ptr<T> uptr(T* ptr) {
//     return std::unique_ptr<T>(ptr);
// }



// template<typename T>
// T* createProto() {
//     return google::protobuf::Arena::CreateMessage<T>(&ARENA);
// }

// typedef adjacency_list<vecS, vecS, directedS> Graph;


// TODO - move all this somewhere else
std::unordered_map<string,EconomyWare> extractXmlData() {
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        auto message = XMLString::transcode(toCatch.getMessage());
        std::cout << "Error during initialization! :\n"
             << message << "\n";
        XMLString::release(&message);
        return std::unordered_map<string, EconomyWare>();
    }

    // TODO - build wrappers around this smart pointer stuff
    auto xmlParser = new XercesDOMParser();
    xmlParser->setValidationScheme(XercesDOMParser::Val_Always);
    xmlParser->setDoNamespaces(true);    // optional

    auto errHandler = new HandlerBase;
    xmlParser->setErrorHandler(errHandler);

    // FIXME - configure this separately, or at least allow the extract point to be specified

    try {
        auto xmlFile = "/mnt/d/Games/Steam/steamapps/common/X4 Foundations/unpacked/libraries/wares.xml";
        xmlParser->parse(xmlFile);
    }
    catch (const XMLException& toCatch) {
        auto message = XMLString::transcode(toCatch.getMessage());
        std::cout << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
        return std::unordered_map<string, EconomyWare>();
    }
    catch (const DOMException& toCatch) {
        auto message = XMLString::transcode(toCatch.msg);
        std::cout << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
        return std::unordered_map<string, EconomyWare>();
    }
    catch (...) {
        std::cout << "Unexpected Exception \n" ;
        return std::unordered_map<string, EconomyWare>();
    }

    // TODO - use Arena for protobufs and clean up pointers
    auto xmlDoc = xmlParser->getDocument();
    auto rootElement = xmlDoc->getDocumentElement();
    auto ecoWares = extractEconomyWares(rootElement);

    delete errHandler;
    delete xmlParser;

    XMLPlatformUtils::Terminate();

    return ecoWares;
}



/**
 * Fetches the dependency graph of raw materials and produced wares
 */
void generateWaresGraph(std::unordered_map<string,EconomyWare> &ecoWares) {
    // typedef pair<string,string> Edge;
    // typedef string Vertex;

    typedef adjacency_list<vecS, vecS, directedS, property<boost::vertex_name_t, std::string>> Graph;

    // TOOD - graph looks off
    std::unordered_map<string,unsigned long> vertices;
     Graph g;//(ecoWares.size());
     for (auto [eco_name, eco_ware] : ecoWares) {
         auto v1 = add_vertex(eco_name, g);
         vertices[eco_name] = v1;
     }

     for (auto [eco_name, eco_ware] : ecoWares) {
         if (eco_ware.has_refined_product()) {
             auto refined_product = eco_ware.refined_product();
             for (const auto& req_name : refined_product.required_ware_ids()) {
                 // auto reqWare = ecoWares[reqName];
                 auto source_vertex =vertices[req_name];
                 auto target_vertex = vertices[eco_name];
                 auto log_msg = format("{}({}) -> {}({})", source_vertex, req_name, target_vertex, eco_name);
                 cout << log_msg << endl;
                 add_edge(source_vertex,target_vertex, g);
             }
         }
     }
     std::ofstream dot_file("graph.dot");
//     ///auto label_writer = make_label_writer(get(vertex_name, g));
// //    auto label_writer = make_label_writer(name);
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
    auto data = extractXmlData();
    generateWaresGraph(data);
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.