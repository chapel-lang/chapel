#!/bin/bash
# Generate e4s-cl profile commands for Chapel/Arkouda library dependencies
# This script detects HPC libraries needed by Chapel programs and outputs
# e4s-cl commands to add them to the current profile

set -e

echo "# E4S-CL Profile Commands for Chapel/Arkouda Dependencies"
echo "# Generated on $(date)"
echo "# Run these commands to add libraries to your e4s-cl profile:"
echo ""

# Function to check if file exists and output e4s-cl command
add_to_profile() {
    local file_path="$1"
    local description="$2"

    if [ -e "$file_path" ]; then
        echo "# $description"
        echo "e4s-cl profile edit --add-libraries \"$file_path\""
        return 0
    else
        echo "# $description - NOT FOUND: $file_path"
        return 1
    fi
}

# Function to check directory and output e4s-cl command
add_dir_to_profile() {
    local dir_path="$1"
    local description="$2"

    if [ -e "$dir_path" ]; then
        echo "# $description"
        echo "e4s-cl profile edit --add-files \"$dir_path\""
        return 0
    else
        echo "# $description - NOT FOUND: $dir_path"
        return 1
    fi
}

echo "# === Network Communication Libraries ==="

# Cray libfabric (critical for OFI)
LIBFABRIC_PATH=$(ls /opt/cray/libfabric/*/lib*/libfabric.so.1 2>/dev/null | sort -V | tail -1)
if [ -n "$LIBFABRIC_PATH" ]; then
    add_to_profile "$LIBFABRIC_PATH" "Cray libfabric for OFI networking"
    # Also add the lib directory
    LIBFABRIC_DIR=$(dirname "$LIBFABRIC_PATH")
    add_dir_to_profile "$LIBFABRIC_DIR" "Cray libfabric library directory"
fi

# CXI library for Slingshot network
CXI_PATH=$(ls /usr/lib*/libcxi.so.1 2>/dev/null | sort -V | tail -1)
if [ -z "$CXI_PATH" ]; then
    CXI_PATH=$(ls /usr/lib*/libcxi.so 2>/dev/null | sort -V | tail -1)
fi
if [ -n "$CXI_PATH" ]; then
    add_to_profile "$CXI_PATH" "CXI library for Slingshot networking"
fi

# Netlink library
NETLINK_PATH=$(ls /usr/lib*/libnl-3.so.200 2>/dev/null | sort -V | tail -1)
if [ -z "$NETLINK_PATH" ]; then
    NETLINK_PATH=$(ls /usr/lib*/libnl-3.so 2>/dev/null | sort -V | tail -1)
fi
if [ -n "$NETLINK_PATH" ]; then
    add_to_profile "$NETLINK_PATH" "Netlink library for network configuration"
fi

echo ""
echo "# === Process Management Interface (PMI) Libraries ==="

# PMI2 libraries - find the newest version available
PMI_DIR=$(ls -d /opt/cray/pe/pmi/*/lib 2>/dev/null | sort -V | tail -1)
if [ -n "$PMI_DIR" ] && [ -f "$PMI_DIR/libpmi2.so.0.6.0" ]; then
    add_dir_to_profile "$PMI_DIR" "PMI library directory (newest version)"
    add_to_profile "$PMI_DIR/libpmi2.so.0.6.0" "PMI2 library"
    add_to_profile "$PMI_DIR/libpmi2.so.0" "PMI2 library (symlink)"
fi

echo ""
echo "# === SLURM System Integration ==="

# SLURM configuration directory
add_dir_to_profile "/etc/slurm" "SLURM configuration directory"

# Munge authentication
add_dir_to_profile "/run/munge" "Munge authentication socket directory"

# SLURM library directory
add_dir_to_profile "/usr/lib64/slurm" "SLURM library directory"

# SLURM spool directory
add_dir_to_profile "/var/spool/slurm" "SLURM spool directory"

echo ""
echo "# === Cray Programming Environment ==="
add_dir_to_profile "/opt/cray" "Cray Programming Environment root"

echo ""
echo "# === Usage Instructions ==="
echo "# 1. Review the commands above"
echo "# 2. Execute the ones for libraries/directories that exist on your system"
echo "# 3. Verify with: e4s-cl profile show"
echo "# 4. Test with your Chapel/Arkouda applications"
echo ""
echo "# Note: You can also run all successful commands at once by filtering:"
echo "# $0 | grep '^e4s-cl profile edit' | bash"
