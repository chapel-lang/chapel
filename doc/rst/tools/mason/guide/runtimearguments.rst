:title: Mason Guide: *Runtime Arguments*

Runtime/Compilation Arguments
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Runtime and compilation options can be passed to a mason-built Chapel program
or the Chapel compiler via ``mason run`` and ``mason build``, respectively.
Note that any options not intended for ``mason`` itself must be separated from
the ``mason`` command by a ``--``. This indicates to ``mason`` that the
following arguments should be forwarded to the Chapel program or Chapel
compiler.

For an example of forwarding arguments in a call to ``mason run``, a Chapel
program built in mason might have a ``config const number`` that corresponds to
a value used in ``MyPackage.chpl``. To try out different values at runtime,
pass the values for ``number`` to ``mason run`` as follows:

.. code-block:: bash

   mason run -- --number=100
   mason run -- --number=1000


Similarly, compilation options can be passed to the Chapel compiler via ``mason
build``. For example, to disable the remote cache when building a package, use:

.. code-block:: bash

   mason build -- --no-remote-cache

Compilation options can also be specified for the entire package in the
``Mason.toml`` manifest file. See :ref:`mason-manifest-file` for more
information.
