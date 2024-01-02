#!/usr/bin/env bash
#
# Update copyright year in source file header comments.
# Excludes third-party and test directories.

set -e

EXCLUDE_PATHS="':!third-party/' ':!test/'"

# Read in and validate input years
if [[ $# -ne 1 ]]; then
  echo "Usage: $0 [previous_year] [current_year]"
  exit 1
fi
PREVIOUS_YEAR=$1
CURRENT_YEAR=$2
if [ "$CURRENT_YEAR" != `date '+%Y'`]; then
  echo "Error: Specified current year $CURRENT_YEAR does not appear to be the actual current year"
  exit 1
fi
if [ $(PREVIOUS_YEAR + 1) != $(CURRENT_YEAR)]; then
  echo "Error: Current year $CURRENT_YEAR is not exactly 1 greater than previous year $PREVIOUS_YEAR"
  exit 1
fi

# Work from $CHPL_HOME for exclude dir paths
if [[ ! -v $CHPL_HOME]]; then
  echo "CHPL_HOME not set"
  exit 1
fi
echo "Using CHPL_HOME of $CHPL_HOME"
cd $CHPL_HOME

echo "Updating latest copyright year from $PREVIOUS_YEAR to $CURRENT_YEAR..."

# Make the update for each variation of how our copyright notice is written.

# Case: "Copyright 202x-[previous year]"->"Copyright 202x-[current year]"
git grep -l "Copyright 202[0-9]-$PREVIOUS_YEAR" -- $EXCLUDE_PATHS | xargs sed -i '' -e "s/Copyright (202[0-9])-$PREVIOUS_YEAR/Copyright \1-$CURRENT_YEAR/g"
# Case: "Copyright [previous year]"->"Copyright [previous year]-[current year]"
git grep -l "Copyright $PREVIOUS_YEAR" -- $EXCLUDE_PATHS | xargs sed -i '' -e "s/Copyright $PREVIOUS_YEAR/Copyright $PREVIOUS_YEAR-$CURRENT_YEAR/g"
