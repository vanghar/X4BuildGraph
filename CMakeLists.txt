cmake_minimum_required(VERSION 3.20)
project(X4BuildGraph)

set(CMAKE_CXX_STANDARD 23)

find_package(pugixml REQUIRED)
find_package(Protobuf REQUIRED)

add_executable(X4BuildGraph main.cpp
        src/common_util.h
        src/xml/xml_attr_util.h
        src/xml/xml_attr_util.cpp

        gen/x4_types.pb.h
        gen/x4_types.pb.cc

        src/x4/library_wares.cpp
        src/x4/library_wares.h
        src/x4/assets_structures.cpp
        src/x4/assets_structures.h
        src/x4/enum_helper.cpp
        src/x4/enum_helper.h
        src/x4/construction_plans.cpp
        src/x4/construction_plans.h
        src/x4/construction_ratios.cpp
        src/x4/construction_ratios.h
)

#
#add_library(Util SHARED util/common_util.h)
#target_link_libraries(Util XercesC::XercesC)
#
##find_package(Protobuf REQUIRED)
#
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/ware_types.proto)

target_link_libraries(X4BuildGraph protobuf::libprotobuf pugixml::pugixml)