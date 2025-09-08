chpl-venv devel files
=====================

As the Makefile installs end-user Python packages into the virutalenv
(described in [README.md](README.md)), the Makefile.devel installs
Python packages required by the developer tool `chplspell`.


The primary package required by `chplspell` is `scspell3k`.

The primary developer packages required by `chpl-language-server` (CLS) are
`pytest` and `pytest-lsp`.  The general packages requires by CLS are mentioned
in README.md.


### scspell3k

A spell checker for source code

**program License**: GPL 2

**word list License**: public domain, assorted (See https://github.com/myint/scspell/blob/master/SCOWL-LICENSE.txt)

**Website**: https://pypi.python.org/pypi/scspell3k

### pytest

A well-known Python testing framework.

Required for testing CLS.

**License**: MIT

**Website**: https://pypi.org/project/pytest/

### pytest-lsp

A pytest plugin for end-to-end testing of language servers.

Required for testing CLS.

**License**: MIT

**Website**: https://pypi.org/project/pytest-lsp/
