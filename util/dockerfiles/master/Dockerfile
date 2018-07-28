FROM debian:9.4

RUN apt-get update && apt-get install -y --no-install-recommends \
    wget \
    ca-certificates \
    curl \
    gcc \
    g++ \
    perl \
    python \
    python-dev \
    python-setuptools \
    libgmp10 \
    libgmp-dev \
    locales \
    bash \
    make \
    mawk \
    file \
    pkg-config \
    && rm -rf /var/lib/apt/lists/*

ENV CHPL_VERSION master
ENV CHPL_HOME    /opt/chapel/$CHPL_VERSION
ENV CHPL_GMP     system

RUN mkdir -p /opt/chapel \
    && wget -q -O - https://github.com/chapel-lang/chapel/archive/master.tar.gz | tar -xzC /opt/chapel --transform 's/chapel-//' \
    && make -C $CHPL_HOME \
    && make -C $CHPL_HOME chpldoc \
    && make -C $CHPL_HOME test-venv 

# Configure locale
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    echo 'LANG="en_US.UTF-8"'>/etc/default/locale && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8

ENV PATH $PATH:$CHPL_HOME/bin/linux64:$CHPL_HOME/util
