#!/bin/sh
#
# Author: Brian W. Barrett <bwbarre@sandia.gov>
#

echo "Generating configure files..."

# If this directory isn't removed, the configure script may not have the right
# dynamically-generated version number
if [ -d autom4te.cache ] ; then
	rm -rf autom4te.cache
fi

autoreconf --install --symlink --warnings=gnu,obsolete,override,portability,no-obsolete && \
  echo "Preparing was successful if there was no error messages above." && \
  exit 0

echo "It appears that configure file generation failed.  Sorry :(."
