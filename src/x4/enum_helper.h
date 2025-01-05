//
// Created by mike on 1/5/2025.
//

#ifndef ENUM_HELPER_H
#define ENUM_HELPER_H

#include "../../gen/x4_types.pb.h"

using namespace std;

StorageType to_storage_type(const string &storage_type);
DockModuleType to_dock_module_type(const string &dock_type);


#endif //ENUM_HELPER_H
