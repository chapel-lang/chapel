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
      echo processing $name
      ;;
  esac
done
