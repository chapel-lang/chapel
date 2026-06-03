.. _mason-external-dependencies:

Using Non-Chapel Dependencies
=============================
Mason allows for specification of external, non-Chapel dependencies as
system dependencies. ``mason system`` provides an interface to ``pkg-config``
to find externally installed dependencies on a system.

.. note::

   Mason used to provide an interface to Spack directly, called
   ``mason external``. This functionality has been removed. If you need to rely
   on external dependencies, you can use the ``mason system`` interface to find
   packages installed by Spack.

System dependencies are packages that are found on your system through
``pkg-config``. To use this functionality of Mason, users must have
``pkg-config`` installed.

Mason interfaces with ``pkg-config`` through the ``mason system`` command.

``mason system search`` will print all the current packages installed and
available for use in a Mason package. To examine the ``.pc`` file of a
particular package, use ``mason system pc <package>`` where ``<package>`` is
replaced with the particular package you are looking for. Here is an example of
a workflow for creating a Mason package with ``zlib`` which has already been
installed.

First, search to see that it is installed with ``mason system search zlib``
which outputs:

.. code-block:: sh

   $ mason system search zlib
   zlib                           zlib - zlib compression library

To find out more about the package, since it is in fact installed on my system,
use the ``mason system pc`` command as follows

.. code-block:: sh

   $ mason system pc zlib

   ------- zlib.pc -------

   prefix=/usr
   libdir=${prefix}/lib
   includedir=${prefix}/include

   Name: zlib
   Description: zlib compression library
   Version: 1.2.12

   Requires:
   Libs: -L${libdir} -lz
   Cflags:

   -------------------


Use the ``mason add --system`` command to add the dependency to the Mason.toml
of the package.

.. code-block:: sh

   $ mason add --system zlib@1.2.12
   Adding system dependency zlib version 1.2.12

The ``Mason.toml`` now looks like:

.. code-block:: toml

   [brick]
   chplVersion = "1.18.0"
   license = "None"
   name = "myPackage"
   version = "0.1.0"

   [system]
   zlib = "1.2.12"

Now, upon calling ``mason build``, Mason will use ``pkg-config`` to find the
necessary flags to link against zlib and include the appropriate headers.

For some dependencies, it may be beneficial to not pin a specific version.
In this case, you can add the dependency without a version number as follows:

.. code-block:: sh

   $ mason add --system zlib
   Adding system dependency zlib with version *

The ``Mason.toml`` now looks like:

.. code-block:: toml

   [brick]
   chplVersion = "1.18.0"
   license = "None"
   name = "myPackage"
   version = "0.1.0"

   [system]
   zlib = "*"
