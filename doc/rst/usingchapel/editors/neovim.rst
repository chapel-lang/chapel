.. _neovim:

Neovim
------

.. _neovim-syntax:

Syntax Highlighting
~~~~~~~~~~~~~~~~~~~

See the :ref:`Vim Syntax Highlighting <vim-syntax>` section for how to setup syntax highlighting in Neovim.

.. _neovim-lsp:

Language Server Support
~~~~~~~~~~~~~~~~~~~~~~~

The built-in LSP API can be used to configure :ref:`Chapel language server
<readme-chpl-language-server>` and :ref:`readme-chplcheck` by adding the
following to your Neovim configuration file. Note that this assumes you have
already installed `lspconfig
<https://github.com/neovim/nvim-lspconfig?tab=readme-ov-file#quickstart>`_.

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
   configs.chplcheck = {
     default_config = {
       cmd = {"chplcheck", "--lsp"},
       filetypes = {'chpl'},
       autostart = true,
       single_file_support = true,
       root_dir = util.find_git_ancestor,
       settings = {},
     },
   }

   lspconfig.cls.setup{}
   lspconfig.chplcheck.setup{}
   vim.cmd("autocmd BufRead,BufNewFile *.chpl set filetype=chpl")
