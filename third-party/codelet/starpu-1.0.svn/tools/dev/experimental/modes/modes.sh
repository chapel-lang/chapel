#!/bin/bash

PREFIX=tools/dev/experimental/modes/
tmp=`mktemp`

spatch -in_place -sp_file $PREFIX/modes.cocci $1
$PREFIX/fix_cocci_output.py $1 > $tmp
mv $tmp $1
