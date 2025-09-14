#!/bin/bash

# Config
PROJECT_NAME="main"
BUILD_DIR="build"
DEBUGGER="lldb"

# If the build directory does not exist, run cmake first
if [ ! -d "${BUILD_DIR}" ]; then
  echo "[INFO] ${BUILD_DIR} not found, mkdir and running cmake..."
  cmake -S . -B "${BUILD_DIR}" || exit 1
fi

if [ "$1" = "debug" ]; then
  ${DEBUGGER} "./${BUILD_DIR}/${PROJECT_NAME}"
elif [ "$1" = "cherno" ]; then
  make cherno -j"$(nproc)" -C "${BUILD_DIR}" && "./${BUILD_DIR}/cherno"
elif [ "$1" = "opgl" ]; then
  make opgl -j"$(nproc)" -C "${BUILD_DIR}" && "./${BUILD_DIR}/opgl"
fi

# vim: ft=sh ts=2 sw=2 et
