.. _vscode:

VSCode
------

.. _vscode-syntax:

Syntax Highlighting
~~~~~~~~~~~~~~~~~~~

Syntax highlighting is provided by the ``Chapel Language`` extension.
To install the extension, visit the
`Visual Studio Code marketplace <https://marketplace.visualstudio.com/items?itemName=chpl-hpe.chapel-vscode>`_.

.. _vscode-lsp:

Language Server Support
~~~~~~~~~~~~~~~~~~~~~~~

The ``Chapel Language`` extension provides builtin support for the :ref:`Chapel
language server <readme-chpl-language-server>` and :ref:`readme-chplcheck`. If
``chpl-language-server`` and ``chplcheck`` are in your ``$PATH``, the extension
will automatically use it. If not, you can either specify you ``CHPL_HOME`` in
the extension settings or specify the path to the executables.

Specifying ``CHPL_HOME``:
^^^^^^^^^^^^^^^^^^^^^^^^^

Open the settings for the Chapel extension and set ``chapel.CHPL_HOME`` to the
path of your Chapel ``CHPL_HOME``.

Specifying the path to the executables:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Open the settings for the Chapel extension and set
``chapel.chpl-language-server.path`` and ``chapel.chplcheck.path`` to the path
of the Chapel language server and chplcheck executables, respectively.
