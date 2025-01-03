cmake_minimum_required(VERSION 3.20)
project(X4BuildGraph)

set(CMAKE_CXX_STANDARD 23)

find_package(XercesC REQUIRED)
find_package(Protobuf REQUIRED)

add_executable(X4BuildGraph main.cpp
        util/common_util.h
        util/xml_attr_util.h
        util/xml_node_util.h
        gen/ware_types.pb.h
        gen/ware_types.pb.cc
        x4util/lib_wares_util.h
        src/xml_parser.cpp
        src/xml_parser.h
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