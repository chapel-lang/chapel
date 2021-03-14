#!/bin/sh

CMAKE=$1
vers=`$CMAKE --version`
if [ $? -eq 0 ]
then
  case $vers in
    "cmake version 2.8.12")
      echo cmake 2.8.12 not new enough
      exit 1
      ;;
    "cmake version 2.8.12.1")
      echo cmake 2.8.12.1 not new enough
      exit 1
      ;;
    "cmake version 2.8.12"*)
      exit 0
      ;;
    "cmake version 3"*)
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

