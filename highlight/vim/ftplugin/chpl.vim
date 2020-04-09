" Disable repeating * in comments, because chpldoc doesn't understand them,
" so that isn't the Chapel style.
setlocal comments=s1:/*,ex:*/,://

" Use list formatting at all
setlocal formatoptions+=n
" Allow 1. * - lists (for use in chpl-doc comments)
setlocal formatlistpat=^\\s*\\(\\d\\+[\\]:.)}\\t\ ]\\\|[*-][\\t\ ]\\)\\s*

" If you prefer spaces to tabs, uncomment the following:
"  (To get a real tab with these settings, do CTRL-V TAB)
setlocal expandtab       " replace indents with spaces
setlocal shiftwidth=2    " set how many spaces per indent
setlocal smarttab        " use spaces as indents at the beginning of lines


