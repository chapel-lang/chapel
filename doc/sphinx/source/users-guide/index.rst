.. _users-guide-index:

Chapel Users Guide (WIP)
========================

Overview
--------

This Users Guide is written with the goal of providing an intuitive
and example-oriented introduction to the Chapel programming language
without feeling the need to be overly pedantic or to worry about
defining every corner case in the language.  

.. note::

   **Pardon Our Dust**

   This users guide is a work-in-progress that was kicked off in
   Autumn 2015.  We are taking the approach of writing it in a
   breadth-first manner in order to cover features that are most
   unique to the language sooner without completely neglecting the
   base language (which is expected to comprise the largest number of
   the sections of the guide).  We welcome feedback and requests on
   the guide as we go.


The Chapel Users Guide is divided into four main sections:


Base Language
-------------

This is the core of Chapel and what remains when all features in
support of parallelism and locality are removed.

.. toctree::
   :maxdepth: 1
   :glob:

   base/simpleConsoleOutput
   base/variableDeclarations

(more to come...)

.. _index-task-parallelism:

Task Parallelism
----------------

These are Chapel's lower-level features for creating parallel
explicitly and synchronizing between them.

.. toctree::
   :maxdepth: 1
   :glob:

   taskpar/taskParallelismOverview
   taskpar/theBeginStatement

(more to come...)

.. _index-data-parallelism:

Data Parallelism
----------------

These are Chapel's higher-level features for creating parallelism more
abstractly using a rich set of data structures.

(more to come...)


Locality
--------

These are Chapel's features for describing how data and tasks should
be mapped to the target architecture for the purposes of performance
and scalability.

.. toctree::
   :maxdepth: 1
   :glob:

   locality/localesInChapel
   locality/compilingAndExecutingMultiLocalePrograms
   locality/localeTypeAndVariables
   locality/onClauses

(more to come...)

