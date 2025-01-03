cmake_minimum_required(VERSION 3.20)
project(X4BuildGraph)

set(CMAKE_CXX_STANDARD 23)

find_package(XercesC REQUIRED)
find_package(Protobuf REQUIRED)

add_executable(X4BuildGraph main.cpp
        src/common_util.h
        src/xml/xml_attr_util.h
        src/xml/xml_attr_util.cpp
        src/xml/xml_node_util.h
        src/xml/xml_node_util.cpp
        src/xml/xml_parser.h
        src/xml/xml_parser.cpp

        gen/ware_types.pb.h
        gen/ware_types.pb.cc

        src/x4_wares_xml.cpp
        src/x4_wares_xml.h
        src/x4_modules_xml.cpp
        src/x4_modules_xml.h
)

#
#add_library(Util SHARED util/common_util.h)
#target_link_libraries(Util XercesC::XercesC)
#
##find_package(Protobuf REQUIRED)
#
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/ware_types.proto)

target_link_libraries(X4BuildGraph XercesC::XercesC protobuf::libprotobuf)