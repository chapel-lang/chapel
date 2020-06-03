.. _Chapter-Language_Overview:

Language Overview
=================

Chapel is an emerging parallel programming language designed for
productive scalable computing. Chapel’s primary goal is to make parallel
programming far more productive, from multicore desktops and laptops to
commodity clusters and the cloud to high-end supercomputers. Chapel’s
design and development are being led by Hewlett Packard Enterprise (formerly
by Cray Inc.) in collaboration with
academia, computing centers, and industry.

Chapel is being developed in an open-source manner at GitHub under the
Apache v2.0 license and also makes use of other third-party open-source
packages under their own licenses. Chapel emerged from Cray’s entry in
the DARPA-led High Productivity Computing Systems program (HPCS). It is
currently being hardened from that initial prototype to more of a
product-grade implementation.

This section provides a brief overview of the Chapel language by
discussing first the guiding principles behind the design of the
language and second how to get started with Chapel.

.. _Guiding_Principles:

Guiding Principles
------------------

The following four principles guided the design of Chapel:

#. General parallel programming

#. Locality-aware programming

#. Object-oriented programming

#. Generic programming

The first two principles were motivated by a desire to support general,
performance-oriented parallel programming through high-level
abstractions. The second two principles were motivated by a desire to
narrow the gulf between high-performance parallel programming languages
and mainstream programming and scripting languages.

.. _General_Parallel_Programming:

General Parallel Programming
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

First and foremost, Chapel is designed to support general parallel
programming through the use of high-level language abstractions. Chapel
supports a *global-view programming model* that raises the level of
abstraction in expressing both data and control flow as compared to
parallel programming models currently in use. A global-view programming
model is best defined in terms of *global-view data structures* and a
*global view of control*.

*Global-view data structures* are arrays and other data aggregates whose
sizes and indices are expressed globally even though their
implementations may distribute them across the *locales* of a parallel
system. A locale is an abstraction of a unit of uniform memory access on
a target architecture. That is, within a locale all threads exhibit
similar access times to any specific memory address. For example, a
locale in a commodity cluster could be defined to be a single core of a
processor, a multicore processor, or an SMP node of multiple processors.

Such a global view of data contrasts with most parallel languages which
tend to require users to partition distributed data aggregates into
per-processor chunks either manually or using language abstractions. As
a simple example, consider creating a 0-based vector with :math:`n`
elements distributed between :math:`p` locales. A language that supports
global-view data structures, as Chapel does, allows the user to declare
the array to contain :math:`n` elements and to refer to the array using
the indices :math:`0 \ldots n-1`. In contrast, most traditional
approaches require the user to declare the array as :math:`p` chunks of
:math:`n/p` elements each and to specify and manage inter-processor
communication and synchronization explicitly (and the details can be
messy if :math:`p` does not divide :math:`n` evenly). Moreover, the
chunks are typically accessed using local indices on each processor
(*e.g.*, \ :math:`0..n/p`), requiring the user to explicitly translate
between logical indices and those used by the implementation.

A *global view of control* means that a user’s program commences
execution with a single logical thread of control and then introduces
additional parallelism through the use of certain language concepts. All
parallelism in Chapel is implemented via multithreading, though these
threads are created via high-level language concepts and managed by the
compiler and runtime rather than through explicit fork/join-style
programming. An impact of this approach is that Chapel can express
parallelism that is more general than the Single Program, Multiple
Data (SPMD) model that today’s most common parallel programming
approaches use. Chapel’s general support for parallelism does not
preclude users from coding in an SPMD style if they wish.

Supporting general parallel programming also means targeting a broad
range of parallel architectures. Chapel is designed to target a wide
spectrum of HPC hardware including clusters of commodity processors and
SMPs; vector, multithreading, and multicore processors; custom vendor
architectures; distributed-memory, shared-memory, and shared
address-space architectures; and networks of any topology. Our
portability goal is to have any legal Chapel program run correctly on
all of these architectures, and for Chapel programs that express
parallelism in an architecturally-neutral way to perform reasonably on
all of them. Naturally, Chapel programmers can tune their code to more
closely match a particular machine’s characteristics.

.. _Locality_Aware_Programming:

Locality-Aware Programming
~~~~~~~~~~~~~~~~~~~~~~~~~~

A second principle in Chapel is to allow the user to optionally and
incrementally specify where data and computation should be placed on the
physical machine. Such control over program locality is essential to
achieve scalable performance on distributed-memory architectures. Such
control contrasts with shared-memory programming models which present
the user with a simple flat memory model. It also contrasts with
SPMD-based programming models in which such details are explicitly
specified by the programmer on a process-by-process basis via the
multiple cooperating program instances.

.. _Object_Oriented_Programming:

Object-Oriented Programming
~~~~~~~~~~~~~~~~~~~~~~~~~~~

A third principle in Chapel is support for object-oriented programming.
Object-oriented programming has been instrumental in raising
productivity in the mainstream programming community due to its
encapsulation of related data and functions within a single software
component, its support for specialization and reuse, and its use as a
clean mechanism for defining and implementing interfaces. Chapel
supports objects in order to make these benefits available in a parallel
language setting, and to provide a familiar coding paradigm for members
of the mainstream programming community. Chapel supports traditional
reference-based classes as well as value classes that are assigned and
passed by value.

.. _Generic_Programming:

Generic Programming
~~~~~~~~~~~~~~~~~~~

Chapel’s fourth principle is support for generic programming and
polymorphism. These features allow code to be written in a style that is
generic across types, making it applicable to variables of multiple
types, sizes, and precisions. The goal of these features is to support
exploratory programming as in popular interpreted and scripting
languages, and to support code reuse by allowing algorithms to be
expressed without explicitly replicating them for each possible type.
This flexibility at the source level is implemented by having the
compiler create versions of the code for each required type signature
rather than by relying on dynamic typing which would result in
unacceptable runtime overheads for the HPC community.

.. _Getting_Started:

Getting Started
---------------

A Chapel version of the standard “hello, world” computation is as
follows: 

.. code-block:: chapel

   writeln("hello, world");

This complete Chapel program contains a single line of code that makes a
call to the standard ``writeln`` function.

In general, Chapel programs define code using one or more named
*modules*, each of which supports top-level initialization code that is
invoked the first time the module is used. Programs also define a single
entry point via a function named ``main``. To facilitate exploratory
programming, Chapel allows programmers to define modules using files
rather than an explicit module declaration and to omit the program entry
point when the program only has a single user module.

Chapel code is stored in files with the extension ``.chpl``. Assuming
the “hello, world” program is stored in a file called ``hello.chpl``, it
would define a single user module, ``hello``, whose name is taken from
the filename. Since the file defines a module, the top-level code in the
file defines the module’s initialization code. And since the program is
composed of the single ``hello`` module, the ``main`` function is
omitted. Thus, when the program is executed, the single ``hello`` module
will be initialized by executing its top-level code thus invoking the
call to the ``writeln`` function. Modules are described in more detail
in :ref:`Chapter-Modules`.

To compile and run the “hello world” program, execute the following
commands at the system prompt: 

.. code-block:: bash

   > chpl hello.chpl
   > ./hello

The following output will be printed to the console: 

.. code-block:: bash

   hello, world
