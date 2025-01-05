//
// Created by mike on 1/5/2025.
//

#include "enum_helper.h"

StorageType to_storage_type(const string &storage_type) {
    if (storage_type == "liquid")
        return STORAGE_TYPE_LIQUID;
    if (storage_type == "solid")
        return STORAGE_TYPE_SOLID;
    if (storage_type == "container")
        return STORAGE_TYPE_CONTAINER;
    return STORAGE_TYPE_UNSPECIFIED;
}

DockModuleType to_dock_module_type(const string &dock_type) {
    if (dock_type == "dockarea")
        return DOCK_MODULE_TYPE_DOCK;
    if (dock_type == "pier")
        return DOCK_MODULE_TYPE_PIER;
    // There's also a dockingbay type, but this is specific to ships, and we don't care about it here.
    return DOCK_MODULE_TYPE_UNSPECIFIED;
}
