#!/bin/bash

# Arguments are versions that are allowed, e.g. 10.0 11.0
# Looks for an installed LLVM with the same major version.

command_exists()
{
  command -v "$1" >/dev/null 2>&1
}

ARGS="$*"
BAD=""
BAD_VERSION=""
FOUND=""
ALLOW_VERS=""

# Compute a human-readable set of versions separated by " or "
for v in $ARGS
do
  ALLOW_VERS="$ALLOW_VERS or $v"
done
ALLOW_VERS=${ALLOW_VERS## or }

# Print usage and stop if no version argument was provided
if [ "$ALLOW_VERS" = "" ]
then
  echo "Usage: $0 <versions>"
  exit 1
fi

for arg in $ARGS
do
  PREFERRED_VERSION=$arg
  PREFERRED_VERSION_MAJOR=`echo $arg | cut -d. -f1`

  # Check a bunch of paths and names for llvm-config
  # The /usr/local/opt ones are indended to support Homebrew
  for t in "llvm-config-$PREFERRED_VERSION" \
           "llvm-config-$PREFERRED_VERSION_MAJOR" \
           "llvm-config" \
           "/usr/local/opt/llvm@$PREFERRED_VERSION/bin/llvm-config" \
           "/usr/local/opt/llvm@$PREFERRED_VERSION_MAJOR/bin/llvm-config" \
           "/usr/local/opt/llvm/bin/llvm-config"
  do
    if command_exists "$t"
    then
      FOUND=`command -v "$t"`
      # Check that the version found is compatible
      FOUND_VERSION=`$FOUND --version`
      FOUND_VERSION_MAJOR=`echo $FOUND_VERSION | cut -d. -f1`
      if [ "$PREFERRED_VERSION_MAJOR" = "$FOUND_VERSION_MAJOR" ]
      then
        break
      else
        # version check failed, so don't consider this one
        # we can use BAD when reporting an error.
        BAD_VERSION="$FOUND_VERSION"
        BAD="$FOUND"
        FOUND=""
      fi
    fi
  done

  if [ "$FOUND" != "" ]
  then
    break
  fi
done

if [ "$FOUND" != "" ]
then
  echo "$FOUND"
else
  echo "Could not find an installed LLVM with version $ALLOW_VERS" 1>&2
  if [ "$BAD" != "" ]
  then
    echo "Found version $BAD_VERSION at path $BAD" 1>&2
    echo "Please install an LLVM with version $ALLOW_VERS" 1>&2
    echo "or set CHPL_LLVM=bundled to use the included LLVM" 1>&2
  fi
  echo missing-llvm-config
fi
