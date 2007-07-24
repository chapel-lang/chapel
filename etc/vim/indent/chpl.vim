" Vim indent file
" Language:	Chapel
" Maintainer:	James Dinan <dinan@cray.com>
" Last Change:	2007 July 23

" Only load this indent file when no other was loaded.
if exists("b:did_indent")
   finish
endif
let b:did_indent = 1

" Use built in C/C++ style
setlocal cindent
