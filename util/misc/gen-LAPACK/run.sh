#!/bin/bash

# temps, remove
PUSHBUTTONDIR=$PWD
PATCHROOM=$( mktemp -d /tmp/lapackgen.XXXXXXXXXXX )
LAPACK=LAPACK

# output, dont remove
CHALAPACK=ChaLAPACK

# statics, dont remove
PATCHFILE=LAPACK.patch
EXTERN_TOOL=extern-tool

if [ -e $CHALAPACK ]
then
  echo "Please move or delete existing $CHALAPACK directory"
  exit -1
fi

cp $PATCHFILE $PATCHROOM/.

cd $PATCHROOM

echo "Downloading lapack-3.5.0.tgz"
wget http://www.netlib.org/lapack/lapack-3.5.0.tgz

echo "Extracting lapack-3.5.0.tgz"
tar -xzf lapack-3.5.0.tgz

echo "Patching "
mv lapack-3.5.0 $LAPACK
patch -s -p0 < $PATCHFILE

#mv $LAPACK $PUSHBUTTONDIR/.
cd $PUSHBUTTONDIR

mkdir $CHALAPACK

cd $PUSHBUTTONDIR/$EXTERN_TOOL

echo "Running magic bullet script"
./main.py $PATCHROOM/$LAPACK

mv *.chpl $PUSHBUTTONDIR/$CHALAPACK/.

cd $PUSHBUTTONDIR

echo "Finished. Output can be found in $PUSHBUTTONDIR/$CHALAPACK/"



