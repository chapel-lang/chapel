#!/bin/sh

if [ -d re2 ]
then

echo Updating RE2
cd re2
git pull
else

echo Checkout RE2
git clone https://github.com/google/re2 re2
git checkout 1deddeb2fe8674852fc425f03727e4ab0903357b
git checkout -b chapel-branch
echo Applying Patches
cd re2
patch -p1 < ../hg_diff_g.patch
echo Copying file_strings.h and file_strings.cc
cp ../file_strings.h re2/
cp ../file_strings.cc re2/
fi
