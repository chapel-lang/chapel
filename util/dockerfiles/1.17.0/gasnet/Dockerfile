FROM chapel/chapel:latest

ENV CHPL_COMM gasnet

RUN make -C $CHPL_HOME \
    && make -C $CHPL_HOME chpldoc \
    && make -C $CHPL_HOME test-venv

ENV GASNET_SPAWNFN L
