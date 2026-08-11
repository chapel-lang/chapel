#!/bin/bash
# Convert podman container images to Apptainer SIF format
# Usage: ./convert-to-sif.sh <container_uri> [options]

set -e

# Configuration
KEEP_ARCHIVE="${KEEP_ARCHIVE:-false}"
OUTPUT_DIR="${OUTPUT_DIR:-$(pwd)}"
FORCE_OVERWRITE="${FORCE_OVERWRITE:-false}"

# Help function
show_help() {
    cat << 'EOF_HELP'
Usage: $0 <container_uri> [options]

Convert podman container images to Apptainer SIF format via OCI archive.

Arguments:
    container_uri    Container image URI (e.g., docker.io/ubuntu:22.04, localhost/myimage:latest)

Options:
    -k, --keep-archive    Keep the intermediate OCI archive file (default: false)
    -o, --output-dir DIR  Output directory for files (default: current directory)
    -f, --filename NAME   Custom filename (without extension) for output files
    -y, --force           Overwrite existing SIF output without prompting
    -h, --help           Show this help message

Environment Variables:
    KEEP_ARCHIVE         Keep intermediate files (true/false)
    OUTPUT_DIR           Default output directory
    FORCE_OVERWRITE      Overwrite existing SIF output without prompting (true/false)

Examples:
    $0 docker.io/ubuntu:22.04
    $0 localhost/myapp:v1.0 --keep-archive
    $0 registry.io/user/app:latest -o /tmp
    $0 docker.io/ubuntu:22.04 --filename my-ubuntu
    $0 localhost/myapp:v1.0 --force
EOF_HELP
}

# Parse arguments
CONTAINER_URI=""
CUSTOM_FILENAME=""
while [[ $# -gt 0 ]]; do
    case $1 in
        -k|--keep-archive)
            KEEP_ARCHIVE="true"
            shift
            ;;
        -o|--output-dir)
            OUTPUT_DIR="$2"
            shift 2
            ;;
        -f|--filename)
            CUSTOM_FILENAME="$2"
            shift 2
            ;;
        -y|--force)
            FORCE_OVERWRITE="true"
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        -*)
            echo "Error: Unknown option $1"
            show_help
            exit 1
            ;;
        *)
            if [ -z "$CONTAINER_URI" ]; then
                CONTAINER_URI="$1"
            else
                echo "Error: Multiple container URIs provided"
                show_help
                exit 1
            fi
            shift
            ;;
    esac
done

# Validate arguments
if [ -z "$CONTAINER_URI" ]; then
    echo "Error: Container URI required"
    show_help
    exit 1
fi

# Validate tools
if ! command -v podman &> /dev/null; then
    echo "Error: podman not found"
    exit 1
fi

if ! command -v apptainer &> /dev/null; then
    echo "Error: apptainer not found"
    exit 1
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"
cd "$OUTPUT_DIR"

# Function to generate filename from container URI
generate_filename() {
    local uri="$1"
    local base_name

    # Remove registry prefix if present (keep last two parts)
    base_name=$(echo "$uri" | sed 's|.*/\([^/]*/[^/]*\)$|\1|' | sed 's|^[^/]*$|&|')

    # If no tag specified, add 'latest'
    if [[ ! "$base_name" == *:* ]]; then
        base_name="${base_name}:latest"
    fi

    # Replace : and / with -
    base_name=$(echo "$base_name" | sed 's/:/-/g' | sed 's/\//-/g')

    # Sanitize for filesystem
    echo "$base_name" | sed 's/[^a-zA-Z0-9._-]/_/g' | sed 's/__*/_/g' | sed 's/^_\|_$//g'
}

# Function to check if image exists locally
image_exists_locally() {
    local uri="$1"
    podman images --format "{{.Repository}}:{{.Tag}}" | grep -q "^${uri}$"
}

# Main conversion logic
echo "=== Container to SIF Converter ==="
echo "Container URI: $CONTAINER_URI"
echo "Output directory: $OUTPUT_DIR"
echo ""

# Generate filenames
if [ -n "$CUSTOM_FILENAME" ]; then
    # Use custom filename (sanitize it)
    BASE_NAME=$(echo "$CUSTOM_FILENAME" | sed 's/[^a-zA-Z0-9._-]/_/g' | sed 's/__*/_/g' | sed 's/^_\|_$//g')
else
    # Auto-generate from container URI
    BASE_NAME=$(generate_filename "$CONTAINER_URI")
fi
OCI_ARCHIVE="${BASE_NAME}-oci.tar"
SIF_FILE="${BASE_NAME}.sif"

echo "Generated filenames:"
echo "  OCI Archive: $OCI_ARCHIVE"
echo "  SIF File: $SIF_FILE"
echo ""

# Check if SIF already exists
if [ -f "$SIF_FILE" ]; then
    echo "Warning: SIF file already exists: $SIF_FILE"
    if [ "$FORCE_OVERWRITE" = "true" ]; then
        echo "Force enabled: overwriting existing SIF without prompt"
    else
        read -p "Overwrite? (y/N): " -n 1 -r
        echo ""
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            echo "Aborted."
            exit 0
        fi
    fi
    rm -f "$SIF_FILE"
fi

# Step 1: Check if image exists locally, pull if needed
echo "=== Step 1: Image Availability ==="
if image_exists_locally "$CONTAINER_URI"; then
    echo "Image found locally: $CONTAINER_URI"
else
    echo "Image not found locally. Pulling: $CONTAINER_URI"
    podman pull "$CONTAINER_URI"
    if [ $? -ne 0 ]; then
        echo "Error: Failed to pull image"
        exit 1
    fi
    echo "Image pulled successfully"
fi

# Step 2: Export to OCI archive
echo ""
echo "=== Step 2: Export to OCI Archive ==="
echo "Exporting to: $OCI_ARCHIVE"
podman save --format oci-archive -o "$OCI_ARCHIVE" "$CONTAINER_URI"
if [ $? -ne 0 ]; then
    echo "Error: Failed to create OCI archive"
    exit 1
fi
echo "OCI archive created: $OCI_ARCHIVE ($(ls -lh "$OCI_ARCHIVE" | awk '{print $5}'))"

# Step 3: Convert to SIF
echo ""
echo "=== Step 3: Convert to SIF ==="
echo "Converting to: $SIF_FILE"
apptainer build "$SIF_FILE" "oci-archive:$OCI_ARCHIVE"
if [ $? -ne 0 ]; then
    echo "Error: Failed to build SIF file"
    rm -f "$OCI_ARCHIVE"
    exit 1
fi
echo "SIF file created: $SIF_FILE ($(ls -lh "$SIF_FILE" | awk '{print $5}'))"

# Step 4: Cleanup
echo ""
echo "=== Step 4: Cleanup ==="
if [ "$KEEP_ARCHIVE" = "true" ]; then
    echo "Keeping OCI archive: $OCI_ARCHIVE"
else
    echo "Removing OCI archive: $OCI_ARCHIVE"
    rm -f "$OCI_ARCHIVE"
fi

# Success summary
echo ""
echo "=== Conversion Complete ==="
echo "SUCCESS: Container URI: $CONTAINER_URI"
echo "SUCCESS: SIF File: $(pwd)/$SIF_FILE"
if [ "$KEEP_ARCHIVE" = "true" ]; then
    echo "SUCCESS: OCI Archive: $(pwd)/$OCI_ARCHIVE"
fi

echo ""
echo "Usage: apptainer exec $SIF_FILE <command>"
echo "   or: apptainer shell $SIF_FILE"
