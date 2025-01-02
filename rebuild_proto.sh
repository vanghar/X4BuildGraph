#!/bin/bash

ROOT_DIR=$(pwd)
mkdir -p gen
protoc -I=${ROOT_DIR}/proto --cpp_out=${ROOT_DIR}/gen ${ROOT_DIR}/proto/ware_types.proto
