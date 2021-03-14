export LIBRARY_PATH=`pwd`/third-party/install/lib
export LD_LIBRARY_PATH=`pwd`/third-party/install/lib
export C_INCLUDE_PATH=`pwd`/third-party/install/include

TESTU01_LIB=third-party/install/lib
TESTU01_INC=third-party/install/include
PCG_SRC=third-party/pcg-c-basic-0.9

TESTS="test-npb32 test-npb64 test-pcg-real-32 test-pcg-real-64 test-pcg-uint-32 test-pcg-uint-32-c test-pcg-uint-64 test-pcg-uint-32-bounded"
