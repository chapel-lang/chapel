## New scripts to package pre-built Chapel binaries into a Cray-compatible RPM.

This directory adds scripts to package pre-built Chapel binaries as an RPM file.
The recommended way to build Chapel binaries is to run `chapel_build.bash` from
the parent directory, with setenv-example-3. See chapel_build.bash for illustration.

The Chapel RPM file produced by these scripts is intended to be fully compatible
with current generation Cray-XC supercomputers and system-management tools.
As far as the packaging is concerned, these RPMs should be interchangeable with the
Chapel Cray modules released through Cray, Inc.

These scripts are designed to work with some more-generic Chapel package support scripts
located in the parent directory. The idea is to implement package-specific details in
a subdirectory like this one (cray-internal), while generic things that should be
applicable in many kinds of Chapel packages are implemented in the parent directory.

This directory should be used as a model for new scripts to build other kinds of Chapel
packages. Start by creating a new subdirectory (under build_configs) for each specific
Chapel package; copy and rename the main script `chapel_package-cray.bash` from here
to the new subdir; edit that file; and go from there, copy-editing additional files from
this directory as needed. When appropriate, add to the existing `package-*.bash` support
files in the parent directory.

### Files in this directory:

* chapel_build.bash:
  Example high-level wrapper script to run a complete Chapel build from source and
  create a Cray-compatible Chapel RPM.

* chapel_package-cray.bash:
  The primary script to build a Chapel RPM in this subdirectory.

* generate-*.bash:
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

* package-functions.bash: General-purpose shell functions for use in Chapel
    packaging scripts; see for example ./cray-internal/chapel_package-cray.bash.

* common.bash: Cray-module-specific bash code snippets.
    See for example chapel_package-cray.bash.

### Users local Chapel projects:

For convenience, users may keep their own Chapel workspaces and project files in
this subdirectory, without adding them to Git. A hypothetical user story follows.

Let's say you have a Cray-XC supercomputer that Cray no longer provides Chapel releases
for. You want to build Chapel from source (a Chapel release tarball like [chapel-1.17.1.tar.gz](https://github.com/chapel-lang/chapel/releases/download/1.17.1/chapel-1.17.1.tar.gz) from GitHub);
create a Chapel RPM from those binaries; and install that RPM on your Cray-XC system.
Users on your Cray-XC system should only need to `module load chapel` to access your
locally-built Chapel version, same as other supported computing languages.

Start in the parent directory (build_configs) with file `setenv-example-3.bash`. Copy 
setenv-example-3.bash to a new file and run it with changes, until you get the set of Chapel
binaries you want. See the README, `setenv-example-3.bash` and `chapel_build.bash` files
in the parent directory for more information.

When satisfied with your locally-built Chapel binaries, cd back to this directory and run
`chapel_package-cray.bash`, with the CHPL_HOME environment variable pointing to the Chapel
build workspace you created above, and with -s pointing to your final setenv script.

For example, if the existing `setenv-example-3` script produced the desired Chapel binaries,
and the Chapel source was saved locally in `/your/chapel-release.tar.gz`:
```
./chapel_build.bash -s ../setenv-example-3.bash -t /your/chapel-release.tar.gz
```


