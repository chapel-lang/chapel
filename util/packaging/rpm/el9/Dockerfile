FROM rockylinux/rockylinux:9 as build

ARG BASENAME=chapel
ARG CHAPEL_VERSION=2.0.0
ARG PACKAGE_VERSION=1
ARG OS_NAME=el9
ARG PARALLEL=1
ARG TARGETARCH

RUN dnf upgrade -y && \
    dnf install -y --allowerasing \
      gcc gcc-c++ m4 perl python3 python3-devel bash make gawk git cmake \
      which diffutils wget vim sudo \
      llvm-devel clang clang-devel \
      rpm-build rpm-devel rpmlint coreutils patch rpmdevtools chrpath

RUN useradd -ms /bin/bash user && \
    usermod -aG wheel user && \
    echo "user:password" | chpasswd && \
    sed -i 's/%wheel[ \t]\{1,\}ALL=(ALL)[ \t]\{1,\}ALL/%wheel ALL=(ALL) NOPASSWD: ALL/g' /etc/sudoers

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

COPY --chown=user ./rpm/$OS_NAME/spec.template /home/user/spec.template
COPY --chown=user ./rpm/common/make_spec.py /home/user/make_spec.py
COPY --chown=user ./common/package_name.py /home/user/package_name.py
RUN python3 make_spec.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH

COPY --chown=user ./rpm/common/rpmlintrc /home/user/.rpmlintrc
RUN rpmdev-setuptree && \
    cp chapel-$CHAPEL_VERSION.tar.gz $(rpm --eval '%{_sourcedir}') && \
    rpmlint -f .rpmlintrc $BASENAME.spec && \
    spectool -g -R $BASENAME.spec

COPY --chown=user ./common/fixpaths.py /home/user/fixpaths.py
USER root
RUN python3 fixpaths.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH
USER user

# hot fixes for rpm, can be removed in 2.1
USER root
RUN short_version=$(python3 package_name.py --short-version $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH) && \
    sed -i 's|#!/usr/bin/env python|#!/usr/bin/env python3|' /usr/share/chapel/${short_version}/util/config/compileline.py && \
    unset short_version
USER user

RUN rpmbuild -ba $BASENAME.spec && \
    cp $(rpm --eval '%{_rpmdir}')/$(rpm --eval '%{_arch}')/*.rpm .

FROM scratch as artifact
COPY --from=build /home/user/*.rpm /

FROM build as release
