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

   This users guide is a work-in-progress.  We are writing it in a
   breadth-first manner in order to cover features that are more
   unique to Chapel sooner.  If you're not already using the `master
   branch <https://chapel-lang.org/docs/master/users-guide/>`_ of the
   documentation (which tracks real-time GitHub development), you may
   find that it contains a more complete set of sections.  We welcome
   feedback and requests on the guide as we go.


The Chapel Users Guide is divided into four main sections:


Base Language
-------------

This is the core of Chapel and what remains when all features in
support of parallelism and locality are removed.

.. toctree::
   :maxdepth: 1
   :glob:

   base/hello
   base/vars
   base/basicTypes
   base/basicValues
   base/constParam
   base/typeAliases
   base/configs
   base/casts
   base/forloops
   base/zip

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
   taskpar/begin
   taskpar/cobegin
   taskpar/coforall

(more to come...)


.. _index-data-parallelism:

Data Parallelism
----------------

These are Chapel's higher-level features for creating parallelism more
abstractly using a rich set of data structures.

.. toctree::
   :maxdepth: 1
   :glob:

   datapar/forall
   datapar/promotion

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

