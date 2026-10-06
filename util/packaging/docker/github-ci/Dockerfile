# This is a container used to run CI jobs on GH
# tzdata is necessary for util/test/check_annotations.py to function properly (get the right dates)
# locales is necessary for util/buildRelease/smokeTest docs

FROM ubuntu:24.04

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
    clang-19 \
    cmake \
    cscope \
    curl \
    doxygen \
    fluid \
    g++ \
    gcc \
    gdb \
    git \
    jq \
    libclang-19-dev \
    libclang-cpp19-dev \
    libclang-rt-19-dev \
    libcurl4-openssl-dev \
    libedit-dev \
    libfltk-dev \
    libprotoc-dev \
    libzmq3-dev \
    llvm-19 \
    llvm-19-dev \
    llvm-19-tools \
    lldb-19 \
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
    sudo \
    time \
    tzdata \
    wget \
    xxd \
    && rm -rf /var/lib/apt/lists/* \
    && update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-19 100 \
    && update-alternatives --install /usr/bin/lldb-server lldb-server /usr/bin/lldb-server-19 100 \
    && update-alternatives --install /usr/bin/llvm-symbolizer llvm-symbolizer /usr/bin/llvm-symbolizer-19 100

RUN groupadd --gid 1001 chpl-tester && \
    useradd --uid 1001 --gid chpl-tester --create-home --shell /bin/bash chpl-tester && \
    echo "chpl-tester ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/chpl-tester && \
    chmod 0440 /etc/sudoers.d/chpl-tester

RUN echo "settings set target.disable-aslr false" > /root/.lldbinit && \
    echo "settings set target.disable-aslr false" > /home/chpl-tester/.lldbinit && \
    chown chpl-tester:chpl-tester /home/chpl-tester/.lldbinit

# Configure locale (taken from primary Chapel Dockerfile)
RUN sed -i -e 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && \
    echo 'LANG="en_US.UTF-8"'>/etc/default/locale && \
    dpkg-reconfigure --frontend=noninteractive locales && \
    update-locale LANG=en_US.UTF-8

# Work around GitHub Actions git "dubious ownership" error
RUN git config --system --add safe.directory "*"
