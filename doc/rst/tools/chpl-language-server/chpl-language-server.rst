.. _readme-chpl-language-server:

chpl-language-server
====================

``chpl-language-server`` (``CLS`` for short) is a language intelligence tool
for the Chapel programming language. ``CLS`` provides many of the common
features users expect from a modern development environment. It is built on top
of the `Language Server Protocol (LSP)
<https://microsoft.github.io/language-server-protocol/>`_ and is designed to be
editor-agnostic.

Getting Started
---------------

The easiest way to make ``CLS`` available is by using the
``chpl-language-server`` Makefile target. This will build the Dyno compiler
frontend and the Python bindings for Dyno if needed, and place
``chpl-language-server`` into ``$CHPL_HOME/bin``. Make sure that you satisfy :ref:`the requirements for building the Python bindings <chapel-py-installation>`.

.. code-block:: bash

   cd $CHPL_HOME
   make chpl-language-server
   chpl-language-server --help

``CLS`` can be used with any editor that supports the LSP. Listed below are
setup instructions for some popular editors. If your preferred editor is not
listed, consider opening an `issue
<https://github.com/chapel-lang/chapel/issues/new>`_ or `pull request
<https://github.com/chapel-lang/chapel/pull/new>`_ to add it.

Neovim
^^^^^^

The built-in LSP API can be used to configure ``CLS`` as follows:

.. code-block:: lua

   local lspconfig = require 'lspconfig'
   local configs = require 'lspconfig.configs'
   local util = require 'lspconfig.util'

   configs.cls = {
     default_config = {
       cmd = {"chpl-language-server"},
       filetypes = {'chpl'},
       autostart = true,
       single_file_support = true,
       root_dir = util.find_git_ancestor,
       settings = {},
     },
   }
   
   lspconfig.cls.setup{}
   vim.cmd("autocmd BufRead,BufNewFile *.chpl set filetype=chpl")


VSCode
^^^^^^

Install the ``chapel`` extension from the `Visual Studio Code marketplace
<https://marketplace.visualstudio.com/items?itemName=chpl-hpe.chapel-vscode>`_.

.. note::

   The extension is not yet available at the time of writing and the above link
   may not work until then. This section will be updated when it is available.

Supported Features
------------------

``CLS`` supports a number of core features, with further features controlled by configuration options.

.. note::

   Not all editor integrations may support all features. This list details
   features that are supported by ``CLS`` itself, not by any particular editor.

Core Features
^^^^^^^^^^^^^

* Diagnostics
   Errors and warnings are reported on file save. ``CLS`` also has limited
   support to fix some errors and warnings automatically with quick fixes.
* Symbol Information
   ``CLS`` provides basic information about symbols in code, enabling various
   actions. For example, you can jump to the definition of a symbol, find all
   references to a symbol, and rename a symbol. Hovering over a symbol will
   show its definition and documentation. Selecting a symbol will highlight all
   references to it in the file.
* Code Completion
   ``CLS`` provides limited code completion for symbols imported from modules
   or present in your code.

Optional Features
^^^^^^^^^^^^^^^^^

* End of Block Markers
   ``CLS`` can display markers after a closing brace to indicate what block it
   closes. This feature is off-by-default, but can be enabled with
   ``--end-markers <marker_list>``.

   ``<marker_list>`` is a comma-separated list of markers to display. The
   following markers are supported:

      * ``none``: Do not display any end markers. This is the default.
      * ``all``: Display all end markers.
      * ``decl``: Display markers for the end of a declaration (e.g. modules,
        records, functions, etc.)
      * ``loop``: Display markers for the end of a loop (e.g. ``for``,
        ``forall``, ``while``, etc.)
      * ``block``: Display markers for the end of other important blocks (e.g.
        ``on``, ``begin``, etc.)

   End of block markers have a threshold of 10 lines of code. If a block is
   smaller than this, no end marker will be displayed. This threshold can be
   adjusted with ``--end-marker-threshold <threshold>``.

Experimental Resolver Features
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All of the following features are disabled without the ``--resolver`` flag. To
use them, enable ``--resolver``.

.. warning::

   These features rely on the Dyno resolver, which is not finished and actively
   under development. It is fairly common for ``--resolver`` to cause ``CLS``
   to crash or hang.

* Type Information
   ``CLS`` can resolve the type of a symbol and allow jumping to type
   definitions.
* Call Hierarchy
   Some editors support showing a call hierarchy for a symbol, both inbound
   calls and outbound calls. ``CLS`` supports this basic feature, as well as
   enabling some additional features for this with generic functions.

The following features are extra visual aids:

+----------------+--------------------------------------------+---------------------------------------+
| Feature        | Description                                | Flag                                  |
+----------------+--------------------------------------------+---------------------------------------+
| Type Inlays    | Type information can be displayed inline   | ``--type-inlays`` (default),          |
|                | as an inlay hint.                          | ``--no-type-inlays``                  |
+----------------+--------------------------------------------+---------------------------------------+
| Param Inlays   | ``param`` values can be computed and       | ``--param-inlays`` (default),         |
|                | displayed inline as inlay hints.           | ``--no-param-inlays``                 |
+----------------+--------------------------------------------+---------------------------------------+
| Evaluated      | ``param`` values can also be computed and  | ``--evaluate-expressions`` (default), |
| Tooltips       | displayed as tooltips.                     | ``--no-evaluate-expressions``         |
+----------------+--------------------------------------------+---------------------------------------+
| Call Inlays    | Names of literal arguments can be          | ``--literal-arg-inlays`` (default),   |
|                | displayed inline as inlay hints.           | ``--no-literal-arg-inlays``           |
+----------------+--------------------------------------------+---------------------------------------+
| Dead Code      | Dyno can determine compile-time dead code, | ``--dead-code`` (default),            |
|                | which ``CLS`` highlights in the editor.    | ``--no-dead-code``                    |
+----------------+--------------------------------------------+---------------------------------------+
| Generic        | ``CLS`` can show the various               | No flag, on by default                |
| Instantiations | instantiations of a generic function.      |                                       |
+----------------+--------------------------------------------+---------------------------------------+

Configuring Chapel Projects
---------------------------

Many Chapel projects are organized in a way that is not immediately
understandable by ``CLS``. For example, a project may have multiple source
directories with any variety of build systems (``make``, ``mason``, etc.).
``CLS`` can be configured to understand the structure of a Chapel project by
creating a ``.cls-commands.json`` file in the root of the project. This is done
automatically when ``chpl-shim`` is used to build a project.

.. note::

   The ``.cls-commands.json`` file is not intended to be edited by hand. It is
   generated by ``chpl-shim`` and should be treated as a build artifact. It is specific to the machine and build environment that generated it.

For example, the following can be used to configure ``CLS`` to understand a
project using ``make``:

.. code-block:: bash

   $CHPL_HOME/tools/chpl-language-server/chpl-shim make

This is similarly done for ``mason`` projects:

.. code-block:: bash

   $CHPL_HOME/tools/chpl-language-server/chpl-shim mason build

.. note::

   The above commands assume a from-source build of Chapel. An installed Chapel
   may require a different path to ``chpl-shim``.

.. note::

   First-class ``mason`` support is currently planned (but not yet
   implemented), avoiding the need for ``chpl-shim`` in ``mason`` projects.
