
=====
mason
=====

Mason is Chapel's package manager

For more detail about mason and about submitting a package see -> doc/{rst,html}/tools/mason/mason.{rst,html}




Installation Instructions
=========================

In the mason directory found in ``$CHPL_HOME/tools/mason`` run the following:

.. code-block:: sh

   make
   make install

It builds the mason binary so that the command line interface can be used. This
installs mason in the same place as the chapel compiler (``chpl``) so that
mason can be used anywhere in the user's file system.


The ``mason update`` command will download the registry to ``$MASON_HOME/registry``
if it there is not already a registry at that location.


To remove mason, change directory to ``$CHPL_HOME/tools/mason`` and run:

.. code-block:: sh

   make clean

