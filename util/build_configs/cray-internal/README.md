## New scripts to build and package Chapel binaries into a Cray-compatible RPM.

This directory adds scripts to build and package Chapel binaries as an RPM file.
The recommended way to build Chapel binaries is to run `chapel_build.bash` with
one of the setenv scripts in this subdir, setenv-example-3 from the parent directory,
or a new setenv script derived from one of those. See `chapel_build.bash` for illustration.

The Chapel RPM file produced by these scripts is intended to be fully compatible
with current generation Cray-XC/XE supercomputers and system-management tools.
As far as the packaging is concerned, these RPMs should be interchangeable with the
officially released Chapel modules for Cray systems.

These scripts are designed to work with some more-generic Chapel package support scripts
located in the parent directory. The idea is to implement package-specific details in
a subdirectory like this one (cray-internal), while generic things that should be
applicable in many kinds of Chapel packages are implemented in the parent directory.

This directory should be used as a model for new scripts to build other kinds of Chapel
packages. Start by creating a new subdirectory (under build_configs) for each new type
of package; copy and rename the main script `chapel_package-cray.bash` from here to the
new subdir; edit that file; and go from there, copy-editing additional files from this
directory as needed. When appropriate, add to the existing `package-*.bash` support
files in the parent directory. Finally, copy-edit the `.gitignore` file from here to the
new subdir.

### Files in this directory:

* chapel_build.bash:
  Example high-level wrapper script to run a complete Chapel build from source and
  create a Cray-compatible Chapel RPM.  Run with "-h" option for Help.

* chapel_package-cray.bash:
  The primary script to build a Chapel RPM in this subdirectory.
  Run with "-h" option for Help.

* common.bash: Cray-module-specific bash code snippets.
  See for example chapel_package-cray.bash.

* generate-\*.bash:
  Scripts to generate various files used in Cray RPMs.
  - generate-dev-releaseinfo.bash: Release_info is a human readable text file found in
    Cray software releases. This generater produces a release_info file suitable as a draft
    or for development purposes. Does NOT produce the release_info used in official releases.
  - generate-modulefile.bash: The Cray modulefile, installed by the Chapel RPM.
    Supports "module load chapel" on Cray computers.
  - generate-rpmspec.bash: The RPM "spec" file read by the rpmbuild tool.
  - generate-set_default.bash: The Cray set_default file, installed by the Chapel RPM.
    A Cray system management tool, this script sets the default Chapel version obtained
    when a user runs `module load chapel` without specifying a specific version.

* setenv-\*-\*.bash:
  Setenv scripts for Chapel Cray RPMs of various types. These setenv scripts are based on
  the setenv-example-3 script in the parent directory, and their basic structure is the
  same.  However, many details were added to successfully build a complete Chapel Cray
  module, including Python-venv, chpldoc, mason, over 100 runtime configs, etc.
  These setenv scripts are located here in this `cray-internal` subdir because they
  are Cray-specific implementations of the general build_configs/setenv pattern:
  - setenv-xc-x86_64:  Chapel Cray-XC module for x86_64
  - setenv-xc-aarch64: Chapel Cray-XC module for aarch64 (ARM)
  - setenv-xe-x86_64:  Chapel Cray-XE module for x86_64

### Users local Chapel projects:

For convenience, users may keep their own Chapel workspaces and project files in
this subdirectory, without adding them to Git. A hypothetical user story follows.

Let's say you have a Cray-XC supercomputer on which Chapel is no longer officially supported. You want to build Chapel from source (a Chapel release tarball like [chapel-1.17.1.tar.gz](https://github.com/chapel-lang/chapel/releases/download/1.17.1/chapel-1.17.1.tar.gz) from GitHub);
create a Chapel RPM from those binaries; and install that RPM on your Cray-XC system.
Users on your Cray-XC system should only need to `module load chapel` to access your
locally-built Chapel version, same as other supported computing languages.

Start in the parent directory (build_configs) with file `setenv-example-3.bash`. Copy
`setenv-example-3.bash` to a new file and run it (with changes as needed) until it
produces the set of Chapel binaries that you want. See the README, `setenv-example-3.bash`
and `chapel_build.bash` files in the parent directory for more information.

When satisfied with your locally-built Chapel binaries, cd back to this directory and run
`chapel_package-cray.bash`, with the CHPL_HOME environment variable pointing to the Chapel
build workspace you created above, and with -s pointing to your final setenv script.

Finally, to execute a complete end-to-end Chapel build and package in one command, you
would switch from `chapel_package-cray.bash` to `chapel_build.bash` in this directory.

For example, if the existing `setenv-example-3` script file produced the Chapel binaries
that you want, and if the Chapel source tarball was saved locally in
`/your/chapel-release.tar.gz`, you could cd to this directory and run:
```
./chapel_build.bash -s ../setenv-example-3.bash -t /your/chapel-x.y.z.tar.gz -b nightly
```
The resulting RPM file (created in this directory) would be the same as from these commands:
```
../chapel_build.bash -s ../setenv-example-3.bash -t /your/chapel-x.y.z.tar.gz

./chapel_package-cray.bash -b nightly
```
... or from these commands:
```
rm -rf chapel-x.y.z
tar -xf /your/chapel-x.y.z.tar.gz
export CHPL_HOME=$PWD/chapel-x.y.z

../chapel_build.bash -s ../setenv-example-3.bash

./chapel_package-cray.bash -b nightly
```

Instead of starting from `setenv-example-3`, a user with more sophisticated requirements
could start from one of the setenv scripts in this subdirectory: `setenv-xc-x86_64`, for
example. This would be more challenging, as these setenv scripts are necessarily more
complicated and site-specific than `setenv-example-3`.
However, the same basic approach would be used: copy the existing setenv script to a new
filename; modify as needed; test-build-test; add packaging; etc.
