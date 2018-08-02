## New scripts to build customizable Chapel module from source

Work-in-progress (2018-07-31)

### Files in this directory:

* build_configs.py:
  Provides a CLI for building Chapel binaries in multiple configurations.
  Moved here from CHPL_HOME/util and adapted for this application.

* setenv-example-*.bash:
  Example setenv script files

* chapel_build.bash:
  Example high-level wrapper script to run a complete build process

* functions.bash: Misc shell functions for use in other bash scripts

* Users local Chapel projects:

  For convenience, users may keep their own setenv script files, make
  logs, CHPL_HOME workspaces, etc in this directory, without adding
  them to Git.

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

