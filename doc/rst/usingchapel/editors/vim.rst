.. _vim:

Vim
---

Chapel support for Vim has been designed for use with Vim versions 5.08 or
newer.

.. _vim-syntax:

Syntax Highlighting
~~~~~~~~~~~~~~~~~~~

To get syntax highlighting for Chapel in Vim, either manually copy the files
top-level ``highlight/vim`` directory of the Chapel source tree into your
``$HOME/.vim`` directory or use a plugin manager like `vim-plug`_ to install an unmanaged plugin.

Manual Installation:
^^^^^^^^^^^^^^^^^^^^

1. Create a directory in the root of your home directory called ``.vim``.

   .. code-block:: bash

      mkdir -p $HOME/.vim

2. Copy the relevant syntax files to your ``.vim`` directory. This command assumes your current working directory is the Chapel source tree.

   .. code-block:: bash

      cp -r highlight/vim/ftdetect \
            highlight/vim/ftplugin \
            highlight/vim/syntax \
            highlight/vim/indent \
            $HOME/.vim

3. Add the following lines to your standard ``.vimrc`` file:

   .. code-block:: vim

      set nocompatible           " allow for non-vi compatible features
      syntax on                  " enable syntax highlighting
      filetype plugin indent on  " enable filetype detection, plugins, indent
      set autoindent             " turn on auto indenting

   Other useful commands are available in the
   ``highlight/vim/sample-vimrc`` file.

Vim-Plug Installation:
^^^^^^^^^^^^^^^^^^^^^^

For users of the `vim-plug`_ plugin manager, the vim Chapel support can be
loaded as an unmanaged plugin. To do this, add the following line to the
``.vimrc``, between the ``call plug#begin()`` and ``call plug#end()`` lines:

.. code-block:: vim

    Plug '~/path/to/chapel/highlight/vim'

Where ``~/path/to/chapel`` is the path to the Chapel repository, starting from
the home directory. Chapel support should work once these changes are made
without any calls to ``:PlugInstall``, which is required for managed plugins.

Note that vim-plug uses the ``~`` character to identify that a plugin is a local
file, so the Chapel repository, or at least the ``highlight/vim/`` directory,
must be somewhere under the home directory.

.. _vim-plug: https://github.com/junegunn/vim-plug

.. _vim-lsp-chpl:

Language Server Support
~~~~~~~~~~~~~~~~~~~~~~~

The use the :ref:`Chapel language server <readme-chpl-language-server>` and :ref:`readme-chplcheck`, you will need to have an extension that supports the Language Server Protocol (LSP). There are a few options for this, these instructions will use the `vim-lsp <https://github.com/prabirshrestha/vim-lsp>`_ extension with `vim-plug`_.

Using vim-lsp:
^^^^^^^^^^^^^^

Note that these instructions assume you have already setup Vim with Chapel syntax highlighting as described in the previous section.

1. Assuming you have already installed the `vim-plug`_ plugin manager, you can add the following lines to your ``.vimrc`` file, between the between the ``call plug#begin()`` and ``call plug#end()`` lines:

   .. code-block:: vim

      Plug 'prabirshrestha/vim-lsp'

2. After the ``call plug#end()`` line, add the following configuration. This
   configuration registers the Chapel language server with the vim-lsp
   extension and sets up some example keybindings for common LSP operations:

   .. code-block:: vim

      if executable('chpl-language-server')
          au User lsp_setup call lsp#register_server({
              \ 'name': 'cls',
              \ 'cmd': {server_info->['chpl-language-server', '--chplcheck']},
              \ 'allowlist': ['chpl'],
              \ })
      endif

      " Configure the lsp plugin for Chapel
      " This function sets up a few example keybindings
      function! s:on_lsp_buffer_enabled() abort
          setlocal omnifunc=lsp#complete
          setlocal signcolumn=yes
          if exists('+tagfunc') | setlocal tagfunc=lsp#tagfunc | endif
          nmap <buffer> gd <plug>(lsp-definition)
          nmap <buffer> gs <plug>(lsp-document-symbol-search)
          nmap <buffer> gS <plug>(lsp-workspace-symbol-search)
          nmap <buffer> gr <plug>(lsp-references)
          nmap <buffer> <leader>rn <plug>(lsp-rename)
          nmap <buffer> [g <plug>(lsp-previous-diagnostic)
          nmap <buffer> ]g <plug>(lsp-next-diagnostic)
          nmap <buffer> K <plug>(lsp-hover)
      endfunction

      augroup lsp_install
          au!
          autocmd User lsp_buffer_enabled call s:on_lsp_buffer_enabled()
      augroup END

      " Enables lsp inlay hints and adds some styling
      let g:lsp_inlay_hints_enabled = 1
      highlight lspInlayHintsType ctermfg=grey guifg=grey

   For other configuration options for the `vim-lsp
   <https://github.com/prabirshrestha/vim-lsp>`_ extension, see the `vim-lsp
   documentation
   <https://github.com/prabirshrestha/vim-lsp/blob/master/doc/vim-lsp.txt>`_.

   .. note::

      The above configuration assumes that the Chapel language server is installed and available in your ``$PATH``. If this is not the case, you will need to modify the ``cmd`` field in the ``lsp#register_server`` calls to point to the correct location of the language server executable.


3. Make sure to open ``vim`` and run ``:PlugInstall`` to install the plugin.
