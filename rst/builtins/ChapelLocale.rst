.. default-domain:: chpl

.. module:: ChapelLocale
   :synopsis: A *locale* is a Chapel abstraction for a piece of a target

Locales
=======
A *locale* is a Chapel abstraction for a piece of a target
architecture that has processing and storage capabilities.
Generally speaking, the tasks running within a locale have
roughly uniform access to values stored in the locale's local
memory and longer latencies for accessing the memories of other
locales.  As examples, a single shared memory machine would be
defined as a single locale, while in a system consisting of a
group of network-connected multicore nodes or SMPs each node
would be defined as a locale.

Chapel provides several predefined methods on locales, as well as
a few variables that describe the locales upon which a program is
running.

In addition to what is documented below, ``numLocales``, ``LocaleSpace``,
and ``Locales`` are available as global variables.

``numLocales`` is the number of top-level (network connected) locales.

.. code-block:: chapel

  config const numLocales: int;

``LocaleSpace`` is the domain over which the global ``Locales`` array is
defined.

.. code-block:: chapel

  const LocaleSpace = {0..numLocales-1};

The global ``Locales`` array contains an entry for each top-level locale.

.. code-block:: chapel

  const Locales: [LocaleSpace] locale;


One common code idiom in Chapel is the following, which spreads parallel
tasks across the network-connected locales upon which the program is running:

  .. code-block:: chapel

    coforall loc in Locales { on loc { ... } }




.. class:: locale

   
   ``locale`` is the abstract class from which the various
   implementations inherit.  It specifies the required interface
   and implements part of it, but requires the rest to be provided
   by the corresponding concrete classes.
   


   .. method:: proc numPUs(logical: bool = false, accessible: bool = true)

      
      A *processing unit* or *PU* is an instance of the processor
      architecture, basically the thing that executes instructions.
      :proc:`numPUs` tells how many of these are present on this
      locale.  It can count either physical PUs (commonly known as
      *cores*) or hardware threads such as hyperthreads and the like.
      It can also either take into account any OS limits on which PUs
      the program has access to or do its best to ignore such limits.
      By default it returns the number of accessible physical cores.
      
      :arg logical: Count logical PUs (hyperthreads and the like),
                    or physical ones (cores)?  Defaults to `false`,
                    for cores.
      :type logical: `bool`
      :arg accessible: Count only PUs that can be reached, or all of
                       them?  Defaults to `true`, for accessible PUs.
      :type accessible: `bool`
      :returns: number of PUs
      :rtype: `int`
      
      There are several things that can cause the OS to limit the
      processor resources available to a Chapel program.  On plain
      Linux systems using the ``taskset(1)`` command will do it.  On
      Cray systems the ``CHPL_LAUNCHER_CORES_PER_LOCALE`` environment
      variable may do it, indirectly via the system job launcher.
      Also on Cray systems, using a system job launcher (``aprun`` or
      ``slurm``) to run a Chapel program manually may do it, as can
      running programs within Cray batch jobs that have been set up
      with limited processor resources.
      

   .. attribute:: var maxTaskPar: int

      
      This is the maximum task concurrency that one can expect to
      achieve on this locale.  The value is an estimate by the
      runtime tasking layer.  Typically it is the number of physical
      processor cores available to the program.  Creating more tasks
      than this will probably increase walltime rather than decrease
      it.
      

   .. attribute:: var callStackSize: size_t

      
      ``callStackSize`` holds the size of a task stack on a given
      locale.  Thus, ``here.callStackSize`` is the size of the call
      stack for any task on the current locale, including the
      caller.
      

   .. method:: proc id: int

      
      Get the integer identifier for this locale.
      
      :returns: locale number, in the range ``0..numLocales-1``
      :rtype: int
      

   .. method:: proc hostname: string

      
      Get the hostname of this locale.
      
      :returns: the hostname of the compute node associated with the locale
      :rtype: string
      

   .. method:: proc name

      
      Get the name of this locale.  In practice, this is often the
      same as the hostname, though in some cases (like when using
      local launchers), it may be modified.
      
      :returns: locale name
      :rtype: string
      

.. function:: proc here

   
   This returns the locale from which the call is made.
   
   :return: current locale
   :rtype: locale
   

.. record:: chpl_privateObject_t

   .. attribute:: var obj: c_void_ptr

.. data:: var chpl_privateObjects: c_ptr(chpl_privateObject_t)

