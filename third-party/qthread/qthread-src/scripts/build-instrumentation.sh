#!/bin/bash

while [ ! -f ./autogen.sh ]; do 
    cd ..
    qthread_root=`pwd`
done

if [ ! -f ./autogen.sh ]; then
    echo "Error: I can't figure out where I am. Please run this script from the root of the qthreads source directory"
    exit 1
fi

if [ ! -x `which cmake` ]; then
    echo "You must install cmake to continue."
    echo "On debian/ubuntu systems, type 'sudo apt-get install cmake'"
    echo "On redhat systems, type 'yum install cmake'"
    exit 1
fi

if [ ! -d ./dependencies/install ]; then
    echo "    Create directory for external code dependencies..."
    mkdir -p ./dependencies/install
fi

if [ ! -f ./dependencies/cmocka-1.1.0.tar.xz ]; then
    echo "   Download cmocka and put it in the dependencies folder..."
    wget http://cmocka.org/files/1.1/cmocka-1.1.0.tar.xz -O dependencies/cmocka-1.1.0.tar.xz
fi

if [ ! -f ./dependencies/cmocka-1.1.0.tar.xz ]; then
    echo "Error: I couldn't download the cmocka unit test suite."
    echo "Please download http://cmocka.org/files/1.1/cmocka-1.1.0.tar.xz and place it in ${qthread_root}/dependencies/"
    exit 1
fi

if [ ! -f ./dependencies/install/lib/libcmocka.so ]; then
    echo "Building cmocka"
    pushd ./dependencies
    xz -d ./cmocka-1.1.0.tar.xz
    tar xf ./cmocka-1.1.0.tar
    cd cmocka-1.1.0
    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX="${qthread_root}/dependencies/install" ..
    make all install
    popd
fi

if [ ! -f ./dependencies/install/lib/libcmocka.so ]; then
    echo "Error: failed to build cmocka. You'll have to check the output above to see what happened"
    exit 1
fi

echo "**** $(pwd)"
echo " Dependencies done, now configure and build qthreads"
echo " this build will include test harness functions, to disable those "
echo " remove -DQTPERF_TESTING from the build flags (see the script for details)"
echo "****"
for i in 10 9 8 7 6 5 4 3 2 1; do
    echo -n "$i "
    sleep 1
done
echo ""

cmocka_lib="${qthread_root}/dependencies/install/lib"
cmocka_inc="${qthread_root}/dependencies/install/include"

if [ ! -f ./configure ]; then
    echo "**** autogen ****"
    if ! ./autogen.sh ; then
        echo "Autogen failed. Make sure you have installed the autoconf and libtool packages"
        echo "On debian/ubuntu systems, type 'sudo apt-get install automake autoconf libtool'"
        echo "On redhat systems, type 'sudo yum install automake autoconf libtool'"
        exit 1
    fi
fi


echo " **** Configure..."
## enable testing, disable perf debug output 
CFLAGS="-DQTHREAD_PERFORMANCE -DQTPERF_TESTING -I${cmocka_inc}" LDFLAGS="-L${cmocka_lib}" LIBS=-lcmocka ./configure --enable-debugging --disable-lazy-threadids  --enable-picky

if [ ! -f ./Makefile ]; then
    echo "Error configuring qthreads, look above for clues"
    exit 1
fi

echo " **** make clean"
make clean

echo " **** make"
make

if [ ! -e ./src/.libs/libqthread.so ]; then
    echo "Error building qthreads. Look for clues in the output above."
    exit 1
fi


