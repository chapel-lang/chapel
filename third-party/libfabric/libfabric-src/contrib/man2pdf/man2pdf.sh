#!/bin/bash

# Copyright (c) 2017 Intel Corp.  All rights reserved.
#
# This software is available to you under a choice of one of two
# licenses.  You may choose to be licensed under the terms of the GNU
# General Public License (GPL) Version 2, available from the file
# COPYING in the main directory of this source tree, or the
# BSD license below:
#
#     Redistribution and use in source and binary forms, with or
#     without modification, are permitted provided that the following
#     conditions are met:
#
#      - Redistributions of source code must retain the above
#        copyright notice, this list of conditions and the following
#        disclaimer.
#
#      - Redistributions in binary form must reproduce the above
#        copyright notice, this list of conditions and the following
#        disclaimer in the documentation and/or other materials
#        provided with the distribution.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

#
# MAN 2 PDF -- Simple script to convert the libfabric manpages into a PDF
#
# Requires libfabric manpages to be installed and available in MANPATH.
# Uses man and ps2pdf to convert each manpage, followed by Mac join.py or
# pdfjoin to generate final PDF
#

# Generate list of manpages, but exclude manpages that are symlinks to other manpages
MANPAGES=( `echo fabric && man -a -w fi_* | xargs basename | sort | uniq | cut -d . -f 1` )

PDFMANPAGES=( "${MANPAGES[@]/%/.pdf}" )
WORKDIR=`mktemp -d /tmp/libfabric-manpages.XXXXXX || exit 1`
OUTFILE="libfabric-manpages-`date \"+%Y-%m-%d\"`.pdf"
EXITDIR="$PWD"

if [ -e $OUTFILE ]; then
    echo "Error: $OUTFILE already exists"
    exit 1
fi

echo Entering work directory: $WORKDIR
cd $WORKDIR || exit 1

for manpage in "${MANPAGES[@]}"; do
    echo "  - Converting $manpage"
    man -t $manpage | ps2pdf - $WORKDIR/${manpage}.pdf || exit 1
done

echo "  + Merging manpages"

if [ -x /System/Library/Automator/Combine\ PDF\ Pages.action/Contents/Resources/join.py ]; then
    /System/Library/Automator/Combine\ PDF\ Pages.action/Contents/Resources/join.py -o $OUTFILE "${PDFMANPAGES[@]}" || exit 1
elif hash pdfjoin 2>/dev/null; then
    pdfjoin --quiet -o $OUTFILE "${PDFMANPAGES[@]}" || exit 1
else
    echo "Error: could not join PDFs (check installation of pdfjoin)"
fi

echo "  + Cleaning up"

rm -f "${PDFMANPAGES[@]}"
cd $EXITDIR
if [ -e $WORKDIR/$OUTFILE ]; then
    mv $WORKDIR/$OUTFILE .
    echo "Done: $OUTFILE"
else
    echo "PDF generation failed"
fi

rmdir $WORKDIR

