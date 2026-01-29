#!/bin/bash

# Generate a "bottle do..." block with bogus SHA256 hashes, to work around
# homebrew behavior of not erroring on failures for un-bottled platforms
# (https://github.com/Homebrew/homebrew-test-bot/issues/791).

# Have to unset this (just within this script) so `brew info` queries the API
HOMEBREW_NO_INSTALL_FROM_API=

# Run once before capturing output, due to initial run having output for
# downloading metadata.
brew info &> /dev/null

platforms=$(brew info --json=v1 chapel | jq ".[0].bottle.stable.files | keys[]" -r)

max_length=$(echo "$platforms" | awk ' { if ( length > x ) { x = length; y = $0 } }END{ print x+1 }')

echo "$platforms" |
  while read -r platform; do
    # hash platform name to get a real SHA which would not match the binary
    namehash=$(echo $platform | sha256sum | cut -d" " -f1)
    # print platforms with padding to align the sha256 hashes
    printf "    sha256 %s \"%s\"\n" "$(printf "%-${max_length}s" "$platform:")" "$namehash"
  done
echo "  end"
