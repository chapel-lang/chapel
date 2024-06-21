import os

def common_substitutions():
    substitutions = dict()

    substitutions[
    "FROM"
    ] = """
ARG DOCKER_IMAGE_NAME_FULL=scratch
FROM $DOCKER_IMAGE_NAME_FULL AS build
"""

    substitutions[
        "ARGUMENTS"
    ] = """
    ARG BASENAME=chapel
    ARG CHAPEL_VERSION=2.1.0
    ARG PACKAGE_VERSION=1
    ARG OS_NAME
    ARG DOCKER_DIR_NAME
    ARG PARALLEL=1
    ARG TARGETARCH
    """

    substitutions["INJECT_BEFORE_DEPS"] = ""
    inject = os.environ.get("INJECT_BEFORE_DEPS")
    if inject is not None:
        substitutions["INJECT_BEFORE_DEPS"] = inject

    substitutions[
        "GET_CHAPEL"
    ] = """
    COPY --chown=user ./tarballs/chapel-$CHAPEL_VERSION.tar.gz /home/user/chapel-$CHAPEL_VERSION.tar.gz
    RUN tar xf chapel-$CHAPEL_VERSION.tar.gz
    """

    substitutions[
        "BUILD_DEFAULT"
    ] = """
    WORKDIR /home/user/chapel-$CHAPEL_VERSION
    RUN export CHPL_HOME=/home/user/chapel-$CHAPEL_VERSION && \\
        rm -f $CHPL_HOME/chplconfig && touch $CHPL_HOME/chplconfig && \\
        echo "CHPL_COMM=none" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_HOST_PLATFORM=linux64" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_PLATFORM=linux64" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_COMPILER=llvm" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_CPU=none" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TASKS=qthreads" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_RE2=bundled" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_GMP=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_MEM=jemalloc" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_JEMALLOC=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_HWLOC=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_LLVM=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_LLVM_CONFIG=$(which llvm-config)" >> $CHPL_HOME/chplconfig && \\
        ./configure --prefix=/usr && \\
        nice make all chpldoc mason chplcheck chpl-language-server -j$PARALLEL && \\
        unset CHPL_HOME
    """

    substitutions["BUILD_GASNET_UDP"] = """
    RUN export CHPL_HOME=/home/user/chapel-$CHAPEL_VERSION && \\
        rm -f $CHPL_HOME/chplconfig && touch $CHPL_HOME/chplconfig && \\
        echo "CHPL_COMM=gasnet" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_HOST_PLATFORM=linux64" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_PLATFORM=linux64" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_COMPILER=llvm" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_CPU=none" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TASKS=qthreads" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_RE2=bundled" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_GMP=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_MEM=jemalloc" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_JEMALLOC=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_HWLOC=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_LLVM=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_LLVM_CONFIG=$(which llvm-config)" >> $CHPL_HOME/chplconfig && \\
        ./configure --prefix=/usr && \\
        nice make all chpldoc mason chplcheck chpl-language-server -j$PARALLEL && \\
        unset CHPL_HOME
    """

    substitutions["BUILD_OFI_SLURM"] = """
    RUN export CHPL_HOME=/home/user/chapel-$CHAPEL_VERSION && \\
        rm -f $CHPL_HOME/chplconfig && touch $CHPL_HOME/chplconfig && \\
        echo "CHPL_COMM=ofi" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_LAUNCHER=slurm-srun" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_LIBFABRIC=bundled" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_COMM_OFI_OOB=pmi2" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_HOST_PLATFORM=linux64" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_PLATFORM=linux64" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_COMPILER=llvm" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_CPU=none" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TASKS=qthreads" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_RE2=bundled" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_GMP=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_MEM=jemalloc" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_TARGET_JEMALLOC=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_HWLOC=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_LLVM=system" >> $CHPL_HOME/chplconfig && \\
        echo "CHPL_LLVM_CONFIG=$(which llvm-config)" >> $CHPL_HOME/chplconfig && \\
        ./configure --prefix=/usr && \\
        nice make all chpldoc mason chplcheck chpl-language-server -j$PARALLEL && \\
        unset CHPL_HOME
    """


    substitutions[
        "INSTALL"
    ] = """
    USER root
    RUN make install
    USER user
    """


    return substitutions
