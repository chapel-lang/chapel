#!/bin/sh

if [ -d llvm ]
then

echo LLVM and CLANG already unpacked.

else

echo Unpacking LLVM

# Upgrading LLVM versions?
# Be sure to also update:
#
#   Makefile.share-system
#   doc/rst/usingchapel/chplenv.rst

tar xf llvm-6.0.0.src.tar.xz
tar xf cfe-6.0.0.src.tar.xz
mv llvm-6.0.0.src llvm
mv cfe-6.0.0.src llvm/tools/clang
patch -p0 < llvm-6.0.0-BasicAliasAnalysis-patch.txt
patch -p0 < llvm-6.0.0-ValueTracking-patch.txt

#tar xf llvm-5.0.1.src.tar.xz
#tar xf cfe-5.0.1.src.tar.xz
#mv llvm-5.0.1.src llvm
#mv cfe-5.0.1.src llvm/tools/clang
#patch -p0 < llvm-4.0.1-BasicAliasAnalysis-patch.txt
#patch -p0 < llvm-4.0.1-ValueTracking-patch.txt

#tar xf llvm-4.0.1.src.tar.xz
#tar xf cfe-4.0.1.src.tar.xz
#mv llvm-4.0.1.src llvm
#mv cfe-4.0.1.src llvm/tools/clang
#patch -p0 < llvm-4.0.1-BasicAliasAnalysis-patch.txt
#patch -p0 < llvm-4.0.1-ValueTracking-patch.txt

fi
