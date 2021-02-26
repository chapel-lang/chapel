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

# Generate the first part of the file with shell expansion

if [ "$chpl_platform" = hpe-cray-ex ]; then
    platform_prefix=/opt/cray
else
    platform_prefix=/opt
fi

cat <<PART_1
#!/bin/bash

export CRAY_product=chapel
export CRAY_version=$pkg_version
export CRAY_inst_dir=$platform_prefix
export CRAY_mod_dir=$platform_prefix/modulefiles
export CRAY_mod_names=chapel

PART_1

# Generate the rest of the file, without shell expansion

cat <<\PART_2
if [ -L /opt/cray/bin/set_default ] ; then
  /opt/cray/bin/set_default "$@"
elif [ -L /opt/cray/pe/bin/set_default ] ; then
  /opt/cray/pe/bin/set_default "$@"
else
  echo "The set_default script is not installed in /opt/cray/bin,"
  echo "or /opt/cray/pe/bin. CrayPE must be installed."
  exit 1
fi
PART_2

exit

#   Original file snippet, for info

##  # Builder: You must edit this script to define:
##  #    product_name= product name as it appears in /opti, /opt/cray,
##  #    or /opt/cray/pe.
##  #    version_string= version being installed
##  #    install_dir= either /opt, /opt/cray, or /opt/cray/pe
##  #    module_dir= either /opt/modulefiles, /opt/cray/modulefiles, or
##  #    /opt/cray/pe/modulefiles
##  #    mod_names= list of names of modulefiles for this install
##  #    i.e. ="cray-mpich cray-shmem"
##  #
##  #    Save this file in /opt/cray/2.4.0 as
##  #    set_default_craype_2.4.0
##  #    You can invoke your finished script with "--help" or with
##  #    "-cray_links_only"
##
##  export CRAY_product=[product_name]
##  export CRAY_version=[version_string]
##  export CRAY_inst_dir=[install_dir]
##  export CRAY_mod_dir=[module_dir]
##  export CRAY_mod_names=[module_name_list]
