.. _readme-localeModels:

=============
Locale Models
=============

------------
Introduction
------------

Chapel's original computer system architecture model was a collection of
simple locales connected by a communication network.  The locales had
one or more homogeneous processor cores and one kind of memory, with all
the memory equidistant from all the processor cores.  But while this
model was conceptually easy to deal with, it couldn't support users who
wanted to take advantage of modern node architectures.  To support
these, we are extending architectural descriptions.  In the new model
the top level may still be a network of locales, but the locales are
more complicated.  They may be internally heterogeneous, containing
multiple instances of memories and/or processors with differing
characteristics.  They may also be hierarchical, with parent locales
containing one or more child sublocales within them.

There are currently two locale models available: flat and NUMA.  The
flat model is the default and maps closely to the view of locales
implemented in the 1.7 release.  The NUMA locale model maps sublocales
to NUMA domains.  The NUMA model is currently implemented at a prototype
level. Performance has not yet been a focus in the NUMA locale model and
will require additional effort in future releases.  We expect to add more
locale models in future releases.


-----------------------------------
Architecture support in the modules
-----------------------------------

The code emitted by the compiler contains calls to support routines that
manage memory, communication, and tasking, among other things.  Before
hierarchical locale support was added, these calls were all satisfied
directly by the runtime.  With hierarchical locales, now they are
satisfied by the Chapel module code that defines the architecture of a
locale.  The required interface for this is defined by ``ChapelLocale`` and
implemented by ``LocaleModel.chpl``.  The required interface is still a work
in progress and will continue to evolve.


-----------------
Flat Locale Model
-----------------

The current default locale model is the flat locale model. In the flat model,
locales have homogeneous processor cores and all cores are equidistant from
memory.


.. _readme-NUMAlm:

-----------------
NUMA Locale Model
-----------------

In the NUMA locale model, the processor is split into NUMA domains
and cores within a domain have faster access to local memory.

The NUMA locale model is supported most fully when qthreads tasking is
used.  While other tasking layers are also functionally correct using
the NUMA locale model, they are not NUMA aware.  In addition, the
Portable Hardware Locality library (hwloc) is used with qthreads to map
sublocales to NUMA domains. For more information about qthreads and
about tuning parameters such as the number of qthread shepherds per
locale, please see :ref:`readme-tasks`.

To use the NUMA locale model:

1) Set the ``CHPL_LOCALE_MODEL`` environment variable to ``numa``.

.. code-block:: sh

    export CHPL_LOCALE_MODEL=numa

2) Re-make the compiler and runtime from ``CHPL_HOME``

.. code-block:: sh

    cd $CHPL_HOME
    make

3) Compile your Chapel program as usual.

.. code-block:: sh

    chpl $CHPL_HOME/examples/hello6-taskpar-dist.chpl


^^^^^^^^^^^^^^^^^^^^^^^^^^
Performance Considerations
^^^^^^^^^^^^^^^^^^^^^^^^^^

Performance when using the NUMA locale model is currently no better than
when using the flat locale model, and often worse. At present most of
our effort has to do with making better use of first-touch to achieve
NUMA affinity.

--------------------------
Qthreads thread scheduling
--------------------------

When qthreads tasking is used, different Qthreads thread schedulers are
selected depending upon the ``CHPL_LOCALE_MODEL`` setting.  For the flat
locale model the "nemesis" thread scheduler is used, and for the NUMA
locale model the "distrib" thread scheduler is used.  This selection is
done at the time the Qthreads third-party package is built, and cannot
be adjusted later, either at user compile time or at execution time.


---------------------------------------
Caveats for using the NUMA locale model
---------------------------------------

* Explicit memory allocation for NUMA domains is not yet implemented.

* Distributed arrays other than Block do not yet map iterations to NUMA
  domains.

* Performance for NUMA has not been optimized.

