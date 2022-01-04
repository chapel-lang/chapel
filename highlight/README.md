Chapel highlighting/formatting support
======================================

This directory contains support for code coloring/formatting using
various editors and highlighting tools.  The level of support and
complete language coverage may vary between tools.

Highlighting for Editors
------------------------

* emacs/ : support for highlighting in emacs (see `emacs/README.rst`
* geany/ : support for highlighting in geany (see `geany/README.md`)
* vim/ : support for highlighting in 'vim' (see `vim/README.rst`)

The emacs and vim modes for Chapel were created by modifying existing
language formatting modes for the respective editors and are therefore
licensed according to the agreements of those packages (i.e., the GPL
for emacs and the VIM license for vim).  See the LICENSE files in the
emacs/ and vim/ subdirectories for details.

Other Highlighting Options
--------------------------

* highlight/ : used by the Computer Lanugage Benchmarks Game
* latex/ : support for Chapel in the lstlistings package in LaTeX
* source-highlight/ : support for GNU source-highlight

Pointers to other external highlighting technologies
----------------------------------------------------

* highlight.js : See https://github.com/chapel-lang/highlightjs-chapel
* pygments/ : See `pygments/README.md` for more information
* tmbundle/ : See `tmbundle/README.md` for more information
