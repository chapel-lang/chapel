# Getting started with singularity

See https://docs.sylabs.io/guides/latest/user-guide/

# Where to find images

* You can use 'singularity search' to look for containers;
  and see default library is https://cloud.sylabs.io/library/library/
  e.g. https://cloud.sylabs.io/library/library/default/debian
* You can find docker containers on https://hub.docker.com

# Notes on writing .def files

## Example using a singularity library image
BootStrap: library
From: ubuntu:16.04

## Example using a docker image
BootStrap: docker
From: ubuntu:16.04

## Example using a docker image from quay.io
BootStrap: docker
From: quay.io/centos/centos:stream9

## Notes about command sections
%post commands run as root
%runscript commands run as regular user

# Commands to run

## to construct the image
singularity build --fakeroot singularity.sif singularity.def

#To create an image for experimenting with where you can install
# more packages etc, use this:
#   sudo singularity build --sandbox singularity.sif singularity.def
#
#   # to explore as root
#   sudo singularity shell --writable singularity.sif
#
#   # to explore not as root
#   singularity shell --writable singularity.sif


## to run the runscript
singularity run singularity.sif

## to run a shell within the image
singularity shell singularity.sif

By default, environment is included in run commands.
Also the directory stays the same (so e.g. a chapel directory
will appear next to singularity.sif if it is created from
within the image)
