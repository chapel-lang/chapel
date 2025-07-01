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
    usermod -aG sudo user && \\
    echo "user:password" | chpasswd
USER user
WORKDIR /home/user
"""

substitutions["PACKAGE_SETUP"] = """
WORKDIR /home/user

COPY --chown=user ./apt/$DOCKER_DIR_NAME/control.template /home/user/control.template
COPY --chown=user ./apt/common/make_dirs.py /home/user/make_dirs.py
COPY --chown=user ./common/package_name.py /home/user/package_name.py
RUN python3 make_dirs.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH

COPY --chown=user ./common/fixpaths.py /home/user/fixpaths.py
USER root
RUN python3 fixpaths.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH
USER user

COPY --chown=user ./apt/common/copy_files.py /home/user/copy_files.py
RUN python3 copy_files.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH
"""

substitutions[
    "PACKAGE_BUILD"
] = """
WORKDIR /home/user
RUN dpkg-deb --build $(python3 package_name.py $BASENAME $CHAPEL_VERSION $PACKAGE_VERSION $OS_NAME $TARGETARCH)

FROM scratch AS artifact
COPY --from=build /home/user/*.deb /

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
