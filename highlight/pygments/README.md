Pygments Chapel Lexer
=====================

[Pygments][0] is an open source, generic syntax highlighter. There is a
`ChapelLexer` class that is used to highlight Chapel source code.

[0]: http://pygments.org/

Updating ChapelLexer
--------------------

If and when the lexer needs to be updated, follow these instructions to setup
your local environment and test changes locally.

* Fork and clone the [pygments-main project from github][1].
* The `ChapelLexer` is in `pygments/lexers/chapel.py`.
* See the [pygments developer docs][2] for specific information.
* There is a test Chapel program that can be used to validate any changes. A
  convenient way to see how it works from the command line is:

```bash
pygmentize tests/examplefiles/99_bottles_of_beer.chpl
```

[1]: https://github.com/pygments/pygments
[2]: http://pygments.org/#contribute
