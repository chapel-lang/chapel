# This is a container used to run CI jobs on GH
# tzdata is necessary for util/test/check_annotations.py to function properly (get the right dates)
# locales is necessary for util/buildRelease/smokeTest docs

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y && apt-get install -y --no-install-recommends \
    bash \
    clang-19 \
    cmake \
    cscope \
    doxygen \
    g++ \
    gcc \
    git \
    libclang-19-dev \
    libclang-cpp19-dev \
    libedit-dev \
    llvm-19 \
    llvm-19-dev \
    llvm-19-tools \
    locales \
    m4 \
    make \
    mawk \
    perl \
    pkg-config \
    python3 \
    python3-dev \
    python3-pip \
    python3-venv \
    tzdata \
    && rm -rf /var/lib/apt/lists/*


# Configure locale (taken from primary Chapel Dockerfile)
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    echo 'LANG="en_US.UTF-8"'>/etc/default/locale && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8
