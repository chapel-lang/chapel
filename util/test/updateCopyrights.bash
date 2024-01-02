#!/usr/bin/env bash
#
# Update copyright year in source file header comments.
# Excludes third-party and test directories.

set -e

# Paths to be ignored for update by 'git grep'
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


# Require working from clean repo for safety.
if [ ! -z `git status -u --porcelain` ]; then
  echo "Refusing to run from unclean working directory for safety"
  exit 1
fi

# Make updates.
echo "Updating latest copyright year from $PREVIOUS_YEAR to $CURRENT_YEAR..."

# Do replacement for a pattern.
# First arg is 'git grep' search pattern, second arg is 'sed' find/replace pattern.
function tryReplacementForPattern {
  SEARCH_PATTERN=$1
  SED_PATTERN=$2

  # Error if a pattern has no matches (and therefore wouldn't result in any updates).
  if ! `git grep -l $SEARCH_PATTERN -- $EXCLUDE_PATHS`; then
    echo "No matches found for pattern $SEARCH_PATTERN, stopping with no further replacement. Script may need to be updated"
    exit 1
  fi

  # Perform replacement
  git grep -l $SEARCH_PATTERN -- $EXCLUDE_PATHS | xargs sed -i '' -e $SED_PATTERN

  echo "Did replacements for pattern $SEARCH_PATTERN"
}

# Replace different cases for the variations of copyright notice we have.
# These patterns will need to be updated if how we write copyright notices does.
# Case: "Copyright 202x-[previous year]"->"Copyright 202x-[current year]"
tryReplacementForPattern "Copyright 202[0-9]-$PREVIOUS_YEAR" "s/Copyright (202[0-9])-$PREVIOUS_YEAR/Copyright \1-$CURRENT_YEAR/g"
# Case: "Copyright [previous year]"->"Copyright [previous year]-[current year]"
tryReplacementForPattern "Copyright $PREVIOUS_YEAR" "s/Copyright $PREVIOUS_YEAR/Copyright $PREVIOUS_YEAR-$CURRENT_YEAR/g"
# Case: "copyright_year = [previous year]"->"copyright_year = [current year]"
# (this is just for doc/rst/conf.py)
tryReplacementForPattern "copyright_year = $PREVIOUS_YEAR" "s/copyright_year = $PREVIOUS_YEAR/copyright_year = $CURRENT_YEAR/g"
