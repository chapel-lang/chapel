# Copyright © 2012-2020 Inria.  All rights reserved.
# See COPYING in top-level directory.


# HWLOC_PREPARE_FILTER_COMPONENTS
#
# Given a list of names, define hwloc_<name>_component_maybeplugin=1.
#
# $1 = space-separated list of components to build as plugins
#
AC_DEFUN([HWLOC_PREPARE_FILTER_COMPONENTS], [
  for name in [$1] ; do
    str="hwloc_${name}_component_wantplugin=1"
    eval $str
  done
])


# HWLOC_FILTER_COMPONENTS
#
# For each component in hwloc_components,
# check if hwloc_<name>_component_wantplugin=1,
# and check if hwloc_<name>_component_maybeplugin=1.
# Add <name> to hwloc_[static|plugin]_components accordingly.
# And set hwloc_<name>_component=[static|plugin] accordingly.
#
AC_DEFUN([HWLOC_FILTER_COMPONENTS], [
for name in $hwloc_components ; do
  str="maybeplugin=\$hwloc_${name}_component_maybeplugin"
  eval $str
  str="wantplugin=\$hwloc_${name}_component_wantplugin"
  eval $str
  if test x$hwloc_have_plugins = xyes -a x$maybeplugin = x1 -a x$wantplugin = x1; then
    hwloc_plugin_components="$hwloc_plugin_components $name"
    str="hwloc_${name}_component=plugin"
  else
    hwloc_static_components="$hwloc_static_components $name"
    str="hwloc_${name}_component=static"
  fi
  eval $str
done
])


# HWLOC_LIST_STATIC_COMPONENTS
#
# Append to file $1 an array of components by listing component names in $2.
#
# $1 = filename
# $2 = list of component names
#
AC_DEFUN([HWLOC_LIST_STATIC_COMPONENTS], [
cat <<EOF >>[$1]
#include <private/internal-components.h>
static const struct hwloc_component * hwloc_static_components[[]] = {
EOF
for comp in [$2]; do
  echo "  &hwloc_${comp}_component," >>[$1]
done
cat <<EOF >>[$1]
  NULL
};
EOF
])
