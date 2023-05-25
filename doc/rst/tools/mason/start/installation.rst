:title: Mason Guide: *Installation*

Installation Instructions
=========================

Building Mason
~~~~~~~~~~~~~~

First, make sure your environment is correctly setup, as described in :ref:`readme-chplenv`. In ``$CHPL_HOME`` run the following:

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
