#include <iostream>
#include <optional>
#include <ranges>

#include "gen/ware_types.pb.h"
#include "util/xml_node_util.h"
#include "util/xml_attr_util.h"
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
// #include <boost/algorithm/string.hpp>
#include <google/protobuf/text_format.h>
// // #include <boost/graph/adjacency_list.hpp>
//
using namespace xercesc;
using namespace std;
// using namespace boost;

const string WARE_NODE_NAME = "ware";
const string TAGS_ATTRIBUTE_NAME = "tags";


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

bool isWare(const DOMNode& domNode) {
    // TODO - possibly doing too much work if this XMLCh* is cached/interned in some way?
    const auto nodeName = xmlToStr(domNode.getNodeName());
    return nodeName == WARE_NODE_NAME;
}

bool isRawMaterial(const DOMNode& domNode) {
    if (! isWare(domNode))
        return false;
    const auto tagsAttr = getNamedAttr(domNode, TAGS_ATTRIBUTE_NAME);
    const vector<string> required_tags = {"economy", "mineral"};
    return tagsAttr ? attrContainsAll(*tagsAttr, required_tags) : false;
}

bool isRefinedProduct(const DOMNode& domNode) {
    if (! isWare(domNode))
        return false;
    const auto tagsAttr = getNamedAttr(domNode, TAGS_ATTRIBUTE_NAME);
    const vector<string> required_tags = {"economy", "container"};
    return tagsAttr ? attrContainsAll(*tagsAttr, required_tags) : false;
}

struct EconomyWare {
    RawMaterial* rawMaterial;
    RefinedProduct* refinedProduct;
};

StorageType toStorageType(const string& storageType) {
    if (storageType == "liquid")
        return STORAGE_TYPE_GAS; // TODO convert 'GAS' to 'LIQUID' then use string hacks for this
    if (storageType == "solid")
        return STORAGE_TYPE_SOLID;
    if (storageType == "container")
        return STORAGE_TYPE_CONTAINER;
    return STORAGE_TYPE_UNSPECIFIED;
}

// TODO - memory leak?
void toRawMaterial(const DOMNode& domNode, RawMaterial* material) {
    material->set_id(getNamedAttrValue(domNode, "id"));
    material->set_storage_type(toStorageType(getNamedAttrValue(domNode, "transport")));
    material->set_volume(stoi(getNamedAttrValue(domNode, "volume")));
}

void toRefinedProduct(const DOMNode& domNode, RefinedProduct* product) {
     product->set_id(getNamedAttrValue(domNode, "id"));
     product->set_storage_type(toStorageType(getNamedAttrValue(domNode, "transport")));
     product->set_volume(stoi(getNamedAttrValue(domNode, "volume")));
     // Required wares need to be looked up after the extraction pass is complete
}

vector<string> getRequiredWareNames(DOMNode* wareNode) {
    vector<string> requiredWareNames;
    auto requiredWareNodes = getNamedNestedNodes({wareNode}, "production/primary/ware");
    for (const auto& reqWareNode: requiredWareNodes) {
        auto wareName = getNamedAttrValue(*reqWareNode, "ware");
        requiredWareNames.push_back(wareName);
    }
    return requiredWareNames;
}

vector<EconomyWare> extractEconomyWares(const DOMElement* domElement) {
    auto childNodes = getChildNodes(domElement);

    std::unordered_map<string, RawMaterial*> rawMaterials;
    std::unordered_map<string, RefinedProduct*> refinedProducts;
    std::unordered_map<string,DOMNode*> refinedProductNodes;

    for (auto childNode : childNodes["ware"]) {
        if (isRawMaterial(*childNode)) {
            auto rawMaterial = new RawMaterial(); // TODO - memory leak
            toRawMaterial(*childNode, rawMaterial);
            rawMaterials[rawMaterial->id()] = rawMaterial;
        }
        else if (isRefinedProduct(*childNode)) {
            auto refinedProduct = new RefinedProduct();
            toRefinedProduct(*childNode, refinedProduct);
            auto id =refinedProduct->id();
            refinedProducts[id] = refinedProduct;
            // TODO - use ID instead of looking up attribute again
            refinedProductNodes[id] = childNode;
        }
    }
    // Now we can process the dependencies for refined products
    // production/primary/ware subNodes with "ware" attribute value
    for (auto idNodePair : refinedProductNodes) {
        // Need ID & required wares
        auto nodeId = idNodePair.first;
        auto reqWareNames = getRequiredWareNames(idNodePair.second);
        auto refinedProduct = refinedProducts[nodeId];
        for (auto reqWareName: reqWareNames) {
            // string reqWare = refinedProduct->add_required_wares();

            // refine
            // auto reqWare = refinedProduct->add_required_wares();
            // reqWare->set(reqWareName);
        }
    }

    std::string jsonString;
    vector<EconomyWare> economyWares;
    for (auto val : views::values(rawMaterials)) {
        EconomyWare economyWare;
        economyWare.rawMaterial = val;
        economyWares.push_back(economyWare);
        google::protobuf::TextFormat::PrintToString(*val, &jsonString);
        cout <<  jsonString << endl;
    }
    for (auto val : views::values(refinedProducts)) {
        EconomyWare economyWare;
        economyWare.refinedProduct = val;
        economyWares.push_back(economyWare);
        google::protobuf::TextFormat::PrintToString(*val, &jsonString);
        cout <<  jsonString << endl;
    }

    return economyWares;
}

void extractXmlData() {
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        auto message = XMLString::transcode(toCatch.getMessage());
        std::cout << "Error during initialization! :\n"
             << message << "\n";
        XMLString::release(&message);
        return;
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
        return;
    }
    catch (const DOMException& toCatch) {
        auto message = XMLString::transcode(toCatch.msg);
        std::cout << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
        return;
    }
    catch (...) {
        std::cout << "Unexpected Exception \n" ;
        return;
    }

    // TODO - use Arena for protobufs and clean up pointers
    auto xmlDoc = xmlParser->getDocument();
    auto rootElement = xmlDoc->getDocumentElement();
    auto ecoWares = extractEconomyWares(rootElement);

    delete errHandler;
    delete xmlParser;

    XMLPlatformUtils::Terminate();
}

/**
 * Fetches the dependency graph of raw materials and produced wares
 */
void generateWaresGraph() {

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
    extractXmlData();
    return 0;
}

// TIP See CLion help at <a
// href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>.
//  Also, you can try interactive lessons for CLion by selecting
//  'Help | Learn IDE Features' from the main menu.