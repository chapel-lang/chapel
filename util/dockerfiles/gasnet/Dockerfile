FROM chapel/chapel:latest

ENV CHPL_COMM gasnet

RUN make -C $CHPL_HOME \
    && make -C $CHPL_HOME chpldoc

ENV GASNET_SPAWNFN L
