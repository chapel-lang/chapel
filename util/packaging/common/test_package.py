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

global chpl_home
chpl_home = os.environ.get("CHPL_HOME", "")

def run_command(cmd, **kwargs):
    if verbose:
        print(f"Running command: \"{' '.join(cmd)}\"")
    return sp.check_call(cmd, **kwargs)

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


def infer_docker_os(package):
    os_tag_to_docker = {
        "el9": "rockylinux/rockylinux:9",
        "fc37": "fedora:37",
        "fc38": "fedora:38",
        "fc39": "fedora:39",
        "fc40": "fedora:40",
        "amzn2023": "amazonlinux:2023",
        "ubuntu22": "ubuntu:22.04",
        "ubuntu24": "ubuntu:24.04",
        "debian11": "debian:bullseye",
        "debian12": "debian:bookworm",
    }
    for tag, docker in os_tag_to_docker.items():
        if ".{}.".format(tag) in package:
            return docker
    return ValueError(f"Could not infer docker image from package {package}")

def infer_env_vars(package):
    if "gasnet-udp" in package:
        return """
ENV GASNET_SPAWNFN=L
ENV GASNET_ROUTE_OUTPUT=0
ENV GASNET_QUIET=Y
ENV GASNET_MASTERIP=127.0.0.1
ENV GASNET_WORKERIP=127.0.0.0
ENV CHPL_RT_OVERSUBSCRIBED=yes
"""

    return ""

def infer_pkg_type(package):
    if package.endswith(".deb"):
        return "apt"
    elif package.endswith(".rpm"):
        return "rpm"
    else:
        raise ValueError(f"Package {package} is not a .deb or .rpm file")


def build_docker(test_dir, package_path, package_name, docker_os):
    template = ""
    template_file = os.path.join(test_dir, "Dockerfile.template")
    with open(template_file, "r") as f:
        template = f.read()

    substitutions = {
        "OS_BASE_IMAGE": docker_os,
        "HOST_PACKAGE_PATH": package_path,
        "PACKAGE_NAME": package_name,
        "TEST_ENV": infer_env_vars(package_name),
    }


    src = MyTemplate(template)
    result = src.safe_substitute(substitutions)

    output_file = os.path.join(test_dir, "Dockerfile")
    with open(output_file, "w") as f:
        f.write(result)


    # now invoke the proper script to fill in the rest of the Dockerfile
    pkg_type = infer_pkg_type(package_name)
    fill_script = "{}/util/packaging/{}/common/fill_docker_template.py".format(chpl_home, pkg_type)
    run_command(["python3", fill_script, output_file])


def docker_build_image(
    test_dir, package, docker_os, imagetag="chapel-test-image"
):
    context = os.path.join(test_dir, "..", "..")
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
        "--load",
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
    parser.add_argument(
        "--dockeros", type=str, default=None, help="The docker image to use"
    )
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

    package = os.path.abspath(os.path.expanduser(args.package))
    pkg_type = infer_pkg_type(package)
    test_dir = os.path.join(chpl_home, "util", "packaging", pkg_type, "test")
    test_dir = os.path.abspath(test_dir)

    # TODO: need to figure out how to test the ofi-slurm package automatically
    if "ofi-slurm" in package:
        print("Skipping ofi-slurm package")
        return

    docker_os = args.dockeros
    if docker_os is None:
        docker_os = infer_docker_os(package)

    imagetag = docker_build_image(test_dir, package, docker_os)
    if args.run:
        docker_run_container(imagetag)
    cleanup(test_dir, imagetag)


if __name__ == "__main__":
    main()
