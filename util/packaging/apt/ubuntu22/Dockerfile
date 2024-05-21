FROM ubuntu:22.04 as build

ARG BASENAME=chapel
ARG CHAPEL_VERSION=2.0.0
ARG PACKAGE_VERSION=1
ARG OS_NAME=ubuntu22
ARG PARALLEL=1
ARG TARGETARCH

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y \
      curl wget vim sudo gcc g++ m4 perl chrpath \
      python3 python3-dev python3-venv bash make mawk git pkg-config cmake \
      llvm-dev llvm clang libclang-dev libclang-cpp-dev libedit-dev

RUN useradd -ms /bin/bash user && \
    usermod -aG sudo user && \
    echo "user:password" | chpasswd

USER user
WORKDIR /home/user

COPY --chown=user ./tarballs/chapel-$CHAPEL_VERSION.tar.gz /home/user/chapel-$CHAPEL_VERSION.tar.gz
RUN tar xf chapel-$CHAPEL_VERSION.tar.gz

WORKDIR /home/user/chapel-$CHAPEL_VERSION
RUN export CHPL_HOME=/home/user/chapel-$CHAPEL_VERSION && \
    ./configure --prefix=/usr && \
    nice make all chpldoc mason chplcheck chpl-language-server -j$PARALLEL && \
    unset CHPL_HOME

USER root
RUN make install
USER user

WORKDIR /home/user

COPY --chown=user ./apt/$OS_NAME/control.template /home/user/control.template
COPY --chown=user ./apt/common/make_dirs.py /home/user/make_dirs.py
COPY --chown=user ./common/package_name.py /home/user/package_name.py
RUN python3 make_dirs.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH

COPY --chown=user ./common/fixpaths.py /home/user/fixpaths.py
USER root
RUN python3 fixpaths.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH
USER user

# copy installed files
COPY --chown=user ./apt/common/copy_files.py /home/user/copy_files.py
RUN python3 copy_files.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH

WORKDIR /home/user
# build the deb package
RUN dpkg-deb --build $(python3 package_name.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH)

FROM scratch as artifact
COPY --from=build /home/user/*.deb /

FROM build as release
