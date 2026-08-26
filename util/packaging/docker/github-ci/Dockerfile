# This is a container used to run CI jobs on GH
# tzdata is necessary for util/test/check_annotations.py to function properly (get the right dates)
# locales is necessary for util/buildRelease/smokeTest docs

FROM ubuntu:latest

ENV DEBIAN_FRONTEND=noninteractive

RUN (type -p wget >/dev/null || (apt update && apt install wget -y)) \
    && mkdir -p -m 755 /etc/apt/keyrings \
    && out=$(mktemp) && wget -nv -O$out https://cli.github.com/packages/githubcli-archive-keyring.gpg \
    && cat $out | tee /etc/apt/keyrings/githubcli-archive-keyring.gpg > /dev/null \
    && chmod go+r /etc/apt/keyrings/githubcli-archive-keyring.gpg \
    && mkdir -p -m 755 /etc/apt/sources.list.d \
    && echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/githubcli-archive-keyring.gpg] https://cli.github.com/packages stable main" | tee /etc/apt/sources.list.d/github-cli.list > /dev/null \
    && apt update \
    && apt install gh -y

RUN apt-get update -y && apt-get install -y --no-install-recommends \
    bash \
    clang \
    cmake \
    cscope \
    curl \
    doxygen \
    g++ \
    gcc \
    git \
    jq \
    libclang-dev \
    libclang-cpp-dev \
    libedit-dev \
    libprotoc-dev \
    llvm \
    llvm-dev \
    llvm-tools \
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
    time \
    tzdata \
    wget \
    xxd \
    && rm -rf /var/lib/apt/lists/*

RUN groupadd --gid 1001 chpl-tester && \
    useradd --uid 1001 --gid chpl-tester --create-home --shell /bin/bash chpl-tester


# Configure locale (taken from primary Chapel Dockerfile)
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    echo 'LANG="en_US.UTF-8"'>/etc/default/locale && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8

# Work around GitHub Actions git "dubious ownership" error
RUN git config --system --add safe.directory "*"
