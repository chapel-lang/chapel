.. _compiler-internals-index:

Frontend Library API Docs
=========================

This section documents the various functions and types of the
new compiler frontend.

.. toctree::
   :caption: Conceptual Guide
   :maxdepth: 1

   queries

.. toctree::
   :caption: Implementation Details
   :maxdepth: 1

   queries-impl

.. toctree::
   :caption: Symbols by Namespace
   :maxdepth: 2

   parsing
   resolution
   types
   uast
   base

.. comment

  Tuning/Future --
    * TODO: Add a table of contents just for C++ symbols?
    * Do we want a finer granularity then one section per namespace?
    * How to handle displaying one class per file for "base"? This
      section represents the base 'chpl' namespace.
    * Indicate which members (including inherited members) should
      be documented (to provide the user-facing API and hiding
      inheritance for implementation reasons).
