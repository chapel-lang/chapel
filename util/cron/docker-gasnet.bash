#!/usr/bin/env bash
# This script will compile and execute hello world inside a docker-gasnet and docker-gasnet-smp container(s) and check if chapel is installed


#check chpl install inside chapel(gasnet/gasnet-smp) container 
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

  
# Compile and execute chapel hello inside the container
echo 'writeln("Hello, world!");' > hello.chpl
chpl -o hello hello.chpl
if [ $? -ne 0 ]
then
      echo "hello.chpl compile failed inside chapel container" 
      exit 1
else
      echo "hello.chpl compiled inside chapel container"
fi

./hello -nl 1
if [ $? -ne 0 ]
then
      echo "./hello -nl 1 failed inside chapel gasnet container" 
      exit 1
else
      echo "./hello -nl 1 succeeded inside chapel gasnet container"
fi

