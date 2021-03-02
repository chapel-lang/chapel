
QTHREADS=../..
QTH_INC=$(QTHREADS)/include
QTH_LIB=$(QTHREADS)/src/.libs
CMOCK_LIB=${QTHREADS}/dependencies/install/lib
CMOCK_INC=${QTHREADS}/dependencies/install/include
PKGLIB=${QTHREADS}/dependencies/install/lib/pkgconfig

CFLAGS_OPT=-I$(QTH_INC) -I${CMOCK_INC} -O3 -Wall -DPERFDBG=1 -DQTPERF_TESTING -DQTHREAD_PERFORMANCE -DQTHREAD_NONLAZY_THREADIDS=1 -g 
CFLAGS_DBG=-I$(QTH_INC) -I${CMOCK_INC} -Wall -DPERFDBG=1 -DQTPERF_TESTING -DQTHREAD_PERFORMANCE -DQTHREAD_NONLAZY_THREADIDS=1 -g 
CFLAGS_NOPERF=-I$(QTH_INC) -DQTHREAD_NONLAZY_THREADIDS=1 -g 
CFLAGS=-std=c99 ${CFLAGS_DBG}

LDFLAGS=-L$(QTH_LIB) -lqthread -L${CMOCK_LIB} -lcmocka
TESTS=$(shell ls test_*.c)
OBJECTS=$(subst .c,.o,${TESTS})
TARGETS=$(subst test_,,$(subst .c,.test,${TESTS}))

LIBPATH=${QTH_LIB}:${CMOCK_LIB}:${LD_LIBRARY_PATH}

all: ${TARGETS}

%.test: test_%.o $(QTH_LIB)/libqthread.la
	gcc -o $*.test test_$*.o $(LDFLAGS)

check: all
	for tst in ${TARGETS}; do echo "start $${tst}"; LD_LIBRARY_PATH="${LIBPATH}" QT_NUM_SHEPHERDS=9 valgrind -q --leak-check=full "./$$tst" > /dev/null; echo "finish $${tst}"; done

%.o: %.c
	gcc -c -o $*.o $(CFLAGS) $*.c

check-%: all
	LD_LIBRARY_PATH="${LIBPATH}" QT_NUM_SHEPHERDS=9 valgrind -q --read-var-info --leak-check=full ./$*.test 

clean:
	rm -f *~ *.o ${TARGETS}
