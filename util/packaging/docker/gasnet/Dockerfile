FROM chapel/chapel:latest

ENV CHPL_COMM=gasnet
ENV CHPL_RT_OVERSUBSCRIBED=yes

RUN CHPL_TARGET_COMPILER=llvm make -C $CHPL_HOME \
    && CHPL_TARGET_COMPILER=gnu make -C $CHPL_HOME \
    && make -C $CHPL_HOME chpldoc test-venv mason \
    && make -C $CHPL_HOME cleanall

ENV GASNET_SPAWNFN=L
