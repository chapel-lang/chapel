

.. image:: mason.png
    :align: center

Mason is Chapel's package manager

For more detail about Mason see https://chapel-lang.org/docs/master/tools/mason/mason.html


Installation Instructions
=========================

In the mason directory found in ``$CHPL_HOME/tools/mason`` run the following:

.. code-block:: sh

   make
   make install

These commands build the mason binary so that the command line interface can be used.
Mason will be installed in the same place as the chapel compiler (``chpl``) so that
Mason can be used anywhere in the user's file system.

The ``mason update`` command will download the registry to ``$MASON_HOME/registry``
if it there is not already a registry at that location.


To remove mason, change directory to ``$CHPL_HOME/tools/mason`` and run:

.. code-block:: sh

   make clean


Getting Started
===============

To start a new Mason project, use the following command:

.. code-block:: sh

   mason new myProject

This will create a new Mason project for you. From there, building and running
your project is as easy as a single command:

.. code-block:: sh

   mason run --build

For help on any command, type the following:

.. code-block:: sh

   mason <command> --help

                
For further information regarding Mason and it's capabilities, please see the documentation.

