#!/usr/bin/env bash
#This script will compile and execute helloworld inside a docker image and check if chapel is installed correctly

# Check if chapel is installed correctly
which chpl
if [ $? -ne 0 ]
then
      echo "which chpl failed inside chapel container"
      exit 1
fi

chpl --version
if [ $? -ne 0 ]
then
      echo "chpl --version failed inside chapel container"
      exit 1
fi

  
# Compile and execute chapel hello
echo 'writeln("Hello, world!");' > hello.chpl
chpl -o hello hello.chpl
if [ $? -ne 0 ]
then
      echo "hello.chpl compile failed inside chapel container" 
      exit 1
else
      echo "hello.chpl compiled inside chapel container"
fi

./hello
if [ $? -ne 0 ]
then
      echo "./hello failed inside chapel container" 
      exit 1
else
      echo "./hello succeeded inside chapel container"
fi
