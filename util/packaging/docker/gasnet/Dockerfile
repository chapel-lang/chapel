FROM chapel/chapel:latest

ENV CHPL_COMM gasnet
ENV CHPL_RT_OVERSUBSCRIBED yes

RUN make -C $CHPL_HOME \
    && make -C $CHPL_HOME chpldoc test-venv mason \
    && make -C $CHPL_HOME cleanall

ENV GASNET_SPAWNFN L
