Pygments Chapel Lexer
=====================

`Pygments <http://pygments.org/>`_ is an open source, generic syntax
highlighter. There is a ``ChapelLexer`` class that is used to highlight Chapel
source code.

Updating ChapelLexer
--------------------

If and when the lexer needs to be updated, follow these instructions to setup
your local environment and test changes locally.

* Fork and clone the `pygments-main project from bitbucket
  <https://bitbucket.org/birkenfeld/pygments-main>`_.
* The ``ChapelLexer`` is in ``pygments/lexers/compiler.py``.
* See the `pygments developer docs <http://pygments.org/#contribute>`_ for
  specific information.
* There is a test Chapel program that can be used to validate any changes. A
  convenient way to see how it works from the command line is:

.. code-block:: bash

    pygmentize tests/examplefiles/99_bottles_of_beer.chpl
