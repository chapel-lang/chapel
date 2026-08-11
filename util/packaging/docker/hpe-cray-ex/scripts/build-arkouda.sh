#!/bin/bash

# Build script for Arkouda on pre-built Chapel container
# =====================================================

set -e

# Resolve this project's root directory regardless of the caller's cwd
# (build context for both target Containerfiles is always this directory,
# regardless of where the project itself lives on disk).
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CONTAINER_DIR="$(dirname "$SCRIPT_DIR")"

# Default values
CHAPEL_BASE_IMAGE="localhost/chapel-2.9.0-libfabric-2.3.1-cxi-pic:latest"
ARKOUDA_VERSION="2026.07.15"
LIBICONV_VERSION="1.17"
ARROW_VERSION="19.0.1-1"
IMAGE_TAG=""
BUILD_ARGS=()
LOG_FILE="build-arkouda-$(date +%Y%m%d-%H%M%S).log"
VERBOSE=false

# Auto-detect container CLI: prefer docker, fall back to podman.
# Override anytime with -d/--docker-cmd or the DOCKER_CMD env var.
if [ -n "${DOCKER_CMD:-}" ]; then
    :
elif command -v docker >/dev/null 2>&1; then
    DOCKER_CMD="docker"
elif command -v podman >/dev/null 2>&1; then
    DOCKER_CMD="podman"
else
    DOCKER_CMD="docker"
fi

# Function to show usage
usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Build Arkouda container using pre-built Chapel base image.

OPTIONS:
    -b, --base-image IMAGE       Chapel base image (default: ${CHAPEL_BASE_IMAGE})
    -v, --arkouda-version VER    Arkouda version to build (default: ${ARKOUDA_VERSION})
        --libiconv-version VER   GNU libiconv version (default: ${LIBICONV_VERSION})
        --arrow-version VER      Arrow/Parquet version (default: ${ARROW_VERSION})
    -t, --tag TAG                Output image tag (default: auto-generated)
    -a, --build-arg ARG          Additional build argument (can be used multiple times)
    -d, --docker-cmd CMD         Container CLI to use (default: auto-detected; tries
                                  'docker' first, falls back to 'podman'; currently: ${DOCKER_CMD})
    -l, --log-file FILE          Build log file (default: ${LOG_FILE})
    -V, --verbose                Enable verbose output
    -h, --help                   Show this help message

EXAMPLES:
    # Build with default settings
    $0

    # Build with custom Chapel base image
    $0 --base-image arezaiihpe/chapel-2.8.0-libfabric-2.3.1-cxi:latest

    # Build specific Arkouda version
    $0 --arkouda-version 2026.02.27

    # Build with custom tag
    $0 --tag my-arkouda:latest

    # Use podman instead of docker
    $0 --docker-cmd podman

EOF
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -b|--base-image)
            CHAPEL_BASE_IMAGE="$2"
            shift 2
            ;;
        -v|--arkouda-version)
            ARKOUDA_VERSION="$2"
            shift 2
            ;;
        --libiconv-version)
            LIBICONV_VERSION="$2"
            shift 2
            ;;
        --arrow-version)
            ARROW_VERSION="$2"
            shift 2
            ;;
        -t|--tag)
            IMAGE_TAG="$2"
            shift 2
            ;;
        -a|--build-arg)
            BUILD_ARGS+=(--build-arg "$2")
            shift 2
            ;;
        -d|--docker-cmd)
            DOCKER_CMD="$2"
            shift 2
            ;;
        -l|--log-file)
            LOG_FILE="$2"
            shift 2
            ;;
        -V|--verbose)
            VERBOSE=true
            shift
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo "Error: Unknown option $1"
            usage
            exit 1
            ;;
    esac
done

# Auto-generate image tag if not set
if [[ -z "${IMAGE_TAG}" ]]; then
    IMAGE_TAG="arkouda-${ARKOUDA_VERSION}-cxi:latest"
fi

# Verify Chapel base image exists
echo "Checking if Chapel base image exists: ${CHAPEL_BASE_IMAGE}"
if ! ${DOCKER_CMD} image inspect "${CHAPEL_BASE_IMAGE}" >/dev/null 2>&1; then
    echo "Error: Chapel base image '${CHAPEL_BASE_IMAGE}' not found!"
    echo "Please build the Chapel container first or specify a different base image."
    exit 1
fi

echo "Checking if Chapel Python bindings are available in base image"
if ! ${DOCKER_CMD} run --rm "${CHAPEL_BASE_IMAGE}" \
    bash -lc 'python3 -c "import chapel"' >/dev/null 2>&1; then
    echo "Error: Chapel base image '${CHAPEL_BASE_IMAGE}' does not expose the Python 'chapel' module."
    echo "Rebuild the Chapel base image with ./scripts/build-chapel-dist-cxi-2.3.1-pic.sh or use a newer image tag."
    exit 1
fi

# Setup logging (always under the shared repo-root build-logs/ directory)
mkdir -p "${CONTAINER_DIR}/build-logs"
LOG_PATH="${CONTAINER_DIR}/build-logs/${LOG_FILE}"
echo "Build log will be written to: ${LOG_PATH}"
echo

