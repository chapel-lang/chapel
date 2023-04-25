FROM debian:11

RUN apt-get update && apt-get install -y --no-install-recommends \
    bash \
    ca-certificates \
    clang-11 \
    cmake \
    curl \
    file \
    gcc \
    git \
    g++ \
    libclang-11-dev \
    libclang-cpp11-dev \
    libedit-dev \
    libgmp10 \
    libgmp-dev \
    llvm-11-dev \
    llvm-11 \
    llvm-11-tools \
    locales \
    make \
    mawk \
    m4 \
    perl \
    pkg-config \
    protobuf-compiler \
    python-setuptools \
    python3 \
    python3-pip \
    python3-venv \
    python3-dev \
    wget \
    && rm -rf /var/lib/apt/lists/*

# configure locale
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    echo 'LANG="en_US.UTF-8"'>/etc/default/locale && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8

# configure dummy git user (required for Mason unit tests)
RUN git config --global user.email "noreply@example.com" && \
    git config --global user.name  "Chapel user"

# set up Chapel environment variables
ENV CHPL_HOME=/opt/chapel \
    CHPL_GMP=system \
    CHPL_LLVM=system

# acquire sources
WORKDIR $CHPL_HOME
COPY . .

# build Chapel for both C and LLVM backends
RUN CHPL_TARGET_COMPILER=llvm make \
    && CHPL_TARGET_COMPILER=gnu make \
    && make chpldoc test-venv mason \
    && make cleanall

# Hack to get access to Chapel binaries
RUN cd $CHPL_HOME/bin && ln -s */* .
ENV PATH="${PATH}:${CHPL_HOME}/bin:${CHPL_HOME}/util"
