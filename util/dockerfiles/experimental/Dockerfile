FROM chapel/chapel:latest

ENV CHPL_COMM gasnet
ENV CHPL_GASNET_SUBSTRATE fast

RUN make -C $CHPL_HOME \
    && make -C $CHPL_HOME chpldoc test-venv mason \
    && make -C $CHPL_HOME cleanall

ENV QT_AFFINITY no
ENV QT_SPINCOUNT 300
ENV GASNET_SPAWNFN L