# Function for timestamped logging
log_with_timestamp() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1" | tee -a "${LOG_PATH}"
}

# Function to show progress
show_progress() {
    if [ "$VERBOSE" = true ]; then
        cat
    else
        local line_count=0
        while IFS= read -r line; do
            line_count=$((line_count + 1))
            if [ $((line_count % 10)) -eq 0 ]; then
                echo -n "."
            fi
            echo "$line"
        done
        echo
    fi
}

log_with_timestamp "Starting Arkouda container build"
log_with_timestamp "Chapel base image:  ${CHAPEL_BASE_IMAGE}"
log_with_timestamp "Arkouda version:    ${ARKOUDA_VERSION}"
log_with_timestamp "libiconv version:   ${LIBICONV_VERSION}"
log_with_timestamp "Arrow version:      ${ARROW_VERSION}"
log_with_timestamp "Output tag:         ${IMAGE_TAG}"
log_with_timestamp "Docker command:     ${DOCKER_CMD}"
log_with_timestamp "Verbose mode:       ${VERBOSE}"
echo

# Change to the project directory, which is the build context for the Containerfile
log_with_timestamp "Changing to build directory: ${CONTAINER_DIR}"
cd "$CONTAINER_DIR"

# Verify Containerfile exists
if [ ! -f "containers/Containerfile.arkouda" ]; then
    log_with_timestamp "Error: Containerfile not found: containers/Containerfile.arkouda"
    exit 1
fi

# Optional: on networks behind a TLS-inspecting proxy, set CORP_CA_FILE to the
# path of the corporate/internal root CA (PEM). It is passed in as a BuildKit/
# Buildah secret and is only mounted into the specific RUN steps that need it
# during the build - it is never copied into the image or committed to any layer.
SECRET_ARGS=()
if [ -n "${CORP_CA_FILE:-}" ]; then
    if [ ! -f "$CORP_CA_FILE" ]; then
        log_with_timestamp "Error: CORP_CA_FILE is set but not found: $CORP_CA_FILE"
        exit 1
    fi
    log_with_timestamp "Using corporate root CA from CORP_CA_FILE=${CORP_CA_FILE} (build-time only)"
    SECRET_ARGS=(--secret "id=corp_ca,src=${CORP_CA_FILE}")
fi

# Build the container with comprehensive logging. Built as an array (rather
# than an interpolated string run through eval) so that values containing
# spaces or shell metacharacters can't be re-parsed/re-executed by the shell.
BUILD_CMD=(
    "${DOCKER_CMD}" build
    --progress=plain
    --file containers/Containerfile.arkouda
    --build-arg "CHAPEL_BASE_IMAGE=${CHAPEL_BASE_IMAGE}"
    --build-arg "ARKOUDA_VERSION=${ARKOUDA_VERSION}"
    --build-arg "LIBICONV_VERSION=${LIBICONV_VERSION}"
    --build-arg "ARROW_VERSION=${ARROW_VERSION}"
    "${BUILD_ARGS[@]}"
    "${SECRET_ARGS[@]}"
    --tag "${IMAGE_TAG}"
    .
)

log_with_timestamp "Build command: $(printf '%q ' "${BUILD_CMD[@]}")"
log_with_timestamp "Starting container build..."
echo

if [ "$VERBOSE" = true ]; then
    echo "Build output (verbose mode):"
    echo "============================="
else
    echo "Building container (use --verbose for detailed output)..."
    echo -n "Progress: "
fi

# Execute build with full logging
"${BUILD_CMD[@]}" 2>&1 | tee -a "${LOG_PATH}" | show_progress
BUILD_EXIT_CODE=${PIPESTATUS[0]}

echo
if [ $BUILD_EXIT_CODE -eq 0 ]; then
    log_with_timestamp "Build completed successfully!"
    echo "Image tagged as: ${IMAGE_TAG}"
    echo "Build log saved to: ${LOG_PATH}"
    echo
    echo "To run the container:"
    echo "  # Interactive shell"
    echo "  ${DOCKER_CMD} run --rm -it ${IMAGE_TAG}"
    echo
    echo "  # Run Arkouda server (single-locale)"
    echo "  ${DOCKER_CMD} run -it --rm --init ${IMAGE_TAG} arkouda_server"
    echo
    echo "  # Server in one container (published port) + client in a second"
    echo "  # container connecting via host.docker.internal - see"
    echo "  # 'Server + separate client container' in docs/README.md:"
    echo "  ${DOCKER_CMD} run --rm --init --name arkouda-server -p 5555:5555 \\"
    echo "    ${IMAGE_TAG} arkouda_server"
    echo "  ${DOCKER_CMD} run --rm -it --add-host=host.docker.internal:host-gateway \\"
    echo "    ${IMAGE_TAG}"
    echo
else
    log_with_timestamp "Build failed with exit code: $BUILD_EXIT_CODE"
    echo
    echo "=================="
    echo "See the log to investigate further"
    echo "Build log location: ${LOG_PATH}"
    echo
    echo "Last 20 lines from build log:"
    echo "------------------------------"
    tail -20 "${LOG_PATH}" | sed 's/^/  /'
    echo
    exit 1
fi
