FROM debian:testing

ENV CHPL_VERSION 1.13.1

RUN apt-get update && apt-get install -y --no-install-recommends \
    wget \
    ca-certificates \
    gcc \
    g++ \
    perl \
    python \
    python-dev \
    python-setuptools \
    bash \
    make \
    mawk \
    && rm -rf /var/lib/apt/lists/*

RUN mkdir -p /opt/chapel \
    && wget -q -O - https://github.com/chapel-lang/chapel/releases/download/$CHPL_VERSION/chapel-$CHPL_VERSION.tar.gz | tar -xzC /opt/chapel --transform 's/chapel-//' \
    && make -C /opt/chapel/$CHPL_VERSION

ENV CHPL_HOME /opt/chapel/$CHPL_VERSION

ENV PATH $PATH:$CHPL_HOME/bin/linux64:$CHPL_HOME/util
