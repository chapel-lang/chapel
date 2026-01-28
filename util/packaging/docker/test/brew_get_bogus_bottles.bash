#!/bin/bash

platforms=$(brew info --json=v1 chapel | jq ".[0].bottle.stable.files | keys[]" -r)

max_length=$(echo "$platforms" | awk ' { if ( length > x ) { x = length; y = $0 } }END{ print x+1 }')

echo "  bottle do"
echo "$platforms" |
  while read -r platform; do
    sha256=$(echo $platform | sha256sum | cut -d" " -f1)
    # print platforms with padding to align the sha256 hashes
    printf "    sha256 %s \"%s\"\n" "$(printf "%-${max_length}s" "$platform:")" "$sha256"
  done
echo "  end"
