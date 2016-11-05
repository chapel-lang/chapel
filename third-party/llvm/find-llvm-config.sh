#!/bin/sh

# takes in a single argument: the preferred version, e.g. 3.7
PREFERRED_VERSION=$1

command_exists()
{
  command -v "$1" >/dev/null 2>&1
}

if command_exists llvm-config-$PREFERRED_VERSION
then
  command -v llvm-config-$PREFERRED_VERSION
elif command_exists llvm-config
then
  command -v llvm-config
else
  echo missing-llvm-config
fi
