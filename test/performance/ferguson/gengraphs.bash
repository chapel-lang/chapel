#!/bin/bash

for name in *.chpl
do
  echo $name
  case $name in
    CommUtil.chpl)
      ;;
    array-assign-block.chpl)
      ;;
    array-assign-block-get.chpl)
      ;;
    array-assign.chpl)
      ;;
    array-assign-get.chpl)
      ;;
    *)
      base=${name%.chpl}
      echo processing $base

      # regenerate .ml-compopts
      rm -f ${base}.ml-compopts
      ln -s MYCOMPOPTS ${base}.ml-compopts

      # Detect whether we are graphing GETS, PUTS, or ONS
      GETS=1
      PUTS=1
      ONS=1
      if grep -q maxGets $name
      then
        # if maxGets was found in the file
        GETS=0
      fi
      if grep -q maxPuts $name
      then
        PUTS=0
      fi
      if grep -q maxOns $name
      then
        ONS=0
      fi

      # regenerate .ml-keys
      rm -f ${base}.ml-keys
      if [ "$GETS" -eq 1 ]
      then
        echo GETs: >> ${base}.ml-keys
      fi
      if [ "$PUTS" -eq 1 ]
      then
        echo PUTs: >> ${base}.ml-keys
      fi
      if [ "$ONS" -eq 1 ]
      then
        echo ONs: >> ${base}.ml-keys
      fi

      # regenerate .graph
      rm -f ${base}.graph
      PERFKEYS="perfkeys: "
      FILES="files: "
      GRAPHKEYS="graphkeys: "
      echo -n "   "
      for cfg in c c-cache
      do
        echo -n "$cfg "
        if [ "$GETS" -eq 1 ]
        then
          PERFKEYS+="GETs:, "
          GRAPHKEYS+="$cfg GETs, "
          FILES+="${base}-${cfg}.dat, "
        fi
        if [ "$PUTS" -eq 1 ]
        then
          PERFKEYS+="PUTs:, "
          GRAPHKEYS+="$cfg PUTs, "
          FILES+="${base}-${cfg}.dat, "
        fi
        if [ "$ONS" -eq 1 ]
        then
          PERFKEYS+="ONs:, "
          GRAPHKEYS+="$cfg ONs, "
          FILES+="${base}-${cfg}.dat, "
        fi
      done

      echo

      # Trim off trailing comma space
      PERFKEYS=${PERFKEYS%??}
      FILES=${FILES%??}
      GRAPHKEYS=${GRAPHKEYS%??}

      echo ${PERFKEYS} >> ${base}.graph
      echo ${FILES} >> ${base}.graph
      echo ${GRAPHKEYS} >> ${base}.graph
      echo "ylabel: Count" >> ${base}.graph
      echo "graphtitle: ${base}" >> ${base}.graph

      ;;
  esac
done
