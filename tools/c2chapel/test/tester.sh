#!/usr/bin/env bash

# This script must be run within c2chapel/test

# Only use colors for stdout with a color-supporting terminal
if test -t 1; then
  nc=$(tput colors)

  if test $? -eq 0 && test $nc -ge 8; then
    NORMAL=$(tput sgr0)
    RED=$(tput setaf 1)
    GREEN=$(tput setaf 2)
  fi
fi

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
  good2=$4

  outFile=out.c2chapel.tmp
  diffFile=diff.c2chapel.tmp

  if [ -z "$good2" ]; then
    # it will just diff against the one good file twice, harmlessly
    good2=$good
  fi

  printf "%s: " "$msg"
  c2chapel $args > $outFile 2>&1
  if diff $outFile $good > $diffFile 2>&1; then
    printf "${GREEN}OK${NORMAL}\n"
  elif diff $outFile $good2 > $diffFile 2>&1; then
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

helper "No arguments" "" "no-args.good" "no-args.2.good"
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
  exit 1;
fi
