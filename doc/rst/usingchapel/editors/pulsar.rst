.. _pulsar:

Pulsar
------

.. _pulsar-syntax:

Syntax Highlighting
~~~~~~~~~~~~~~~~~~~

Syntax highlighting is provided by the ``pulsar-ide-chapel`` community package.
To install the package, visit the
`Pulsar package library <https://packages.pulsar-edit.dev/packages/pulsar-ide-chapel>`_.

.. _pulsar-lsp:

Language Server Support
~~~~~~~~~~~~~~~~~~~~~~~

The ``pulsar-ide-chapel`` package provides built-in support for the :ref:`Chapel
language server <readme-chpl-language-server>` with ``--chplcheck`` enabled for
diagnostics. If ``chpl-language-server`` is in your ``$PATH``, the package will
automatically use it. If not, you can either specify your ``CHPL_HOME`` in the
package settings or specify the path to the executable.

Specifying ``CHPL_HOME``:
^^^^^^^^^^^^^^^^^^^^^^^^^

Open the package settings and set ``chapel_home_directory`` to the path of your Chapel ``CHPL_HOME``.

Specifying the path to the executable:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Open the package settings and set ``chapel_language_server_path`` to the path of the Chapel language server executable.
