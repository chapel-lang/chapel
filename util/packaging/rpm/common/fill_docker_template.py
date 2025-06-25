#!/usr/bin/env python3
import os
import sys
import argparse
from string import Template

chpl_home = os.environ.get("CHPL_HOME")
if chpl_home is None:
    raise ValueError("CHPL_HOME is not set")
sys.path.append(os.path.join(chpl_home, "util", "packaging", "common"))
try:
    from fill_docker_template_common import common_substitutions, filter_substitutions
except ImportError:
    raise ImportError("fill_docker_template_common.py not found")

class MyTemplate(Template):
    delimiter = "@@"


substitutions = common_substitutions()

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


substitutions["PACKAGE_SETUP"] = """
WORKDIR /home/user

COPY --chown=user ./rpm/$DOCKER_DIR_NAME/spec.template /home/user/spec.template
COPY --chown=user ./rpm/common/make_spec.py /home/user/make_spec.py
COPY --chown=user ./common/package_name.py /home/user/package_name.py
RUN python3 make_spec.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH

COPY --chown=user ./rpm/$DOCKER_DIR_NAME/rpmlintrc /home/user/.rpmlintrc
RUN rpmdev-setuptree && \\
    cp chapel-$CHAPEL_VERSION.tar.gz $(rpm --eval '%{_sourcedir}') && \\
    ignore_unused=$([[ "$(rpm --eval '%{dist_name}')" == "Fedora Linux" ]] && echo "--ignore-unused-rpmlintrc") || echo "" && \\
    rpmlint $ignore_unused --file .rpmlintrc $BASENAME.spec && \\
    unset ignore_unused && \\
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

FROM scratch AS artifact
COPY --from=build /home/user/*.rpm /

FROM build AS release
"""

substitutions = filter_substitutions(substitutions)


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
