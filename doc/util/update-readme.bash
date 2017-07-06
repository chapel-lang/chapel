#!/bin/bash

# Run this to update CHIP table of contents in README.md  after adding a CHIP

cat README.md.in > README.md

for name in `ls -v [0-9]*.rst | sort -n`
do
  echo $name
  title=`head -n 10 $name | grep -e '^[A-Za-z]' | head -n 1`
  number=`echo $name | sed 's/.rst$//'`
  echo "* [$number $title]($name)" >> README.md
done

echo >> README.md

