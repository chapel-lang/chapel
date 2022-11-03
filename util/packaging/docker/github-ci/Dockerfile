# This is a container used to run CI jobs on GH
# tzdata is necessary for util/test/check_annotations.py to function properly (get the right dates)
# locales is necessary for util/buildRelease/smokeTest docs

# We use 22.10 to get llvm-14
FROM ubuntu:22.10

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y && apt-get install -y --no-install-recommends \
    bash \
    clang-14 \
    cmake \
    cscope \
    doxygen \
    g++ \
    gcc \
    git \
    libclang-14-dev \
    libclang-cpp14-dev \
    libedit-dev \
    llvm-14 \
    llvm-14-dev \
    llvm-14-tools \
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
