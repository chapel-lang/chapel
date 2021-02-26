.. default-domain:: chpl

.. _Chapter-Notation:

Notation
========

Special notations are used in this specification to denote Chapel code
and to denote Chapel syntax.

Chapel code is represented with a fixed-width font where keywords are
bold and comments are italicized.

   *Example*.

   

   .. code-block:: chapel

      for i in D do   // iterate over domain D
        writeln(i);   // output indices in D

Chapel syntax is represented with standard syntax notation in which
productions define the syntax of the language. A production is defined
in terms of non-terminal (*italicized*) and terminal (non-italicized)
symbols. The complete syntax defines all of the non-terminal symbols in
terms of one another and terminal symbols.

A definition of a non-terminal symbol is a multi-line construct. The
first line shows the name of the non-terminal that is being defined
followed by a colon. The next lines before an empty line define the
alternative productions to define the non-terminal.

   *Example*.

   The production 

   .. code-block:: syntaxdonotcollect

      bool-literal:
        'true'
        'false'

   defines ``bool-literal`` to be either the symbol ``true``
   or ``false``.

In the event that a single line of a definition needs to break across
multiple lines of text, more indentation is used to indicate that it is
a continuation of the same alternative production.

As a short-hand for cases where there are many alternatives that define
one symbol, the first line of the definition of the non-terminal may be
followed by “one of” to indicate that the single line in the production
defines alternatives for each symbol.

   *Example*.

   The production 

   .. code-block:: syntaxdonotcollect

      unary-operator: one of
        + - ~ !

   is equivalent to 

   .. code-block:: syntaxdonotcollect

      unary-operator:
        +
        -
        ~
        !

As a short-hand to indicate an optional symbol in the definition of a
production, the subscript “opt” is suffixed to the symbol.

   *Example*.

   The production 

   .. code-block:: syntaxdonotcollect

      formal:
        formal-tag identifier formal-type[OPT] default-expression[OPT]

   is equivalent to 

   .. code-block:: syntaxdonotcollect

      formal:
        formal-tag identifier formal-type default-expression
        formal-tag identifier formal-type
        formal-tag identifier default-expression
        formal-tag identifier
