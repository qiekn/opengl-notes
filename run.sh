#!/bin/bash

# Config
BUILD_DIR="build"
OBJECT_NAME="opgl"
DEBUGGER="lldb"

# If the build directory does not exist, run cmake first
if [ ! -d "${BUILD_DIR}" ]; then
  echo "[INFO] ${BUILD_DIR} not found, mkdir and running cmake..."
  cmake -S . -B "${BUILD_DIR}" || exit 1
fi

if [ "$1" = "debug" ]; then
  ${DEBUGGER} "./${BUILD_DIR}/${OBJECT_NAME}"
else
  make -j"$(nproc)" -C "${BUILD_DIR}" && "./${BUILD_DIR}/${OBJECT_NAME}"
fi

# vim: ft=sh ts=2 sw=2 et
