
=====
mason
=====

Mason is Chapel's package manager

For even more detail about mason and about submitting a package see -> doc/{rst,html}/tools/mason/mason.{rst,html}




Installation Instructions
=========================

In the mason directory found in ``$CHPL_HOME/tools/mason`` run the following:

.. code-block:: sh

   make
   make install

This will create the hidden ``.mason/`` directory in ``MASON_HOME``, which
defaults to your ``$HOME`` if unset. It builds the mason binary so that the
command line interface can be used. The mason registry is also downloaded from
github so that a user can start to rely on mason to specify project
dependencies. This installs mason in the same place as the chapel compiler (``chpl``)
so that mason can be used anywhere in the user's file system.



To remove mason, change directory to ``$CHPL_HOME/tools/mason`` and run:

.. code-block:: sh

   make clean


To remove mason and the mason-registry change directory to ``$CHPL_HOME/tools/mason``
and run:

.. code-block:: sh

   make clobber
