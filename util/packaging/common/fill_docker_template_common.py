import os
import itertools
from typing import List, Dict, Union
import textwrap


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

    # generate_run_command_default_config must go last to ensure the package's
    # default configuration is the chapel default configuration, which all the
    # necessary tools.
    substitutions[
        "BUILD_DEFAULT"
    ] = f"""
WORKDIR /home/user/chapel-$CHAPEL_VERSION
{generate_run_command(default_config)}
{generate_run_command(gasnet_udp_config)}
{generate_run_command(ofi_pmi2_config)}
{generate_run_command(gpu_cpu_config)}
{generate_run_command_default_config()}
    """

    # build the minimal configuration
    # which is only the Chapel compiler and runtime
    substitutions[
        "BUILD_ONLY_COMPILER"
    ] = f"""
WORKDIR /home/user/chapel-$CHAPEL_VERSION
{generate_run_command(default_config)}
{generate_run_command(gasnet_udp_config)}
{generate_run_command(ofi_pmi2_config)}
{generate_run_command(gpu_cpu_config)}
    """

    substitutions[
        "INSTALL"
    ] = """
    USER root
    RUN make install
    USER user
    """


    return substitutions

def filter_substitutions(substitutions: Dict[str, str]) -> Dict[str, str]:
    filtered_substitutions = {}
    for key, value in substitutions.items():
        if value.strip():
            filtered_substitutions[key] = textwrap.dedent(value).strip()
        else:
            filtered_substitutions[key] = value
    return filtered_substitutions

default_config = {
    "CHPL_HOST_PLATFORM": "linux64",
    "CHPL_TARGET_PLATFORM": "linux64",
    "CHPL_TARGET_COMPILER": ["llvm", "gnu", "clang"],
    "CHPL_TARGET_CPU": "none",
    "CHPL_LOCALE_MODEL": "flat",
    "CHPL_COMM": "none",
    "CHPL_TASKS": "qthreads",
    "CHPL_LAUNCHER": [None, "slurm-srun"],
    "CHPL_UNWIND": ["none", "bundled"],
    "CHPL_HOST_MEM": "jemalloc",
    "CHPL_TARGET_MEM": ["jemalloc", "cstdlib"],
    "CHPL_ATOMICS": "cstdlib",
    "CHPL_GMP": "bundled",
    "CHPL_HWLOC": "bundled",
    "CHPL_RE2": "bundled",
    "CHPL_LLVM": "system",
    "CHPL_LIB_PIC": "none",
    "CHPL_SANITIZE": "none",
    "CHPL_SANITIZE_EXE": ["none", "address"]
}
gasnet_udp_config = {
    "CHPL_HOST_PLATFORM": "linux64",
    "CHPL_TARGET_PLATFORM": "linux64",
    "CHPL_TARGET_COMPILER": ["llvm", "gnu", "clang"],
    "CHPL_TARGET_CPU": "none",
    "CHPL_LOCALE_MODEL": "flat",
    "CHPL_COMM": "gasnet",
    "CHPL_COMM_SUBSTRATE": "udp",
    "CHPL_GASNET_SEGMENT": "everything",
    "CHPL_TASKS": "qthreads",
    "CHPL_LAUNCHER": "amudprun",
    "CHPL_UNWIND": ["none", "bundled"],
    "CHPL_HOST_MEM": "jemalloc",
    "CHPL_TARGET_MEM": ["jemalloc", "cstdlib"],
    "CHPL_ATOMICS": "cstdlib",
    "CHPL_GMP": "bundled",
    "CHPL_HWLOC": "bundled",
    "CHPL_RE2": "bundled",
    "CHPL_LLVM": "system",
    "CHPL_LIB_PIC": "none",
    "CHPL_SANITIZE": "none",
    "CHPL_SANITIZE_EXE": ["none", "address"]
}
ofi_pmi2_config = {
    "CHPL_HOST_PLATFORM": "linux64",
    "CHPL_TARGET_PLATFORM": "linux64",
    "CHPL_TARGET_COMPILER": ["llvm", "gnu", "clang"],
    "CHPL_TARGET_CPU": "none",
    "CHPL_LOCALE_MODEL": "flat",
    "CHPL_COMM": "ofi",
    "CHPL_LIBFABRIC": "bundled",
    "CHPL_COMM_OFI_OOB": "pmi2",
    "CHPL_TASKS": "qthreads",
    "CHPL_LAUNCHER": [None, "slurm-srun"],
    "CHPL_UNWIND": ["none", "bundled"],
    "CHPL_HOST_MEM": "jemalloc",
    "CHPL_TARGET_MEM": "jemalloc",
    "CHPL_ATOMICS": "cstdlib",
    "CHPL_NETWORK_ATOMICS": "ofi",
    "CHPL_GMP": "bundled",
    "CHPL_HWLOC": "bundled",
    "CHPL_RE2": "bundled",
    "CHPL_LLVM": "system",
    "CHPL_LIB_PIC": "none",
    "CHPL_SANITIZE": "none",
    "CHPL_SANITIZE_EXE": ["none", "address"]
}
gpu_cpu_config = {
    "CHPL_HOST_PLATFORM": "linux64",
    "CHPL_TARGET_PLATFORM": "linux64",
    "CHPL_TARGET_COMPILER": "llvm",
    "CHPL_TARGET_CPU": "none",
    "CHPL_LOCALE_MODEL": "gpu",
    "CHPL_GPU": "cpu",
    "CHPL_COMM": "none",
    "CHPL_TASKS": "qthreads",
    "CHPL_UNWIND": "none",
    "CHPL_HOST_MEM": "cstdlib",
    "CHPL_TARGET_MEM": "jemalloc",
    "CHPL_ATOMICS": "cstdlib",
    "CHPL_GMP": "bundled",
    "CHPL_HWLOC": "bundled",
    "CHPL_RE2": "bundled",
    "CHPL_LLVM": "system",
    "CHPL_LIB_PIC": "none",
    "CHPL_SANITIZE": "none",
    "CHPL_SANITIZE_EXE": "none"
}


