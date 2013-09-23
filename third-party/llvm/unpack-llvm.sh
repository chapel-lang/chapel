#!/bin/sh

if [ -d llvm ]
then

echo LLVM and CLANG already unpacked.

else

echo Unpacking LLVM
tar xzf llvm-3.3.src.tar.gz
tar xzf cfe-3.3.src.tar.gz
mv llvm-3.3.src llvm
mv cfe-3.3.src llvm/tools/clang
echo Applying SROA address space patch for PR15907
cd llvm
patch -p0 < ../sroa-patch.txt
cd ..
fi
