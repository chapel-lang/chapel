:title: Mason Guide: *Chapel Dependencies*

Using Chapel Dependencies
=========================

There are multiple types of dependencies in Mason. Chapel or "Mason" dependencies are other
Mason packages that you want to use in your Mason package.

To search through all the current available Mason packages, use ``mason search``.

Chapel Dependencies are listed under the ``[dependencies]`` table in the ``Mason.toml``
file of the package as follows:

.. code-block:: text


   [brick]
   chplVersion = "1.18.0"
   license = "None"
   name = "myPackage"
   version = "0.1.0"

   [dependencies]
   MatrixMarket = 0.1.0

To add a Chapel dependency without editing the ``Mason.toml`` manually, use the ``mason add``
command as follows::

  mason add MatrixMarket@0.1.0

For example usage of a Mason package, see :ref:`using-a-package`.
