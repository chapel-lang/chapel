#!/bin/sh

# takes in a single argument: the preferred version, e.g. 3.7
PREFERRED_VERSION=$1
PREFERRED_VERSION_MAJOR=`echo $1 | cut -d. -f1`

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
# If llvm-config is not found by now, search the Mac Homebrew directories.
elif command_exists /usr/local/opt/llvm@$PREFERRED_VERSION/bin/llvm-config
then
  command -v /usr/local/opt/llvm@$PREFERRED_VERSION/bin/llvm-config
elif command_exists /usr/local/opt/llvm@$PREFERRED_VERSION_MAJOR/bin/llvm-config
then
  command -v /usr/local/opt/llvm@$PREFERRED_VERSION_MAJOR/bin/llvm-config
elif command_exists /usr/local/opt/llvm/bin/llvm-config
then
  command -v /usr/local/opt/llvm/bin/llvm-config
else
  echo missing-llvm-config
fi
