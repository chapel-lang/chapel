FROM debian:10

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
    git \
    && rm -rf /var/lib/apt/lists/*

ENV CHPL_VERSION master
ENV CHPL_HOME    /opt/chapel/$CHPL_VERSION
ENV CHPL_GMP     system

RUN mkdir -p /opt/chapel \
    && wget -q -O - https://github.com/chapel-lang/chapel/archive/$CHPL_VERSION.tar.gz | tar -xzC /opt/chapel --transform 's/chapel-//' \
    && make -C $CHPL_HOME \
    && make -C $CHPL_HOME chpldoc test-venv mason \
    && make -C $CHPL_HOME cleanall

# Configure locale
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    echo 'LANG="en_US.UTF-8"'>/etc/default/locale && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8

# Configure dummy git user
RUN git config --global user.email "noreply@example.com" && \
    git config --global user.name  "Chapel user"

ENV PATH $PATH:$CHPL_HOME/bin/linux64-x86_64:$CHPL_HOME/util
