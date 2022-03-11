# This is a container used to run CI jobs on GH
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
    m4 \
    make \
    mawk \
    perl \
    pkg-config \
    python3 \
    python3-dev \
    python3-pip \
    python3-venv
