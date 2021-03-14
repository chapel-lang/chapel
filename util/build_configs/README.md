## New scripts to build customizable Chapel modules from source

This directory contains scripts to build customizable Chapel binaries
in multiple configurations.

The `cray-internal` subdirectory contains scripts to build and package
Chapel binaries into RPM modules suitable for installation on a Cray-XC
Those scripts are Cray-specific applications of the more general patterns
presented in this directory. See `./cray-internal/README.md` for more info.

### Files in this directory:

* build_configs.py:
  Provides a CLI for building Chapel binaries in multiple configurations.
  Moved here from CHPL_HOME/util and adapted for this application.
  Run with "-h" option for Help.

* setenv-example-\*.bash:
  Example setenv script files
  - setenv-example-1: The simplest tutorial illustrating a trivial Chapel configuration
    (With and without Gasnet; Gasnet with two comm substrates).
  - setenv-example-2: Same trivial Chapel configuration, but a more elaborate scripting style.
  - setenv-example-3: Realistic Chapel configuration for a Cray-XC
    (With and without Cray ugni; Slurm launcher; two target compilers).

* chapel_build.bash:
  Example high-level wrapper script to run a complete Chapel build.
  Run with "-h" option for Help.

* functions.bash: General-purpose shell functions for use in bash scripts

* module-functions.bash: Shell functions to manipulate Cray system modules

* package-functions.bash: General-purpose shell functions for use in Chapel
    packaging scripts; see for example ./cray-internal/chapel_package-cray.bash.

* build-common.bash: Reusable bash code to find existing CHPL_HOME directory
    or create a new one from a given Chapel release tarball.
    See for example chapel_build.bash.

* package-common.bash: Reusable bash code for use in Chapel packaging scripts;
    see for example ./cray-internal/chapel_package-cray.bash.

### Users local Chapel projects:

For convenience, users may keep their own setenv script files, make
logs, CHPL_HOME workspaces, etc in this directory, without adding
them to Git. A hypothetical user story follows.

Let's say you have a Linux cluster with Slurm; you want to build Chapel
binaries with and without Gasnet (where Gasnet may use either of two widely-
supported comm substrates); and you also want the option to use the native
Slurm launcher "slurm-srun", as well as Chapel's default Gasnet launcher(s).
Let's call your new project "slurm".

The Chapel build config matrix shown in setenv-example-1 and -2 seems quite
similar, except it does not include the native Slurm launcher.

To get started, copy file setenv-example-2.bash to a new file "slurm.bash",
and edit the new file.
* Find the build_configs.py command `to make Chapel "runtime"`
* That build_configs.py command line includes existing parameters:
  - `--comm=none,gasnet --launcher=UNSET --substrate=none,$substrates`
* Replace `--launcher=UNSET` with `--launcher=UNSET,slurm-srun`.

This will add another Chapel make (with CHPL_LAUNCHER=slurm-srun) to each
existing Gasnet config:
```
        # CHPL_COMM   CHPL_COMM_SUBSTRATE   CHPL_LAUNCHER   Make Chapel?
        # ---------   -------------------   -------------   -----------
        # none        none                  <default>       Yes
        # none        none                  slurm-srun      Yes (?)
        # none        mpi                   <default>       No  (skip)
        # none        mpi                   slurm-srun      No  (skip)
        # none        udp                   <default>       No  (skip)
        # none        udp                   slurm-srun      No  (skip)
        # gasnet      none                  <default>       No  (skip)
        # gasnet      none                  slurm-srun      No  (skip)
        # gasnet      mpi                   <default>       Yes
        # gasnet      mpi                   slurm-srun      Yes
        # gasnet      udp                   <default>       Yes
        # gasnet      udp                   slurm-srun      Yes
```
It will also add another Chapel make (with slurm-srun) to the existing
non-Gasnet (CHPL_COMM=none) config.
You might want to add something to the setenv callback to skip the relatively-
useless CHPL_COMM=none and CHPL_LAUNCHER=slurm-srun config. But, its optional.

(Please see [Using Chapel](https://chapel-lang.org/docs/usingchapel/launcher.html)
for more information about Slurm launchers.)

You could then try your new setenv script file in an existing Chapel build
workspace:
```
export CHPL_HOME=/your/chapel-home-directory
./slurm.bash
```
Or you could use the existing chapel_build.bash script to create a new Chapel
build workspace from a Chapel source tar file you have downloaded (e.g., from
[GitHub](https://github.com/chapel-lang/chapel/releases/download/1.17.1/chapel-1.17.1.tar.gz)).

The following command will create the new CHPL_HOME under your current
working directory, and then run your "slurm.bash" setenv project:
```
  ./chapel_build.bash -s slurm.bash -t /your/chapel-x.y.z.tar.gz
```

### Discussion

Requirements

* Provide a response to https://github.com/chapel-lang/chapel/issues/9551:
  easy-to-use yet customizable Chapel module build scripts, capable of
  building Chapel binaries in multiple runtime configurations for various
  target platforms.

* The same new build scripts shall replace the scripts currently used with
  the Cray-internal Jenkins CI server to build Chapel Cray modules for
  nightly testing and official release.

Components

* "Make" system. Given an existing Chapel build workspace, run multiple makes
  in all Chapel configurations needed in the desired Chapel module.

  * build_configs.py:
    Runs make for a defined matrix of Chapel build config options, with
    little knowledge of their meaning in Chapel.
    The Chapel build config values are communicated through the OS environment
    seen by each make.
    The OS environment seen by each make can be further customized with setenv
    callbacks (arbitrary bash commands).
    Available options to show printchplenv's in each build config env, instead
    of running the makes.

  * "setenv" scripts:
    Project-specific knowledge, like which Chapel build configs do you want,
    and how do you set up a particular build machine to build each one.
    Keeps project- and site-specific details out of build_configs.py itself.

    - Setenv "project": orchestrates multiple calls to build_configs.py in the
      Chapel build workspace.

      The user runs this script, and this script runs build_config.py,
      as many times as needed, passing the desired Chapel build config
      parameters and make targets each time. It can also make changes
      to the default environment seen by build_config.py.

    - Setenv "callback": arbitrary bash commands run before each make in the
      same OS environment

      This same setenv script file may be source'd by build_config.py, once
      for each Chapel build config, to customize the host environment seen by
      each Chapel make. It can also skip the Chapel make command entirely.

* Cray module packaging script:
  Given an existing build workspace with the makes completed, create the
  Chapel RPM and collateral.

  - 2018-08-21: Now implemented in subdirectory ./cray-internal

* High-level wrappers.

  * For the external user envisioned in #9551:
    A script to unroll a Chapel release tarball, call a setenv project, and
    possibly the Cray module packaging script.

    Example: cray-internal/chapel_build.sh
