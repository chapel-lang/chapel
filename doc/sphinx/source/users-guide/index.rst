.. _users-guide-index:

Chapel Users Guide (work-in-progress)
=====================================

This Users Guide is being written with the goal of providing an
online, intuitive, and example-oriented introduction to the Chapel
programming language without worrying about being overly pedantic or
defining every corner case of the language.  For a more precise
definition of the language, see the `Language Specification
<http://chapel.cray.com/spec/spec-0.98.pdf>`_.

**Note:** This guide is a work-in-progress that was kicked off in
Autumn 2015.  We are taking the approach of writing it in a
breadth-first manner in order to cover advanced features that are
unique to the language without completely neglecting the base language
(expected to comprise the largest number of the sections of the
guide).  We welcome feedback and requests on the guide as we go.


Base Language
-------------

Chapel's base language features are those that remain when all
concepts related to parallelism and locality are removed.


.. toctree::
   :maxdepth: 1
   :glob:

   base/simpleConsoleOutput
   base/variableDeclarations
   base/basicTypes
   base/basicValues

Task Parallelism
----------------


Chapel's task parallel features support the explicit creation of
parallel tasks and synchronization between them.

.. toctree::
   :maxdepth: 1
   :glob:

   taskpar/taskParallelismOverview
   taskpar/theBeginStatement


Data Parallelism
----------------

Chapel's data-parallel features are higher-level concepts for creating
parallelism using a rich set of data structures.


Locality
--------

Chapel's locality-oriented features describe how data and tasks should
be mapped to the target architecture for the purposes of performance
and scalability.

.. toctree::
   :maxdepth: 1
   :glob:

   locality/localesInChapel
   locality/compilingAndExecutingMultiLocalePrograms
   locality/localeTypeAndVariables
