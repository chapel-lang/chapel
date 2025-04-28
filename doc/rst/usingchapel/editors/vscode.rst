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


.. _vscode-debugging:

Debugging in VSCode
~~~~~~~~~~~~~~~~~~~

The Chapel VSCode extension provides builtin support for debugging Chapel
programs. Users can set breakpoints and inspect variables directly in the
editor. This is done by configuring a ``launch.json`` file in the ``.vscode``
directory of your project. See the `VSCode documentation on debugging
<https://code.visualstudio.com/docs/debugtest/debugging>`_ for more information
on creating launch configurations.

Since version 0.0.7, the extension also provides builtin support to generate
``launch.json`` files for Chapel programs via the "Create a debug launch.json
for Chapel" command, which can be invoked from the `command palette
<https://code.visualstudio.com/docs/getstarted/userinterface#_command-palette>`_.
If custom arguments are needed, its recommend to generate a default
``launch.json`` file and modify it to your needs.

.. image:: editors/vscode-debug.png
  :alt: Screenshot of using the Chapel extension to generate a launch.json configuration file for Chapel

See :ref:`readme-debugging-bkc` for the best way to build your Chapel program
for the best possible experience. Other configurations do work, but this works
best.

Building and running Chapel code with Tasks
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The Chapel extension also provides support for building and running Chapel code
with the ``chpl`` and ``chpl-run`` tasks. The extension automatically detects
and creates tasks using the compiler in the default ``PATH`` with default
settings. You can also create your own tasks by creating a ``tasks.json`` file
in the ``.vscode`` directory of your project. See the `VSCode documentation on tasks
<https://code.visualstudio.com/docs/editor/tasks>`_ for more information on
creating tasks. For a good starting point, it is recommended to reconfigure one of
the default tasks.
