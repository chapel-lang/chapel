.. _readme-ugni:

======================
Using Chapel with ugni
======================

Chapel supports a Cray-specific ``ugni`` communication layer. The ugni
communication layer interacts with the system's network interface very closely
through a lightweight interface called uGNI (user Generic Network Interface).
On Cray XC systems the ugni communication layer is the default.


Using the ugni Communications Layer
___________________________________

To use ugni communications:

#. Leave your ``CHPL_COMM`` environment variable unset or set it to
   ``ugni``:

   .. code-block:: sh

      export CHPL_COMM=ugni

   This specifies that you wish to use the Cray-specific communication
   layer.


#. *(Optional)* Load an appropriate ``craype-hugepages`` module.  For example::

     module load craype-hugepages16M

   The ugni communication layer can be used with or without so-called
   *hugepages*.  Performance for remote variable references is much
   better when hugepages are used.  The only downside of using hugepages
   is that the tasking layer may not be able to detect task stack
   overflows by means of guard pages (see below).

   To use hugepages, you must have a ``craype-hugepages`` module loaded
   both when building your program and when running it.  There are
   several hugepage modules, with suffixes indicating the page size they
   support.  For example, ``craype-hugepages16M`` supports 16 MiB
   hugepages.  It does not matter which ``craype-hugepages`` module you
   have loaded when you build your program.  Any of them will do.  Which
   one you have loaded when you run a program does matter, however.  For
   general use, the Chapel group recommends the ``craype-hugepages16M``
   module.  You can read on for more information about hugepage modules
   if you would like, but the recommended ``craype-hugepages16M`` module
   will probably give you satisfactory results.

   The Cray network interface chips (NICs) can only address memory that
   has been registered with them. In practical terms, the Aries(TM) NIC
   on Cray XC systems is not limited as to how much memory it can
   register.  However, it does have an on-board cache of 512 registered
   page table entries, and registering more than this can cause reduced
   performance if the program's memory reference pattern causes refills
   in this cache.  We have seen up to a 15% reduction from typical
   nightly XC-16 performance in an ra-rmo run using hugepages small
   enough that every reference should have missed in this cache.
   Covering an entire 128 GiB XC compute node with only 512 hugepages
   will require at least the ``craype-hugepages256M`` module's 256 MiB
   hugepages.

   Offsetting this, using larger hugepages may reduce performance because
   it can result in poorer NUMA affinity.  With the ugni communication
   layer, arrays larger than 2 hugepages are allocated separately from the
   heap, which improves NUMA affinity.  An obvious side effect of using
   larger hugepages is that an array has to be larger to qualify.  Thus,
   achieving the best performance for any given program may require
   striking a balance between using larger hugepages to reduce NIC page
   table cache refills and using smaller ones to improve NUMA locality.

   Note that when hugepages are used with the ugni comm layer, tasking
   layers cannot use guard pages for stack overflow detection.  Qthreads
   tasking cannot detect stack overflow except by means of guard pages,
   so if ugni communications is combined with qthreads tasking and a
   hugepage module is loaded, stack overflow detection is unavailable.


Network Atomics
_______________

The Aries networks on Cray XC series systems support remote atomic
memory operations (AMOs).  When the ``CHPL_NETWORK_ATOMICS`` environment
variable is set to ``ugni``, the following operations on remote atomics
are done using the network::

    32- and 64-bit signed and unsigned integer types:
    32- and 64-bit real types:
      read()
      write()
      exchange()
      compareAndSwap()
      add(), fetchAdd()
      sub(), fetchSub()

    32- and 64-bit signed and unsigned integer types:
      or(),  fetchOr()
      and(), fetchAnd()
      xor(), fetchXor()

All of the operations shown above are done natively by the network
hardware except 64-bit real add, which is disabled in hardware and thus
done using ``on`` statements.


ugni Communication Layer and the Heap
_____________________________________

The "heap" is an area of memory used for dynamic allocation of
everything from user data to internal management data structures.
When running on Cray XC systems using the default configuration
with the ugni comm layer and a ``craype-hugepages`` module loaded, the
heap is used for all dynamic allocations except data space for arrays
larger than 2 hugepages.  (See `Using the ugni Communications Layer`_,
just above, for more about hugepages.)  It is normally extended
dynamically, as needed.  But if desired, the heap can instead be created
at a specified fixed size at the beginning of execution.  In some cases
this will reduce certain internal comm layer overheads and marginally
improve performance.

