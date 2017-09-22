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

There are currently three locale models available, flat, NUMA, and KNL.  The
flat model is the default and maps closely to the view of locales
implemented in the 1.7 release.  The NUMA locale model maps sublocales
to NUMA domains.  The NUMA model is currently implemented at a prototype
level. Performance has not yet been a focus in the NUMA locale model and
will require additional effort in future releases.  The KNL locale
model provides support for self-hosting Xeon Phi (formerly Knight's
Landing) processors and includes provision for access to
tightly-coupled on-package high-bandwidth DRAM memory.  We expect to
add more locale models in future releases.


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

    chpl -o jacobi $CHPL_HOME/examples/programs/jacobi.chpl


^^^^^^^^^^^^^^^^^^^^^^^^^^
Performance Considerations
^^^^^^^^^^^^^^^^^^^^^^^^^^

Performance when using the NUMA locale model is currently no better than
when using the flat locale model, and often worse.  The *multi-ddata*
feature introduced in the 1.15 Chapel release improved some cases for
the NUMA locale model but slowed many others, sometimes by a lot.  In
the end we disabled it because much of the performance loss was inherent
in the implementation and could not be removed.  On Cray XE and XC
systems with ``CHPL_COMM=ugni`` and NIC-registered memory, recent work
to allocate arrays separately and register them dynamically has improved
NUMA affinity and thus performance, but the benefits of that effort
apply to the flat locale model as well as they do to the numa one.  For
other configurations, with the multi-ddata feature disabled performance
with the NUMA locale model has returned, for better or worse, to what it
was before that was introduced.  At present most of our effort has to do
with making better use of first-touch to achieve NUMA affinity.


.. _readme-KNLlm:

----------------
KNL Locale Model
----------------

The KNL locale model has the same properties as the NUMA locale model,
plus it allows access to the Xeon Phi processor's on-package
high-bandwidth memory.

The KNL locale model requires the Intel Memkind library, which can be
obtained in source form, and is also available in the binary
repositories of some Linux distributions.

For more information on the Memkind library, please see:

    https://memkind.github.io

On a Cray system, Memkind can be loaded with the following command.
Note that this makes dynamic linking the default, because Memkind is
dynamically linked.

.. code-block:: sh

    module load cray-memkind

Once the Memkind library is available, Chapel can be built using the
instructions under :ref:`readme-NUMAlm`, except that
``CHPL_LOCALE_MODEL`` must be set to ``knl``.

On a Cray system, the KNL locale model is included in the Chapel
module, so the following commands are sufficient.

.. code-block:: sh

    module load cray-memkind
    module load chapel
    export CHPL_LOCALE_MODEL=knl

Please see :ref:`readme-cray` for more detailed information.

New locale model member functions are provided for controlling which
kind of memory is used for new allocations.  To allocate in high
bandwidth memory, use the ``.highBandwidthMemory()`` member function.
For example:

.. code-block:: chapel

    on here.highBandwidthMemory() {
      x = new MyObject();
    }

It is also possible to say "Use the same locale as variable ``y``, but
use high bandwidth memory" as follows.

.. code-block:: chapel

    on y.locale.highBandwidthMemory() {
      // . . .
    }

In case one is nested inside ``on`` statements and desires to get back
to the default externally-attached memory, a ``.defaultMemory()``
member function is available.

.. code-block:: chapel

    on x {
      // . . .
      on here.defaultMemory() {
        // . . .
      }
    }

In addition, ``.lowLatencyMemory()`` and ``.largeMemory()`` functions
are provided for explicitly referencing the externally-attached
memory.  In the KNL locale model, ``.defaultMemory()``,
``.lowLatencyMemory()``, and ``.largeMemory()`` are all the same.

If the KNL processor is booted in the ``cache`` configuration, where
high-bandwidth memory is not exposed to the user, then the program
will still run and ``.highBandwidthMemory()`` will use the default
externally-attached memory.

The four memory selection functions have also been added to the flat
and NUMA locale models, so it is possible to write programs that take
advantage of the KNL processor when it is present, and yet still run
on other processors.

Please see :ref:`readme-knl` for additional information.


--------------------------
Qthreads thread scheduling
--------------------------

When qthreads tasking is used, different Qthreads thread schedulers are
selected depending upon the ``CHPL_LOCALE_MODEL`` setting.  For the flat
locale model the "nemesis" thread scheduler is used, and for the NUMA and KNL
locale models the "distrib" thread scheduler is used.  This selection is
done at the time the Qthreads third-party package is built, and cannot
be adjusted later, either at user compile time or at execution time.


---------------------------------------
Caveats for using the NUMA locale model
---------------------------------------

* Explicit memory allocation for NUMA domains is not yet implemented.

* Distributed arrays other than Block do not yet map iterations to NUMA
  domains.

* Performance for NUMA has not been optimized.

