# This is a container used to run CI jobs on GH
# tzdata is necessary for util/test/check_annotations.py to function properly (get the right dates)
# locales is necessary for util/buildRelease/smokeTest docs

# We use 22.04 to get llvm-13
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y && apt-get install -y --no-install-recommends \
    bash \
    clang-13 \
    cmake \
    cscope \
    doxygen \
    g++ \
    gcc \
    git \
    libclang-13-dev \
    libclang-cpp13-dev \
    libedit-dev \
    llvm-13 \
    llvm-13-dev \
    llvm-13-tools \
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


# Configure locale (taken from util/dockerfiles/Dockerfile)
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    echo 'LANG="en_US.UTF-8"'>/etc/default/locale && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8
