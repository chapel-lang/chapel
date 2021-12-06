#!/bin/sh

CMAKE=$1
vers=`$CMAKE --version`
if [ $? -eq 0 ]
then
  case $vers in
    *"version 3"*)
      exit 0
      ;;
    *)
      echo cmake version not recognized
      exit 1
      ;;
  esac
else
  echo cmake not found
  exit 1
fi

