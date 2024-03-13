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

The easiest way to make ``CLS`` available ``chpl-language-server`` Makefile
target. This will build the Dyno compiler frontend and the Python bindings for
Dyno if needed, and place ``chpl-language-server`` into ``$CHPL_HOME/bin``. In
from-source builds, this directory should already be in your ``PATH``; this
means you should be invoke ``chpl-language-server`` from the command-line
without any additional work.

``CLS`` can be used with any editor that supports the Language Server Protocol
(LSP). Listed below are setup instructions for some popular editors. If your
preferred editor is not listed, consider opening an issue or pull request to
add it.

Neovim
^^^^^^

In Neovim, the built-in LSP API can be used to configure ``CLS`` as
follows:

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

Install the soon-to-be-released ``chapel`` extension from the `Visual Studio
Code marketplace
<https://marketplace.visualstudio.com/items?itemName=chpl-hpe.chapel>`_.

.. note::

   The extension is not yet available at the time of writing and the above link
   may not work until then. This section will be updated when it is available.

Supported Features
------------------

``CLS`` supports a number of core features, with further features controlled by configuration options.

Core Features
^^^^^^^^^^^^^

Optional Features
^^^^^^^^^^^^^^^^^

Experimental Resolver Features
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

All of the following features are disabled without the ``--resolver`` flag. To use them, enable ``--resolver``.

.. warning::

   These features rely on the Dyno resolver, which is not finished and actively
   under development. It is not unexpected for ``--resolver`` to cause ``CLS``
   to crash or hang.