The disadvantage of a fixed heap is that it usually produces worse NUMA
affinity, it limits available heap memory to the specified fixed size,
and it limits memory for arrays to whatever remains after the fixed-size
heap is created.  If either of the latter are less than what a program
needs, it will terminate prematurely with an "Out of memory" message.

To specify a fixed heap, set the ``CHPL_RT_MAX_HEAP_SIZE`` environment
variable to indicate its size.  For the value of this variable you can
use any of the following formats, where *num* is a positive integer
number:

    ======= ==========================================
    Format  Resulting Heap Size
    ======= ==========================================
    num     num bytes
    num[kK] num * 2**10 bytes
    num[mM] num * 2**20 bytes
    num[gG] num * 2**30 bytes
    num%    percentage of compute node physical memory
    ======= ==========================================

Any of the following would specify an approximately 1 GiB heap on a
128-GiB compute node, for example:

  .. code-block:: sh

    export CHPL_RT_MAX_HEAP_SIZE=1073741824
    export CHPL_RT_MAX_HEAP_SIZE=1048576k
    export CHPL_RT_MAX_HEAP_SIZE=1024m
    export CHPL_RT_MAX_HEAP_SIZE=1g
    export CHPL_RT_MAX_HEAP_SIZE=1% # 1.28 GiB, really

Note that the resulting heap size may get rounded up to match the page
alignment.  How much this will add, if any, depends on the hugepage size
in any ``craype-hugepage`` module you have loaded at the time you
execute the program.  It may also be reduced, if some resource
limitation prevents making the heap as large as requested.


ugni Communication Layer Registered Memory Regions
__________________________________________________

The ugni communication layer maintains information about every memory
region it registers with Aries NIC.  Roughly speaking there are a few
memory regions for each tasking layer thread, plus one for each array
larger than 2 hugepages allocated and registered separately from the
heap.  By default the comm layer can handle up to 16k (2**14) total
memory regions, which is plenty under normal circumstances.  In the
event a program needs more than this, a message like the following will
be printed:

  .. code-block:: sh

    warning: no more registered memory region table entries (max is 16384).
             Change using CHPL_RT_COMM_UGNI_MAX_MEM_REGIONS.

To provide for more registered regions, set the
``CHPL_RT_COMM_UGNI_MAX_MEM_REGIONS`` environment variable to a number
indicating how many you want to allow.  For example:

  .. code-block:: sh

    export CHPL_RT_COMM_UGNI_MAX_MEM_REGIONS=30000

Note that there are certain comm layer overheads that are proportional to
the number of registered memory regions, so allowing a very high number of
them may lead to reduced performance.


ugni Hugepage-related Warnings
______________________________

   Communication performance with ugni is so much better when hugepages
   are used that if you do not use them, the runtime will print the
   following warning when a multilocale program starts::

      warning: without hugepages, communication performance will suffer

   If you definitely do not want to use hugepages you can quiet this
   warning by giving the ``--quiet`` or ``-q`` option when you run the
   executable.  Otherwise, load a hugepage module as described above in
   `Using the ugni Communications Layer`_ before running.

   When you are using hugepages and do not have a fixed heap (that is,
   the ``CHPL_RT_MAX_HEAP_SIZE`` environment variable is not set), the
   Chapel runtime expects certain hugepage-related environment variables
   to have been set by the Chapel launcher.  If you do not use a Chapel
   launcher you have to provide these settings yourself.  Not doing so
   will result in one or both of the following messages::

      warning: dynamic heap on hugepages needs HUGETLB_NO_RESERVE set to something
      warning: dynamic heap on hugepages needs CHPL_JE_MALLOC_CONF set properly

   To quiet these warnings, use the following settings:

    .. code-block:: sh

      export HUGETLB_NO_RESERVE=yes
      export CHPL_JE_MALLOC_CONF=purge:decay,lg_chunk:log2HPS

   where *log2HPS* is the base-2 log of the hugepage size.  For example,
   with 16 MiB hugepages you would use:

    .. code-block:: sh

      export CHPL_JE_MALLOC_CONF=purge:decay,lg_chunk:24
