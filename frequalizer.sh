#!/bin/bash
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

function print_usage() {
  echo ""
  echo "SYNOPSIS"
  echo ""
  echo "  ./frequalizer.sh [
      |macos: Build and open Xcode
      |macos-prod: Build and open Xcode in release mode
      |macos-lr: Build and open Xcode
      |macos-lr-prod: Build and open Xcode in release mode
      |lsp: Configure LSP
      |options: Print options
      |i: Iteractive with fzf
      |help
  ]"
}

function build_and_open_xcode_macos() {
  cmake \
    -DJUCE_BUILD_EXTRAS=ON \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 \
    -H"." \
    -B"build/xcode" \
    -G Xcode
  open build/xcode/frequalizer.xcodeproj
}

function build_and_open_xcode_macos_lr() {
  cmake \
    -DLR_MODE=ON \
    -DJUCE_BUILD_EXTRAS=ON \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 \
    -H"." \
    -B"build/xcode-lr" \
    -G Xcode
  open build/xcode-lr/frequalizer.xcodeproj
}

function build_and_open_xcode_macos_lr_prod() {
  cmake \
    -DLR_MODE=ON \
    -DJUCE_BUILD_EXTRAS=ON \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 \
    -H"." \
    -B"build/xcode-lr-prod" \
    -G Xcode
  open build/xcode-lr-prod/frequalizer.xcodeproj
}

function configure_lsp() {
  echo "+++ Configuring LSP."
  rm -rf build/lsp
  cmake \
    -DBUILD_TESTS=ON \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DJUCE_BUILD_EXTRAS=ON \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -H"." \
    -B"build/lsp" \
    -G Ninja
  rm compile_commands.json
  ln -s build/lsp/compile_commands.json compile_commands.json
  echo "+++ Compiling, which seems to make some LSP things work even if it fails (Ninja build neither supported nor necessary)."
  cmake --build build/lsp --target frequalizer
}

function print_options() {
  echo macos
  echo macos-prod
  echo lsp
  echo options
  echo help
}

function interactive_mode() {
  echo "+++ Interactive mode."
  echo "+++ Choose an option."
  print_options | fzf | xargs -I {} ./frequalizer.sh {}
}

case $1 in
  macos)
    build_and_open_xcode_macos
    ;;
  macos-prod)
    build_and_open_xcode_macos_prod
    ;;
  macos-lr)
    build_and_open_xcode_macos_lr
    ;;
  macos-lr-prod)
    build_and_open_xcode_macos_lr_prod
    ;;
  lsp)
    configure_lsp
    ;;
  options)
    print_options
    ;;
  i)
    interactive_mode
    ;;
  *)
    print_usage
    ;;
esac
