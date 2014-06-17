#!/bin/sh

if [ -d re2 ]
then

echo Updating RE2
cd re2
hg pull -u
else

echo Checkout RE2
hg clone https://re2.googlecode.com/hg re2
echo Applying Patches
cd re2
patch -p1 < ../hg_diff_g.patch
echo Copying file_strings.h and file_strings.cc
cp ../file_strings.h re2/
cp ../file_strings.cc re2/
fi
