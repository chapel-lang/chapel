FROM chapel/chapel:latest

ENV CHPL_COMM gasnet
ENV CHPL_COMM_SUBSTRATE smp
ENV CHPL_RT_OVERSUBSCRIBED yes

# By default GASNet will use POSIX shared memory, whose memory allocations
# reside in /dev/shm. The default /dev/shm size for docker is only 64MB, which
# is far too small for us, so use the SystemV implementation instead.
ENV CHPL_GASNET_MORE_CFG_OPTIONS "--disable-pshm-posix --enable-pshm-sysv"

RUN make -C $CHPL_HOME \
    && make -C $CHPL_HOME chpldoc test-venv mason \
    && make -C $CHPL_HOME cleanall
