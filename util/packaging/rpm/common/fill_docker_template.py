#!/usr/bin/env python3
import os
import argparse
from string import Template


class MyTemplate(Template):
    delimiter = "@@"


substitutions = dict()
substitutions[
    "ARGUMENTS"
] = """
ARG BASENAME=chapel
ARG CHAPEL_VERSION=2.1.0
ARG PACKAGE_VERSION=1
ARG OS_NAME
ARG PARALLEL=1
ARG TARGETARCH
"""

substitutions[
    "USER_CREATION"
] = """
RUN useradd -ms /bin/bash user && \\
    usermod -aG wheel user && \\
    echo "user:password" | chpasswd && \\
    sed -i 's/%wheel[ \\t]\\{1,\\}ALL=(ALL)[ \\t]\\{1,\\}ALL/%wheel ALL=(ALL) NOPASSWD: ALL/g' /etc/sudoers

USER user
WORKDIR /home/user
"""

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
    ./configure --prefix=/usr && \\
    nice make all chpldoc mason chplcheck chpl-language-server -j$PARALLEL && \\
    unset CHPL_HOME
"""

substitutions["BUILD_GASNET_UDP"] = """
RUN export CHPL_HOME=/home/user/chapel-$CHAPEL_VERSION && \\
    export CHPL_COMM=gasnet && \\
    ./configure --prefix=/usr && \\
    nice make all chpldoc mason chplcheck chpl-language-server -j$PARALLEL && \\
    unset CHPL_HOME && \\
    unset CHPL_COMM
"""

substitutions["BUILD_OFI_SLURM"] = """
RUN export CHPL_HOME=/home/user/chapel-$CHAPEL_VERSION && \\
    export CHPL_COMM=ofi && \\
    export CHPL_LAUNCHER=slurm-srun && \\
    export CHPL_COMM_OFI_OOB=pmi2 && \\
    export CHPL_LIBFABRIC=bundled && \\
    ./configure --prefix=/usr && \\
    nice make all chpldoc mason chplcheck chpl-language-server -j$PARALLEL && \\
    unset CHPL_HOME && \\
    unset CHPL_COMM && \\
    unset CHPL_LAUNCHER && \\
    unset CHPL_COMM_OFI_OOB && \\
    unset CHPL_LIBFABRIC
"""

substitutions[
    "INSTALL"
] = """
USER root
RUN make install
USER user
"""

substitutions["PACKAGE_SETUP"] = """
WORKDIR /home/user

COPY --chown=user ./rpm/$OS_NAME/spec.template /home/user/spec.template
COPY --chown=user ./rpm/common/make_spec.py /home/user/make_spec.py
COPY --chown=user ./common/package_name.py /home/user/package_name.py
RUN python3 make_spec.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH

COPY --chown=user ./rpm/common/rpmlintrc /home/user/.rpmlintrc
RUN rpmdev-setuptree && \\
    cp chapel-$CHAPEL_VERSION.tar.gz $(rpm --eval '%{_sourcedir}') && \\
    rpmlint --file .rpmlintrc $BASENAME.spec && \\
    spectool -g -R $BASENAME.spec

COPY --chown=user ./common/fixpaths.py /home/user/fixpaths.py
USER root
RUN python3 fixpaths.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH
USER user
"""

substitutions[
    "PACKAGE_BUILD"
] = """
RUN rpmbuild -ba $BASENAME.spec && \\
    cp $(rpm --eval '%{_rpmdir}')/$(rpm --eval '%{_arch}')/*.rpm .

FROM scratch as artifact
COPY --from=build /home/user/*.rpm /

FROM build as release
"""


def fill_docker_template(template_file):

    template = ""
    with open(template_file, "r") as f:
        template = f.read()

    src = MyTemplate(template)
    result = src.substitute(substitutions)

    dirname = os.path.dirname(template_file)
    output_file = os.path.join(dirname, "Dockerfile")
    with open(output_file, "w") as f:
        f.write(result)


def main():
    parser = argparse.ArgumentParser(description="Fill Docker template")
    parser.add_argument("template_file", type=str, help="Template file to fill")
    args = parser.parse_args()
    template_file = os.path.abspath(os.path.expanduser(args.template_file))
    fill_docker_template(template_file)


if __name__ == "__main__":
    main()
