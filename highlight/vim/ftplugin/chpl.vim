" Disable repeating * in comments, because chpldoc doesn't understand them,
" so that isn't the Chapel style.
setlocal comments=s1:/*,ex:*/,://

" Use list formatting at all
setlocal formatoptions+=n
" Allow 1. * - lists (for use in chpl-doc comments)
setlocal formatlistpat=^\\s*\\(\\d\\+[\\]:.)}\\t\ ]\\\|[*-][\\t\ ]\\)\\s*

" To control tab settings for .chpl files, you can use setlocal commands
" in this file.
" Below are some example settings, but adjust to your preference.
"
" setlocal expandtab       " replace indents with spaces
" setlocal shiftwidth=2    " set how many spaces per indent
" setlocal smarttab        " use spaces as indents at the beginning of lines
