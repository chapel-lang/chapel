#!/bin/bash

cat README.md.in > README.md
for name in [0-9]*.rst
do
  echo $name
  title=`head -n 1 $name`
  echo "* [$title]($name)" >> README.md
done

echo >> README.md

