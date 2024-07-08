#!/usr/bin/env python3
"""
Run tests on built packages
"""
import sys
import argparse
import os
import subprocess as sp
from string import Template


class MyTemplate(Template):
    delimiter = "@@"


global verbose
verbose = False


def run_command(cmd, **kwargs):
    if verbose:
        print(f"Running command: \"{' '.join(cmd)}\"")
    return sp.check_call(cmd, **kwargs)


def build_docker(test_dir, package_path, package_name, docker_os):
    template = ""
    template_file = os.path.join(test_dir, "Dockerfile.template")
    with open(template_file, "r") as f:
        template = f.read()

    substitutions = {
        "OS_BASE_IMAGE": docker_os,
        "HOST_PACKAGE_PATH": package_path,
        "PACKAGE_NAME": package_name,
    }

    src = MyTemplate(template)
    result = src.substitute(substitutions)

    output_file = os.path.join(test_dir, "Dockerfile")
    with open(output_file, "w") as f:
        f.write(result)


def determine_arch(package):
    # if the arch is aarch64 or arm64, return arm64
    # if the arch is x86_64 or amd64, return amd64
    # otherwise return the components just before the suffix

    arch = os.path.basename(package).split(".")[-2]
    if arch in ["aarch64", "arm64"]:
        return "arm64"
    elif arch in ["x86_64", "amd64"]:
        return "amd64"
    else:
        return arch


def docker_build_image(
    test_dir, package, docker_os, imagetag="chapel-test-image"
):
    context = os.path.join(test_dir, "..")
    context = os.path.abspath(context)
    dockerfile = os.path.join(test_dir, "Dockerfile")

    # check that the package is in the context directory
    rel = os.path.relpath(package, context)
    if rel.startswith(".."):
        print(f"Package {package} is not in the context directory {context}")
        sys.exit(1)

    build_docker(
        test_dir, os.path.dirname(rel), os.path.basename(rel), docker_os
    )

    platform = f"linux/{determine_arch(package)}"

    cmd = [
        "docker",
        "buildx",
        "build",
        "--platform",
        platform,
        "-t",
        imagetag,
        "-f",
        dockerfile,
        context,
    ]
    run_command(cmd)

    return imagetag


def docker_run_container(imagetag):
    cmd = ["docker", "run", "--rm", "-it", imagetag]
    run_command(cmd)


def cleanup(test_dir, imagetag):
    cmd = ["docker", "image", "rm", imagetag]
    run_command(cmd)
    os.remove(os.path.join(test_dir, "Dockerfile"))


def main():
    parser = argparse.ArgumentParser(description="Run tests on build packages")
    parser.add_argument("package", type=str, help="The package to test")
    parser.add_argument("dockeros", type=str, help="The docker image to use")
    parser.add_argument(
        "--run",
        action="store_true",
        help="Run the container after building the image",
    )
    parser.add_argument(
        "--verbose", action="store_true", help="Print verbose output"
    )
    args = parser.parse_args()
    global verbose
    verbose = args.verbose

    chpl_home = os.environ.get("CHPL_HOME", "")
    package = os.path.abspath(os.path.expanduser(args.package))
    test_dir = os.path.join(chpl_home, "util", "packaging")
    if package.endswith(".deb"):
        test_dir = os.path.join(test_dir, "apt")
    elif package.endswith(".rpm"):
        test_dir = os.path.join(test_dir, "rpm")
    else:
        print(f"Package {package} is not a .deb or .rpm file")
        sys.exit(1)
    test_dir = os.path.join(test_dir, "test")
    test_dir = os.path.abspath(test_dir)
    docker_os = args.dockeros

    imagetag = docker_build_image(test_dir, package, docker_os)
    if args.run:
        docker_run_container(imagetag)
    cleanup(test_dir, imagetag)


if __name__ == "__main__":
    main()
