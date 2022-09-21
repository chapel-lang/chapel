FROM chapel/chapel:main

ENV CHPL_COMM gasnet

RUN make -C $CHPL_HOME \
    && make -C $CHPL_HOME chpldoc \
    && make -C $CHPL_HOME test-venv \
    && make -C $CHPL_HOME mason

ENV GASNET_SPAWNFN L