def generate_configs(base_config: Dict[str, Union[str,List[str]]]) -> List[Dict[str, str]]:
    incompatibilities = [
        lambda cfg: cfg["CHPL_TARGET_MEM"] == "jemalloc" and cfg["CHPL_SANITIZE_EXE"] != "none",
        lambda cfg: cfg["CHPL_TASKS"] == "qthreads" and cfg["CHPL_SANITIZE_EXE"] != "none",
        lambda cfg: cfg["CHPL_TARGET_COMPILER"] == "llvm" and cfg["CHPL_SANITIZE_EXE"] != "none",
        lambda cfg: cfg["CHPL_TARGET_COMPILER"] == "gnu"
    ]
    keys = list(base_config.keys())

    # generate all combinations of values for the keys in base_config
    configs = []
    for values in itertools.product(*[base_config[key] if isinstance(base_config[key], list) else [base_config[key]] for key in keys]):
        perm = dict(zip(keys, values))
        if any(incompatibility(perm) for incompatibility in incompatibilities):
            continue
        configs.append(perm)
    return configs

def generate_run_command_default_config() -> str:
    """
    Generate a default configuration string for Chapel. This only has 1 runtime
    """
    config = default_config.copy()
    for key, value in config.items():
        if isinstance(value, list):
            value = value[0]
        config[key] = value
    return generate_run_command(config, build_cmd="nice make all chpldoc mason chplcheck chpl-language-server -j$PARALLEL")

def generate_run_command(base_config: Dict[str, Union[str,List[str]]], build_cmd="nice make all -j$PARALLEL") -> str:
    configs = generate_configs(base_config)

    run_commands = []
    for config in configs:
            command_prefix = textwrap.dedent(f"""
            RUN export CHPL_HOME=/home/user/chapel-$CHAPEL_VERSION && \\
              rm -f $CHPL_HOME/chplconfig && touch $CHPL_HOME/chplconfig && \\""").strip()

            command_postfix = textwrap.indent(textwrap.dedent(f"""
            echo "CHPL_LLVM_CONFIG=$(which llvm-config)" >>$CHPL_HOME/chplconfig && \\
            ./configure --prefix=/usr && \\
            {build_cmd} && \\
            unset CHPL_HOME""").strip(), "  ")

            command_body = "\n".join(
                f'  echo "{key}={value}" >> $CHPL_HOME/chplconfig && \\'
                for key, value in config.items() if value is not None
            )
            run_command = f"{command_prefix}\n{command_body}\n{command_postfix}"
            run_commands.append(run_command)
    return "\n\n".join(run_commands) + "\n"
