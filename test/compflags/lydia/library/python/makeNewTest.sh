#!/bin/bash

opener="emacs"
for file in checkDiffModname.*; do
    newname="$(echo $file | sed -e 's/DiffModname/DiffPythonModname/')";
    cp $file $newname;
    opener="$opener $newname"
done

#for file in use_bar.*; do
#    newname="$(echo $file | sed -e 's/checkExplicitLibname/bar/')";
#    cp $file $newname;
#    opener="$opener $newname"
#done
eval $opener
