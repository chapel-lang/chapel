#!/usr/bin/env bash
#
# Update current copyright year in source file header comments.
# Excludes third-party and test directories.
# Needs to be updated if how we write copyright notices is.

set -e

# Paths to be ignored for update by 'git grep'
EXCLUDE_PATHS=(':!third-party/' ':!test/')


# Read in and validate input years
DRY_RUN=false
if [[ "${1:-}" == "--dry-run" ]]; then
  DRY_RUN=true
  shift
fi
if [[ $# -ne 2 ]]; then
  echo "Usage: $0 [--dry-run] previous_year current_year"
  exit 1
fi
PREVIOUS_YEAR=$1
CURRENT_YEAR=$2
if [[ "$DRY_RUN" == false ]]; then
  if [ "$CURRENT_YEAR" != "$(date '+%Y')" ]; then
    echo "Specified current year $CURRENT_YEAR does not appear to be the actual current year"
    exit 1
  fi
  if [[ $((PREVIOUS_YEAR + 1)) != "$CURRENT_YEAR" ]]; then
    echo "Current year $CURRENT_YEAR is not exactly 1 greater than previous year $PREVIOUS_YEAR"
    exit 1
  fi
fi


# Work from $CHPL_HOME for exclude dir paths
if [ -z "$CHPL_HOME" ]; then
  echo "CHPL_HOME not set"
  exit 1
fi
echo "Using CHPL_HOME of $CHPL_HOME"
cd "$CHPL_HOME"


# Require working from clean repo for safety.
if [ -n "$(git status -u --porcelain)" ]; then
  echo "Refusing to run from unclean working directory for safety"
  exit 1
fi

# Make updates.
echo "Updating latest copyright year from $PREVIOUS_YEAR to $CURRENT_YEAR..."

# Do replacement for a pattern.
# First arg is 'git grep' search pattern, second arg is 'sed' find/replace pattern.
# Pass 'optional' as a third arg if the pattern might not appear in a given year.
function tryReplacementForPattern {
  SEARCH_PATTERN=$1
  SED_PATTERN=$2
  local grep_status

  echo -n "Attempting replacements for grep pattern '$SEARCH_PATTERN' with sed command '$SED_PATTERN'... "

  search_output=$(git --no-pager grep -l "$SEARCH_PATTERN" -- "${EXCLUDE_PATHS[@]}") || {
    grep_status=$?
    if [[ "$grep_status" -eq 1 && "${3:-}" == optional ]]; then
      echo "no matches (optional)"
      return 0
    fi
    echo "ERROR"
    if [[ "$grep_status" -eq 1 ]]; then
      echo "No matches found for pattern '$SEARCH_PATTERN', stopping with no further replacement; script may need to be updated if we aren't using this pattern anymore."
    else
      echo "git grep failed for pattern '$SEARCH_PATTERN' (exit status $grep_status)."
    fi
    exit "$grep_status"
  }

  # Perform replacement
  # Sed '-i' argument for in-place differs between GNU sed (on Linux) and BSD sed (on macOS).
  if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "$search_output" | xargs sed -i '' -e "$SED_PATTERN"
  else
    echo "$search_output" | xargs sed -i -e "$SED_PATTERN"
  fi

  echo "done"
}

# Replace different cases for the variations of copyright notice we have.
# These patterns will need to be updated if how we write copyright notices does.

# Case: "Copyright 20xy-[previous year]"->"Copyright 20xy-[current year]"
tryReplacementForPattern "Copyright 20[0-9][0-9]-$PREVIOUS_YEAR" "s/Copyright \(20[0-9][0-9]\)-$PREVIOUS_YEAR Hewlett/Copyright \1-$CURRENT_YEAR Hewlett/g"
# Case: "Copyright [previous year]"->"Copyright [previous year]-[current year]"
tryReplacementForPattern "Copyright $PREVIOUS_YEAR Hewlett" "s/Copyright $PREVIOUS_YEAR Hewlett/Copyright $PREVIOUS_YEAR-$CURRENT_YEAR Hewlett/g" optional
# Case: "copyright_year = [previous year]"->"copyright_year = [current year]"
# (this is just for doc/rst/conf.py)
tryReplacementForPattern "copyright_year = $PREVIOUS_YEAR" "s/copyright_year = $PREVIOUS_YEAR/copyright_year = $CURRENT_YEAR/g"
