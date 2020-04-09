#!/usr/bin/env bash
#
# Build Chapel and upload results to Coverity Scan.
#
# In order to run this script:
#
#  * You need a Coverity Scan account and access to the Chapel project.
#  * A ~/.coverity file must exist and have at least the following two tokens:
#    COV_SCAN_EMAIL and COV_SCAN_TOKEN.
#  * COV_BUILD_PREFIX must be defined in the environment or in ~/.coverity.
#

# COV_BUILD_PREFIX, COV_SCAN_EMAIL, and COV_SCAN_TOKEN can be set in
# ~/.coverity.
if [ -f ~/.coverity ] ; then
    source ~/.coverity
fi

if [ "${COV_SCAN_EMAIL+x}" != "x" ] ; then
    echo "[ERROR] COV_SCAN_EMAIL needs to be set."
    cov_error=true
fi
if [ "${COV_SCAN_TOKEN+x}" != "x" ] ; then
    echo "[ERROR] COV_SCAN_TOKEN needs to be set."
    cov_error=true
fi
if [ "${COV_BUILD_PREFIX+x}" != "x" ] ; then
    echo "[ERROR] COV_BUILD_PREFIX needs to be set."
    cov_error=true
fi
if [ "${cov_error}" = "true" ] ; then
    exit 1
fi

# Grab the current revision and set description.
version=$(git rev-parse --short HEAD)
description="Git master build."

# Build chapel with cov-build.
$COV_BUILD_PREFIX/cov-build --dir cov-int make

# Create compressed tarball.
tar --create --bzip2 --verbose --file chapel.tar.bz2 cov-int

curl --verbose --silent \
    --form token="${COV_SCAN_TOKEN}" \
    --form email="${COV_SCAN_EMAIL}" \
    --form file=@chapel.tar.bz2 \
    --form version="${version}" \
    --form description="${description}" \
    https://scan.coverity.com/builds?project=chapel
