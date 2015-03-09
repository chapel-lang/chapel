#!/bin/sh

if [ -d llvm ]
then

echo LLVM and CLANG already unpacked.

else

echo Unpacking LLVM

tar xf llvm-3.6.0.src.tar.xz
tar xf cfe-3.6.0.src.tar.xz
mv llvm-3.6.0.src llvm
mv cfe-3.6.0.src llvm/tools/clang

#tar xf llvm-3.5.1.src.tar.xz
#tar xf cfe-3.5.1.src.tar.xz
#mv llvm-3.5.1.src llvm
#mv cfe-3.5.1.src llvm/tools/clang

#tar xzf llvm-3.4.2.src.tar.gz
#tar xzf cfe-3.4.2.src.tar.gz
#mv llvm-3.4.2.src llvm
#mv cfe-3.4.2.src llvm/tools/clang
#tar xzf llvm-3.3.src.tar.gz
#tar xzf cfe-3.3.src.tar.gz
#mv llvm-3.3.src llvm
#mv cfe-3.3.src llvm/tools/clang
#echo Applying SROA address space patch for PR15907
#cd llvm
##patch -p0 < ../sroa-patch.txt
#cd ..
fi
