#!/bin/bash
# Build script for Chapel container with CXI provider support

set -e

# Get the directory where this script is located, and resolve this project's
# root directory (build context for both target Containerfiles is always
# this directory, regardless of where the project itself lives on disk).
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Version configurations (can be overridden with environment variables)
LIBFABRIC_VERSION=${LIBFABRIC_VERSION:-2.3.1}
CHAPEL_VERSION=${CHAPEL_VERSION:-2.9.0}
CXI_VERSION=${CXI_VERSION:-release/shs-13.1.0}
CXI_DRIVER_COMMIT=${CXI_DRIVER_COMMIT:-3233be5}
LIBCXI_COMMIT=${LIBCXI_COMMIT:-ebd57a9}

# Container CLI to use. Auto-detects: prefers docker, falls back to podman if
# docker isn't on PATH. Override with the DOCKER_CMD environment variable
# (e.g. `DOCKER_CMD=podman ./scripts/build-chapel-dist-cxi-2.3.1-pic.sh`).
if [ -n "${DOCKER_CMD:-}" ]; then
    :
elif command -v docker >/dev/null 2>&1; then
    DOCKER_CMD="docker"
elif command -v podman >/dev/null 2>&1; then
    DOCKER_CMD="podman"
else
    DOCKER_CMD="docker"
fi

# Configuration
CONTAINER_NAME="chapel-${CHAPEL_VERSION}-libfabric-${LIBFABRIC_VERSION}-cxi-pic"
CONTAINERFILE="containers/Containerfile.hpe-cray-ex-chapel-pic"
IMAGE_TAG="localhost/${CONTAINER_NAME}:latest"

# Build container
echo "Building Chapel-Arkouda server container with CXI provider support..."
echo ""

cd "$PROJECT_DIR"

if [ ! -f "$CONTAINERFILE" ]; then
    echo "Error: $CONTAINERFILE not found"
    exit 1
fi

# Create build log directory
BUILD_LOG_DIR="${PROJECT_DIR}/build-logs"
mkdir -p "$BUILD_LOG_DIR"
BUILD_LOG="${BUILD_LOG_DIR}/cxi-build-$(date +%Y%m%d_%H%M%S).log"

echo "Build log: $BUILD_LOG"
echo ""

# Log the command line and environment for reproducibility
{
    echo "=========================================="
    echo "Build started at: $(date)"
    echo "=========================================="
    echo ""
    echo "Command: $0 $*"
    echo "Working directory: $(pwd)"
    echo ""
    echo "Build configuration:"
} | tee "$BUILD_LOG"

echo "Container CLI: ${DOCKER_CMD}" | tee -a "$BUILD_LOG"
echo "Building with versions:" | tee -a "$BUILD_LOG"
echo "  CHPL_TARGET_CPU=none" | tee -a "$BUILD_LOG"
echo "  libfabric=${LIBFABRIC_VERSION}" | tee -a "$BUILD_LOG"
echo "  Chapel=${CHAPEL_VERSION}" | tee -a "$BUILD_LOG"
echo "  CXI Version=${CXI_VERSION}" | tee -a "$BUILD_LOG"
echo "  CXI Driver Commit=${CXI_DRIVER_COMMIT}" | tee -a "$BUILD_LOG"
echo "  CXI libcxi Commit=${LIBCXI_COMMIT}" | tee -a "$BUILD_LOG"
echo "" | tee -a "$BUILD_LOG"

# Optional: on networks behind a TLS-inspecting proxy, set CORP_CA_FILE to the
# path of the corporate/internal root CA (PEM). It is passed in as a BuildKit/
# Buildah secret and is only mounted into the specific RUN steps that need it
# during the build - it is never copied into the image or committed to any layer.
SECRET_ARGS=()
if [ -n "${CORP_CA_FILE:-}" ]; then
    if [ ! -f "$CORP_CA_FILE" ]; then
        echo "Error: CORP_CA_FILE is set but not found: $CORP_CA_FILE"
        exit 1
    fi
    echo "Using corporate root CA from CORP_CA_FILE=${CORP_CA_FILE} (build-time only)" | tee -a "$BUILD_LOG"
    SECRET_ARGS=(--secret "id=corp_ca,src=${CORP_CA_FILE}")
fi

"${DOCKER_CMD}" build --progress plain -t "$IMAGE_TAG" -f "$CONTAINERFILE" \
    --build-arg LIBFABRIC_VERSION="$LIBFABRIC_VERSION" \
    --build-arg CHAPEL_VERSION="$CHAPEL_VERSION" \
    --build-arg CXI_VERSION="$CXI_VERSION" \
    --build-arg CXI_DRIVER_COMMIT="$CXI_DRIVER_COMMIT" \
    --build-arg LIBCXI_COMMIT="$LIBCXI_COMMIT" \
    "${SECRET_ARGS[@]}" \
    . 2>&1 | tee -a "$BUILD_LOG"

BUILD_EXIT_CODE=${PIPESTATUS[0]}
echo "Build completed at: $(date)" | tee -a "$BUILD_LOG"
echo "Final disk space: $(df -h . | tail -1 | awk '{print $4}')" | tee -a "$BUILD_LOG"
if [ $BUILD_EXIT_CODE -ne 0 ]; then
    echo "Build failed (${DOCKER_CMD}) with exit code: $BUILD_EXIT_CODE" | tee -a "$BUILD_LOG"
    exit 1
fi
