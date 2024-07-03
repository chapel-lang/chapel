.. _vim:

Vim
---

.. _vim-syntax:

Syntax Highlighting
~~~~~~~~~~~~~~~~~~~

TODO: add instructions from highlight/vim/README.rst

.. _vim-lsp:

Language Server Support
~~~~~~~~~~~~~~~~~~~~~~~

TODO: Add instructions for setting up Chapel language server in Vim.

1. Install vim-plug

.. code-block:: bash

   curl -fLo ~/.vim/autoload/plug.vim --create-dirs \
       https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim

2. Install vim-lsp extension

Make sure to open vim and run ``:PlugInstall``

3. write config

.. code-block:: vim

      call plug#begin()

      Plug 'prabirshrestha/vim-lsp'

      call plug#end()



      if executable('chpl-language-server')
          au User lsp_setup call lsp#register_server({
              \ 'name': 'cls',
              \ 'cmd': {server_info->['chpl-language-server', "--resolver"]},
              \ 'allowlist': ['chpl'],
              \ })
      endif

      function! s:on_lsp_buffer_enabled() abort
          setlocal omnifunc=lsp#complete
          setlocal signcolumn=yes
          if exists('+tagfunc') | setlocal tagfunc=lsp#tagfunc | endif
          nmap <buffer> gd <plug>(lsp-definition)
          nmap <buffer> gs <plug>(lsp-document-symbol-search)
          nmap <buffer> gS <plug>(lsp-workspace-symbol-search)
          nmap <buffer> gr <plug>(lsp-references)
          nmap <buffer> gi <plug>(lsp-implementation)
          nmap <buffer> gt <plug>(lsp-type-definition)
          nmap <buffer> <leader>rn <plug>(lsp-rename)
          nmap <buffer> [g <plug>(lsp-previous-diagnostic)
          nmap <buffer> ]g <plug>(lsp-next-diagnostic)
          nmap <buffer> K <plug>(lsp-hover)
          nnoremap <buffer> <expr><c-f> lsp#scroll(+4)
          nnoremap <buffer> <expr><c-d> lsp#scroll(-4)

      endfunction

      augroup lsp_install
          au!
          " call s:on_lsp_buffer_enabled only for languages that has the server registered.
          autocmd User lsp_buffer_enabled call s:on_lsp_buffer_enabled()
      augroup END

      let g:lsp_document_highlight_enabled = 1
      highlight lspReference ctermfg=red guifg=red ctermbg=green guibg=green
      let g:lsp_inlay_hints_enabled = 1
      highlight lspInlayHintsType ctermfg=grey guifg=grey

      au BufRead,BufNewFile *.chpl set filetype=chpl


