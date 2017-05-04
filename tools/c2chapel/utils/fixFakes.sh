#!/bin/sh

fakesDir=$1
header=$2

linkName="_c2chapel_custom.h"
linkPath=${fakesDir}/${linkName}

if [ -d ${fakesDir} -a -f ${header} ]; then
  echo "Fixing fake headers..."

  # Use system limits.h
  rm ${fakesDir}/limits.h

  # include our custom header in every default fake header
  for f in `find ${fakesDir} -iname "*.h"` ; do
    echo "#include \"${linkName}\"" >> $f
  done

  if [ -f ${linkPath} ]; then
    rm ${linkPath}
  fi
  ln -s ${header} ${linkPath}

else
  echo "FAILURE: Invalid arguments: ${fakesDir} ${header}"
  exit 1
fi
