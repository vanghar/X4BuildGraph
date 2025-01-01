#!/bin/bash

ROOT_DIR=$(pwd)
protoc -I=${ROOT_DIR}/proto --cpp_out=${ROOT_DIR}/gen ${ROOT_DIR}/proto/ware_types.proto