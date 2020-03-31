.. default-domain:: chpl

.. module:: NetCDF
   :synopsis: NetCDF bindings for Chapel

NetCDF
======
**Usage**

.. code-block:: chapel

   use NetCDF;

**Submodules**

.. toctree::
   :maxdepth: 1
   :glob:

   NetCDF/*

NetCDF bindings for Chapel

This module implements the C-API for netCDF version 4.6.1.

Compiling with netCDF
---------------------

In order to compile a Chapel program that uses this module, the netCDF library
must be installed on the system.  The paths to the ``netcdf.h`` header file
and netCDF library must be known to the compiler, either by finding them in
a default search path, or by using the ``-I`` and ``-L`` compiler arguments.

The compilation command should look something like this:

.. code-block:: sh

    chpl -I$PATH_TO_NETCDF_DIR \
         -L$PATH_TO_NETCDF_LIBS -lnetcdf source.chpl

On Cray systems with the ``cray-netcdf`` module loaded, no compiler flags
are necessary to use the HDF5 module.

