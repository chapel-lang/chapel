#!/usr/bin/env bash

#
# We need to ensure that the Programming Environment is set up to use
# the Gnu compiler.  This is complicated by the fact that PrgEnv-gnu
# could be either a meta-module or a collection.
#
if module avail PrgEnv- 2>&1 | grep --quiet PrgEnv- ; then
  # We have PrgEnv-* meta-modules.
  existing_prgenv=$(module -t list 2>&1 | grep PrgEnv-)
  if [ -z "${existing_prgenv}" ]
  then
    # No PrgEnv loaded, load PrgEnv-gnu
    module load PrgEnv-gnu 2>/dev/null
  elif [[ "${existing_prgenv}" != PrgEnv-gnu* ]]
  then
    # Replace current PrgEnv with PrgEnv-gnu
    module unload "${existing_prgenv}" 2>/dev/null
    module load PrgEnv-gnu 2>/dev/null
  fi
elif module savelist 2>&1 | grep --quiet PrgEnv-gnu ; then
  # We have a PrgEnv-gnu collection.
  #
  # The 'restore' will unload our Chapel module, so we have to preserve
  # CHPL_HOME, which we'll need below.
  save_CHPL_HOME=$CHPL_HOME
  module restore PrgEnv-gnu &> /dev/null
  export CHPL_HOME=$save_CHPL_HOME
else
  # No PrgEnv-* meta-modules are available.  We can't tell the compiler
  # what to link.
  true # do nothing here
fi

# Now run the command again
exec "$1" "${@:2}"
