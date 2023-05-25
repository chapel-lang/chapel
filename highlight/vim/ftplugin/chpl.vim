" Disable repeating * in comments, because chpldoc doesn't understand them,
" so that isn't the Chapel style.
setlocal comments=s1:/*,ex:*/,://

" Use list formatting at all
setlocal formatoptions+=n
" Allow 1. * - lists (for use in chpl-doc comments)
setlocal formatlistpat=^\\s*\\(\\d\\+[\\]:.)}\\t\ ]\\\|[*-][\\t\ ]\\)\\s*
