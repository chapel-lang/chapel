#!/bin/bash

# Generates a set_default script for Chapel Cray Module
# Writes to stdout

set -e

cwd=$( cd $(dirname "${BASH_SOURCE[0]}" ) && pwd )
source $cwd/../functions.bash

# Accept command line parameters of the form NAME=Value. Or use environment variables.

while test $# -gt 0; do
    case $1 in ( *=* ) export "$1" ;; esac
    shift
done

# Cray-internal-specific shell variables

source $cwd/common.bash

# Generate part 1 without shell expansion

cat <<\PART_1
#%Module
#
# Chapel module
#

if { [info exists env(CPU)] } {
    set CHPL_HOST_MACHINE $env(CPU)
} else {
    set CHPL_HOST_MACHINE "UNSET"
}

if { [ info exists env(CRAYPE_DIR) ]} {
    set latest_module $env(CRAYPE_DIR)
} elseif { [ file exists "/opt/cray/craype" ]} {
    set start_path "/opt/cray/craype"
} elseif { [ file exists "/opt/cray/pe/craype" ]} { # SLES12?
    set start_path "/opt/cray/pe/craype"
} elseif { [ file exists "/opt/cray/xt-asyncpe" ]} {
    set start_path "/opt/cray/xt-asyncpe"
} else {
    puts stderr "Either craype/1.05 or later or xt-asyncpe/5.21 or later  are required."
    exit 1
}

if { [ info exists start_path ]} {
    set available_modules [lsort [glob $start_path/*]]
    set latest_module [lindex $available_modules [expr [llength $available_modules] - 2]]
}

if { [file exists "$latest_module/admin/bin/modulefile-utils.tcl" ]} {
    source $latest_module/admin/bin/modulefile-utils.tcl
} else {
    puts stderr "Error: could not find modulefile-utils.tcl."
    puts stderr "Either craype/1.05 or later  or xt-asyncpe/5.21 or later are required."
    exit 1
}


proc ModulesHelp { } {
    puts stderr "This modulefile defines the system paths and"
    puts stderr "environment variables needed to use "
    puts stderr "the Chapel compiler on XT. This module "
    puts stderr "requires a PrgEnv environment to be loaded."
}

conflicts cray-mpich2 < 7.0.0
conflicts xt-mpich2 < 6.0.0
conflict chapel
#conflict gcc

if { [string match aarch64 $CHPL_HOST_MACHINE] } {
    # ARM-based CPU, 2018-06-08
} elseif { [string match x86_64 $CHPL_HOST_MACHINE] } {
    # Legacy Cray-XC/XE
    module load cray-mpich
} else {
    puts stderr "Error: CPU=$cpu"
    exit 1
}

if { ! [ info exists env(PE_ENV) ] } {
    module load PrgEnv-gnu
}

##
## Now, we're going to attempt to get to the right build type
###
#
set network seastar
if { [ info exists env(XTPE_NETWORK_TARGET) ] } {
    set network $env(XTPE_NETWORK_TARGET)
} elseif { [ info exists env(CRAYPE_NETWORK_TARGET) ] } {
    set network $env(CRAYPE_NETWORK_TARGET)
}
set accel 0
if { [ info exists env(CRAY_ACCEL_TARGET) ] } {
    set accel 1
}


setenv MPICH_GNI_DYNAMIC_CONN disabled
if { [string match gemini $network] } {
    set CHPL_HOST_PLATFORM cray-xe
} elseif { [string match aries $network] } {
    set CHPL_HOST_PLATFORM cray-xc
} else {
    set CHPL_HOST_PLATFORM cray-xt
}

set BASE_INSTALL_DIR    [BASE_INSTALL_DIR]
PART_1

# Generate part 2 With shell expansion

cat <<PART_2
set CHPL_LEVEL          $pkg_version
PART_2

# Generate part 3 without shell expansion

cat <<\PART_3
set CHPL_LOC            $BASE_INSTALL_DIR/chapel/$CHPL_LEVEL/$CHPL_HOST_PLATFORM
set is_module_rm        [module-info mode remove]

if { [ file exists $CHPL_LOC/release_info ] } {
    set REL_INFO [ exec cat $CHPL_LOC/release_info ]
} else {
    set REL_INFO ""
}

# The default comm layer for Chapel version 1.11+ is ugni, which requires a
# craype-hugepages module in order to link correctly. If CHPL_COMM is not set
# in the environment or is set to ugni, make sure there is a craype-hugepages
# module loaded. Use craype-hugepages16M if a craype-hugepages module is not
# already loaded.
set compiler $env(PE_ENV)
if { [info exists env(CHPL_COMM)] } {
    set chpl_comm $env(CHPL_COMM)
} else {
    set chpl_comm "UNSET"
}

set hugepagesLoaded [string match "*HUGETLB*" $env(PE_PRODUCT_LIST)]
# Logic for hugepages is split into loading and unloading phases
if { !([is-loaded chapel] == 1) }  {
    # Loading chapel

    if {$hugepagesLoaded} {
        setenv CHPL_MODULE_KEEP_HUGEPAGES 1
    }

    # Check to see if we require hugepages
    if { ([string equal UNSET $chpl_comm] || [string equal ugni $chpl_comm]) &&
         ([string equal -nocase GNU $compiler] || [string equal -nocase INTEL $compiler] || [string equal -nocase CRAY $compiler]) } {

        if {! $hugepagesLoaded} {
            module load craype-hugepages16M
        }
    }
} else {
    # Unloading or reloading chapel

    if {$hugepagesLoaded} {
        # Was hugepages already loaded before we loaded chapel?
        if {[ info exists env(CHPL_MODULE_KEEP_HUGEPAGES)]} {
            # When we are unloading, this actually unsets CHPL_MODULE_KEEP_HUGEPAGES
            setenv CHPL_MODULE_KEEP_HUGEPAGES 1
        } else {
            # When we are unloading, this actually unloads craype-hugepages16M
            module load craype-hugepages16M
        }
    }
}

proc ModulesHelp { } {
    global REL_INFO
    global CHPL_LOC
    global CHPL_LEVEL
    puts stderr "$REL_INFO"
    puts stderr "==================================================================\n"
    puts stderr "To re-display chapel/$CHPL_LEVEL release information,"
    puts stderr "type:    less $CHPL_LOC/release_info"
    puts stderr "==================================================================\n"
}

setenv CHPL_HOME        $CHPL_LOC
setenv CHPL_MODULE_HOME $CHPL_LOC
prepend-path PATH       ${CHPL_LOC}/bin/${CHPL_HOST_PLATFORM}-${CHPL_HOST_MACHINE}
prepend-path MANPATH    ${CHPL_LOC}/man
append-path             PE_PRODUCT_LIST CHAPEL
PART_3
