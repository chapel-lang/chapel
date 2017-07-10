#!/usr/bin/env sh

# This script must be run within c2chapel/test

NORMAL="\033[0m"
RED="\033[0;31m"
GREEN="\033[0;32m"

if ! type "c2chapel" > /dev/null 2>&1; then
  printf "${RED}Failure: c2chapel command not found${NORMAL}\n"
  exit 1;
fi

numFailures=0
numTotal=0

function helper() {
  msg=$1
  args=$2
  good=$3

  outFile=out.c2chapel.tmp
  diffFile=diff.c2chapel.tmp


  printf "%s: " "$msg"
  c2chapel $args > $outFile 2>&1
  if diff $outFile $good > $diffFile 2>&1; then
    printf "${GREEN}OK${NORMAL}\n"
  else
    printf "${RED}ERROR${NORMAL}\n"
    printf "\"c2chapel $args\" vs. $good':\n"
    cat $diffFile
    numFailures=$((numFailures+1))
  fi

  numTotal=$((numTotal+1))

  rm $outFile > /dev/null 2>&1
  rm $diffFile > /dev/null 2>&1
}

echo "Testing c2chapel...\n"

helper "No arguments" "" "no-args.good"
helper "--help" "--help" "help.good"
helper "File not found" "notFound.h" "notFound.good"

for f in *.h ; do
  title=`basename $f .h`
  otherArgs=""
  if [ -e $title.execopts ]; then
    otherArgs=`cat $title.execopts`
  fi
  helper "$title" "$f $otherArgs" "$title.chpl"
done

helper ".c file" "justC.c" "justC.chpl"

echo ""

if [ "$numFailures" -eq "0" ]; then
  printf "${GREEN}SUCCESS${NORMAL}\n"
else
  printf "${RED}$numFailures/$numTotal tests failed${NORMAL}\n"
fi
