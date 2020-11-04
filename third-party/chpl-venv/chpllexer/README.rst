This is a Python package that is a local snapshot of the  Pygments highlighter
that Thomas Van Doren originally developed as a member of the Chapel team and
contributed to the Pygments project at https://github.com/pygments/pygments.

In November 2020, we decided to bring a copy of it back into our source tree in
order to avoid lags in published pygments versions and shorten the time between
any improvements to this lexer and seeing their impact on the generated
documentation.

See https://github.com/chapel-lang/chapel/issues/14623 for motivation.

Changes made to chpllexer/chapel.py should be reflected downstream for making
the improvements publicly available through Pygments.
