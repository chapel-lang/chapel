#!/bin/sh

if [ -d re2 ]
then

echo Updating RE2
cd re2
git pull
else

echo Checkout RE2
git clone https://github.com/google/re2 re2
cd re2
git checkout cc56ba02d9d2bdafa614ad5ebf564dde287625bb
git checkout -b chapel-branch
echo Applying Patches
patch -p1 < ../hg_diff_g.patch
echo Copying file_strings.h and file_strings.cc
cp ../file_strings.h re2/
cp ../file_strings.cc re2/
fi
