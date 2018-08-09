## New scripts to build customizable Chapel module from source

Work-in-progress

### Files in this directory:

* build_configs.py:
  Provides a CLI for building Chapel binaries in multiple configurations.
  Moved here from CHPL_HOME/util and adapted for this application.

* setenv-example-*.bash:
  Example setenv script files
  - setenv-example-1: The simplest tutorial illustrating a trivial Chapel configuration.
  - setenv-example-2: Same trivial Chapel configuration, but a more elaborate scripting style.
  - setenv-example-3: Realistic Chapel configuration for a Cray-XC.

* chapel_build.bash:
  Example high-level wrapper script to run a complete build process

* functions.bash: General-purpose shell functions for use in bash scripts

* module_functions.bash: Shell functions to manipulate Cray system modules

* Users local Chapel projects:

  For convenience, users may keep their own setenv script files, make
  logs, CHPL_HOME workspaces, etc in this directory, without adding
  them to Git.

  Let's say your host machine has Slurm; you want a Chapel multi-config
  build with and without Gasnet (where Gasnet may use either of two widely-
  supported comm layers); and you also want the option to use the native
  Slurm launcher slurm-srun as well as Chapel's default Gasnet launcher(s).
  Let's call your new project "slurm".

  The existing setenv-example-1.bash script seems quite similar, except for
  using Slurm.

  To get started, copy setenv-example-1.bash to a new file "slurm.bash" and
  add an additional command line parameter "--launcher=UNSET,slurm-srun" to
  the first build_configs.py command in the file.

  This will add another Chapel make (with CHPL_LAUNCHER=slurm-srun) to each
  existing Gasnet config. It will also add another Chapel make (with slurm-srun)
  to the existing non-Gasnet (CHPL_COMM=none) config- a small waste of time!
  You might want to add something to the setenv callback to skip useless builds
  with CHPL_COMM=none and CHPL_LAUNCHER=<any non-null value>. But, its optional.

  Finally, replace all references to "setenv-example-1.bash" in your file with
  "slurm.bash", so that build_configs.py will use the callback scripts from your
  file instead of the original setenv-example-1.bash.

  You could then try your new setenv script file in an existing Chapel build
  workspace:

```
    export CHPL_HOME=/your/chapel-home-directory
    ./slurm.bash
```

  Or you could use the existing chapel_build.bash script to create a new Chapel
  build workspace from a Chapel source tar file, and then run your new setenv
  script:

```
    ./chapel_build.bash -s slurm.bash -t /your/chapel-release.tar.gz
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

* Cray module packaging script. (2018-07-31: not available yet)
  Given an existing build workspace with the makes completed, create the
  Chapel RPM and collateral.

* High-level wrappers.

  * For the external user envisioned in #9551:
    A script to unroll a Chapel release tarball, call a setenv project, and
    possibly the Cray module packaging script.

    WIP example: chapel_build.sh

  * For Cray-internal Jenkins build script replacement:
    A script to build a new (or unroll a previously-built) Chapel release tarball;
    call a different setenv project, depending on Cray-XC or XE; and
    call the Cray module packaging script.

