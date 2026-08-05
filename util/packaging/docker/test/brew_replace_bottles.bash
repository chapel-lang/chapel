#!/usr/bin/env bash

# Replace a Homebrew formula's existing "bottle do ... end" block with the
# block supplied on standard input.

set -euo pipefail

if [[ $# -ne 1 ]]; then
  echo "Usage: $0 FORMULA < replacement-block" >&2
  exit 2
fi

formula=$1
if [[ ! -f $formula ]]; then
  echo "Formula not found: $formula" >&2
  exit 1
fi

replacement="new-bottle-block.tmp"
cat > "$replacement"
if [[ ! -s $replacement ]]; then
  echo "Replacement bottle block is empty" >&2
  rm -f "$replacement"
  exit 1
fi

updated="$1.updated.tmp"
awk -v replacement="$replacement" '
  !replaced && /^[[:space:]]*bottle[[:space:]]+do[[:space:]]*$/ {
    while ((getline line < replacement) > 0) print line
    close(replacement)
    replaced = 1
    skipping = 1
    next
  }
  skipping {
    if (/^[[:space:]]*end[[:space:]]*$/) skipping = 0
    next
  }
  { print }
  END {
    if (!replaced || skipping) exit 1
  }
' "$formula" > "$updated" || {
  echo "Could not find a complete bottle block in $formula" >&2
  rm -f "$updated" "$replacement"
  exit 1
}
mv "$updated" "$formula"
