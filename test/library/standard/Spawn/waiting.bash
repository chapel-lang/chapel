#!/bin/bash

rm -f WaitingForThisFile
echo WaitingForThisFile

for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
do
  echo Before
done

# Wait for the file to be created.
# On linux, this could use inotify to not sleep,
# but that wouldn't be portable.
while [ ! -f WaitingForThisFile ]
do
  echo Waiting
  sleep 1
done


for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
do
  echo After
done

rm -f WaitingForThisFile
