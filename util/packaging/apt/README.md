# Chapel APT Packages

This directory contains the scripts necessary to build Chapel apt packages.

## Directory Structure

Each OS should have its own directory containing the scripts necessary to build
the Chapel apt packages for that OS. Each one should have a Dockerfile that can
be used to build the package in a clean environment.

The `common` directory contains the common scripts and configuration files used
by all the packages. The docker builds use the `apt` directory as the context and pulls in these files as needed.
