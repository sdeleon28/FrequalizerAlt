# Frequalizer justfile
# Requires: cmake, ninja (brew install cmake ninja)
# Note: Xcode Command Line Tools still needed for compiler/frameworks (xcode-select --install)

build_dir := "build/ninja"
build_dir_lr := "build/ninja-lr"

# Default: list recipes
default:
    @just --list

# Configure the project (Debug, M/S mode)
configure:
    cmake \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -G Ninja \
        -S . \
        -B {{build_dir}}
    ln -sf {{build_dir}}/compile_commands.json compile_commands.json

# Configure in Release mode
configure-release:
    cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 \
        -G Ninja \
        -S . \
        -B {{build_dir}}

# Configure in LR mode
configure-lr:
    cmake \
        -DLR_MODE=ON \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
        -G Ninja \
        -S . \
        -B {{build_dir_lr}}

# Configure LR in Release mode
configure-lr-release:
    cmake \
        -DLR_MODE=ON \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_OSX_DEPLOYMENT_TARGET=10.13 \
        -G Ninja \
        -S . \
        -B {{build_dir_lr}}

# Build the plugin (configure first if needed)
build: _ensure-configured
    cmake --build {{build_dir}}

# Build LR variant
build-lr: _ensure-configured-lr
    cmake --build {{build_dir_lr}}

# Build with verbose output (shows compiler commands)
build-verbose: _ensure-configured
    cmake --build {{build_dir}} --verbose

# Build only the VST3 target
build-vst3: _ensure-configured
    cmake --build {{build_dir}} --target frequalizer_VST3

# Build only the AU target
build-au: _ensure-configured
    cmake --build {{build_dir}} --target frequalizer_AU

# Clean build artifacts
clean:
    rm -rf {{build_dir}}

# Clean everything (all build dirs)
clean-all:
    rm -rf build

# Deploy built plugins to system plugin folders
deploy:
    cp -R {{build_dir}}/frequalizer_artefacts/Debug/VST3/Frequalizer*.vst3 ~/Library/Audio/Plug-Ins/VST3/
    cp -R {{build_dir}}/frequalizer_artefacts/Debug/AU/Frequalizer*.component ~/Library/Audio/Plug-Ins/Components/
    @echo "Deployed to ~/Library/Audio/Plug-Ins/"

# Deploy release artifacts to system plugin folders
deploy-release:
    cp -R {{build_dir}}/frequalizer_artefacts/Release/VST3/Frequalizer*.vst3 ~/Library/Audio/Plug-Ins/VST3/
    cp -R {{build_dir}}/frequalizer_artefacts/Release/AU/Frequalizer*.component ~/Library/Audio/Plug-Ins/Components/
    @echo "Deployed release build to ~/Library/Audio/Plug-Ins/"

# Build and deploy plugins
build-and-deploy: build deploy

# Full rebuild from scratch (clean + configure + build + deploy)
rebuild: clean configure build deploy

# Show build errors only (useful for agents)
check: _ensure-configured
    cmake --build {{build_dir}} 2>&1 | grep -E "error:|warning:" || echo "Build clean!"

# Configure LSP (compile_commands.json)
lsp: configure

# Open the built plugin in the system AU host for quick testing
[macos]
run-auval:
    auval -v aufx FqMS FFAU

# Print the path to built artifacts
artifacts:
    @echo "VST3: {{build_dir}}/frequalizer_artefacts/Debug/VST3/"
    @echo "AU:   {{build_dir}}/frequalizer_artefacts/Debug/AU/"
    @ls -la {{build_dir}}/frequalizer_artefacts/Debug/ 2>/dev/null || echo "(not built yet — run 'just build')"

# --- internal helpers ---

[private]
_ensure-configured:
    #!/usr/bin/env bash
    if [ ! -f {{build_dir}}/build.ninja ]; then
        echo "Build not configured, running configure..."
        just configure
    fi

[private]
_ensure-configured-lr:
    #!/usr/bin/env bash
    if [ ! -f {{build_dir_lr}}/build.ninja ]; then
        echo "LR build not configured, running configure-lr..."
        just configure-lr
    fi
