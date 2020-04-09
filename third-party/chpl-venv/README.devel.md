chpl-venv devel files
=====================

As the Makefile installs end-user Python packages into the virutalenv
(described in [README.md](README.md)), the Makefile.devel installs
Python packages required by the developer tool `chplspell`.


The primary packages required by `chplspell` are: `scspell`,
`argparse`, and `virtualenv` (for installation).  `argparse` and
`virtualenv` are described in [README.md](README.md) as they are also
required by end-user tools.


### scspell

A spell checker for source code

**program License**: GPL 2

**word list License**: public domain, assorted (See https://github.com/myint/scspell/blob/master/SCOWL-LICENSE.txt)

**Website**: https://pypi.python.org/pypi/scspell3k
