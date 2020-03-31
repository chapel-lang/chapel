.. default-domain:: chpl

.. module:: HDF5
   :synopsis: HDF5 bindings for Chapel

HDF5
====
**Usage**

.. code-block:: chapel

   use HDF5;

**Submodules**

.. toctree::
   :maxdepth: 1
   :glob:

   HDF5/*

HDF5 bindings for Chapel

This module implements the C-API for HDF5 version 1.10.1, as well as some
functionality for reading and writing HDF5 files built on top of the C-API.

Compiling with HDF5
-------------------

In order to compile a Chapel program that uses this module, the HDF5 library
must be installed on the system.  The paths to the ``hdf5_hl.h`` header
file and HDF5 library must be known to the compiler, either by finding
them in a default search path, or by using the ``-I`` and ``-L`` compiler
arguments.

The compilation command should look something like this:

.. code-block:: sh

  chpl -I$PATH_TO_HDF5_DIR \
       -L$PATH_TO_HDF5_LIBS -lhdf5 source.chpl

On Cray systems with the ``cray-hdf5`` module loaded, no compiler flags
are necessary to use the HDF5 module.

This module currently requires using a version of the Chapel compiler that
is linked with LLVM.  See :ref:`readme-llvm`.

Chapel HDF5 API
---------------
This module provides some higher-level functions using the HDF5 API. It
provides for parallel reads and writes to HDF5 files using multiple
Chapel locales.

The native HDF5 functions can be called directly by calling into the
:mod:`C_HDF5` submodule.

.. function:: proc readAllHDF5Files(locs: [] locale, dirName: string, dsetName: string, filenameStart: string, type eltType, param rank, preprocessor: borrowed nilable HDF5Preprocessor = )

   Read the dataset named `dsetName` from all HDF5 files in the
   directory `dirName` with filenames that begin with `filenameStart`.
   This will read the files in parallel with one task per locale in the
   `locs` array.  Specifying the same locale multiple times in the `locs`
   array will cause undefined behavior.
   
   Returns a distributed array of :record:`ArrayWrapper` records
   containing the arrays that are read. Each instance will reside on
   the locale where the corresponding data was read.
   

.. function:: proc readAllNamedHDF5Files(locs: [] locale, filenames: [] string, dsetName: string, type eltType, param rank, preprocessor: borrowed nilable HDF5Preprocessor = )

   Read all HDF5 files named in the filenames array into arrays 

.. function:: proc readNamedHDF5FilesInto1DArrayInt(filenames: [] string, fnCols: int, fnRows: int, dsetName: string, preprocessor: borrowed nilable HDF5Preprocessor = )

   Read the datasets named `dsetName` from the files named in
   `filenames` into a 1D array.  This function assumes the data
   elements are all int(64)s.
   `fnCols` and `fnRows` refer to the columns and rows that `filenames`
   should have when converting it back to 2-D.
   

.. function:: proc readHDF5Dataset(file_id, dsetName: string, data)

   Read the dataset named `dsetName` from the open file that `file_id`
   refers to.  Store the dataset into the array `data`.
   Can read data of type int/uint (size 8, 16, 32, 64), real (size 32, 64),
   and c_string.
   

.. function:: proc getHDF5Type(type eltType)

   Return the HDF5 type equivalent to the Chapel type `eltType` 

.. enum:: enum Hdf5OpenMode { Truncate, Append }

   Enum indicating the way to open a file for writing.  `Truncate` will
   clear the contents of an existing file.  `Append` will add to a file
   that already exists.  Both will open a new empty file.
   


.. function:: proc writeArraysToHDF5Files(dirName: string, dsetNames: [] string, filenames: [] string, type eltType, param rank: int, data: [] eltTypeArrayWrapperrank, mode: Hdf5OpenMode) throws

   Write the arrays from the :record:`ArrayWrapper` records stored in
   the `data` argument to the corresponding filename in the
   `filenames` array. The dataset name is taken from the corresponding
   position in the `dsetNames` array.
   
   The `data` argument should be a Block distributed array with
   `dataParTasksPerLocale==1`.
   
   Either truncate or append to the file depending on the `mode` argument.
   
   It would be preferable to find the `eltType` and `rank` values
   directly from the `data` argument instead of needing explicit
   arguments for them, but it isn't obvious how to do that currently.
   If the generic fields in the `ArrayWrapper` could be queried that
   would be a nice replacement for these arguments.  e.g.
   `data: [] ArrayWrapper(?eltType, ?rank)`.
   

.. iterfunction:: iter hdf5ReadChunks(filename: string, dset: string, chunkShape: domain, type eltType, preprocessor: borrowed nilable HDF5Preprocessor = )

   Read the dataset named `dset` from the HDF5 file named `filename`.
   The dataset consists of elements of type `eltType`.  The file will
   be read in chunks matching the `chunkShape` domain, and yielded as
   arrays of that size until the end of the dataset is reached.
   
   This allows operating on a very large dataset in smaller sections,
   for example when it is too big to fit into the system memory, or
   to allow each section to fit within cache.
   
   Currently, the `chunkShape` domain describing the yielded array shape
   and the shape of the data in the file must both have the same rank.
   For example, if the data in the file is 2D, `chunkShape` must be
   two-dimensional as well.  It is expected that this restriction can
   be relaxed in the future.
   

.. class:: HDF5Preprocessor

   A class to preprocess arrays returned by HDF5 file reading procedures.
   Procedures in this module that take an `HDF5Preprocessor` argument can
   accept a subclass of this class with the `preprocess` method overridden
   to do preprocessing as desired before returning the data read.
   


   .. method:: proc preprocess(A: [])

.. record:: ArrayWrapper

   A record that stores a rectangular array.  An array of `ArrayWrapper`
   records can store multiple differently sized arrays.
   


   .. attribute:: type eltType

   .. attribute:: param rank: int

   .. attribute:: var D: domain(rank)

   .. attribute:: var A: [D] eltType

