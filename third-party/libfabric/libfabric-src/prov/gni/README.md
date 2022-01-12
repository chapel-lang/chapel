# OFI libfabric GNI provider

The GNI provider is a research prototype provider layer for OFI
libfabric running on Cray XC (TM) systems.  It is being developed as a
collaboration between Los Alamos National Laboratory and Cray Inc.
The goals of the collaboration are to enable libfabric clients such as
OpenMPI to experiment at scale on today's hardware.  As such, we have
initially tried to address requirements of MPI, SHMEM and PGAS
language and library implementation.  As with any provider, there
features that are difficult to support efficiently (e.g.,
FI_MR_SCALABLE) and have been omitted for the time being.

Bugs should be filed as GitHub issues on the ofi-cray/libfabric-cray
repository (https://github.com/ofi-cray/libfabric-cray).

## For Developers

See the wiki pages at the ofi-cray/libfabric-cray repository
(https://github.com/ofi-cray/libfabric-cray) for tips and other useful
information.

