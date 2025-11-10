:title: Mason Guide: *Installation*

Installation Instructions
=========================

If you have installed Chapel from a package manager or binary distribution,
mason may already be included. You can check by running ``mason --version`` in
your terminal.

Building and Installing Mason From Source
-----------------------------------------

Prerequisites
~~~~~~~~~~~~~

To build mason from source, ensure you have a working Chapel build from source
and that your environment is set up correctly, as described in
:ref:`readme-chplenv`.

Mason requires a build of Chapel with ``CHPL_COMM=none`` and
``CHPL_RE2=bundled``. If your current Chapel build does not meet these
requirements, you will need to build Chapel with this configuration before
proceeding. You can do this by running the following commands in your Chapel
source directory:

.. code-block:: sh

   CHPL_COMM=none CHPL_RE2=bundled make

Building Mason
~~~~~~~~~~~~~~

In ``$CHPL_HOME`` run the following:

.. code-block:: sh

   make mason

It builds the mason binary as ``$CHPL_HOME/bin/$CHPL_HOST_PLATFORM-$CHPL_HOST_ARCH/mason`` so that the command line interface can be used.
If the binary is already in your path (see :ref:`using-chapel-in-other-sessions`),  mason can be used anywhere in the user’s file system.

To remove mason, change directory to ``$CHPL_HOME/tools/mason`` and run:

.. code-block:: sh

   make clean


Installing Mason
~~~~~~~~~~~~~~~~

Similarly to ``chpl``, mason can be built and installed into a specific directory (e.g ``/usr/local/bin``) as follows

.. code-block:: sh

  ./configure
  make mason
  make install

The folder where the binary is installed can be configured with the ``--prefix`` or ``--chpl-home`` options, as described in :ref:`readme-installing`.
