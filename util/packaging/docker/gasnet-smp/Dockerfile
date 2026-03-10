FROM chapel/chapel:latest

ARG MAKE_THREADS=1

ENV CHPL_COMM=gasnet
ENV CHPL_COMM_SUBSTRATE=smp
ENV CHPL_RT_OVERSUBSCRIBED=yes

# By default GASNet will use POSIX shared memory, whose memory allocations
# reside in /dev/shm. The default /dev/shm size for docker is only 64MB, which
# is far too small for us, so use the SystemV implementation instead.
ENV CHPL_GASNET_MORE_CFG_OPTIONS="--disable-pshm-posix --enable-pshm-sysv"

RUN CHPL_TARGET_COMPILER=llvm make -j$MAKE_THREADS -C $CHPL_HOME \
    && CHPL_TARGET_COMPILER=gnu make -j$MAKE_THREADS -C $CHPL_HOME \
    && make -j$MAKE_THREADS -C $CHPL_HOME chpldoc test-venv mason \
    && make -j$MAKE_THREADS -C $CHPL_HOME cleanall
