#!/bin/bash
SCRIPTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

function print_usage() {
  echo ""
  echo "SYNOPSIS"
  echo ""
  echo "  ./frequalizer.sh [
      |macos: Build and open Xcode
      |macos-prod: Build and open Xcode in release mode
      |config-ninja: Configure Ninja
      |ninja: Build with Ninja
      |config-ninja-prod: Configure Ninja in release mode
      |ninja-prod: Build with Ninja in release mode
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

function build_and_open_xcode_macos_prod() {
  cmake \
    -DJUCE_BUILD_EXTRAS=ON \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 \
    -H"." \
    -B"build/xcode-prod" \
    -G Xcode
  open build/xcode-prod/frequalizer.xcodeproj
}

function configure_ninja() {
  cmake \
    -DJUCE_BUILD_EXTRAS=ON \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -H"." \
    -B"build/ninja" \
    -G Ninja
}

function build_ninja() {
  cmake --build build/ninja --target frequalizer
  # Make sure it gets copied
  echo "+++ Copying VST3 to /Library/Audio/Plug-Ins/VST3"
  cp -r "build/ninja/frequalizer_artefacts/Debug/VST3/Frequalizer Alt.vst3" "/Library/Audio/Plug-Ins/VST3/Frequalizer Alt.vst3"
}

function configure_ninja_prod() {
  cmake \
    -DJUCE_BUILD_EXTRAS=ON \
    -DCMAKE_BUILD_TYPE:STRING=Release \
    -H"." \
    -B"build/ninja" \
    -G Ninja
}

function build_ninja_prod() {
  cmake --build build/ninja --target frequalizer
  echo "+++ Copying VST3 to /Library/Audio/Plug-Ins/VST3"
  cp -r "build/ninja/frequalizer_artefacts/Release/VST3/Frequalizer Alt.vst3" "/Library/Audio/Plug-Ins/VST3/Frequalizer Alt.vst3"
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
  echo config-ninja
  echo ninja
  echo config-ninja-prod
  echo ninja-prod
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
  config-ninja)
    configure_ninja
    ;;
  ninja)
    build_ninja
    ;;
  config-ninja-prod)
    configure_ninja_prod
    ;;
  ninja-prod)
    build_ninja_prod
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
