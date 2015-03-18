Documenting Chapel
==================

The Chapel standard libraries are documenting using ``chpldoc``, which
generates HTML-based documentation for Chapel code from comments embedded
within the code.

.. FIXME: Add link to web documentation, when it becomes available (thomasvandoren, 2015-03-10)

**Contents**:

#. chpldoc_
#. Prerequisites_
#. `Generating documentation`_
#. `Documenting Chapel code`_
#. `reStructuredText primer`_
#. `Advanced chpldoc options`_
#. `How chpldoc works`_
#. `Future directions`_

.. _chpldoc:

chpldoc
-------

``chpldoc`` is a tool for generating HTML based documentation from Chapel
source code and embedded comments. It is similar in spirit to Javadoc and
Pydoc. To build ``chpldoc``, use::

    [g]make chpldoc

This will downloads the required Python package dependencies and creates the
``chpldoc`` program in the same location as the ``chpl`` compiler.

To ensure chpldoc is installed properly, optionally run the automatic sanity
check::

    [g]make check-chpldoc


.. _Prerequisites:

Prerequisites
-------------

``chpldoc`` depends on several Python packages. The full list is available in
`$CHPL_HOME/third-party/chpldoc-venv/README.md`_ including details on each
package.

.. _$CHPL_HOME/third-party/chpldoc-venv/README.md: https://github.com/chapel-lang/chapel/blob/master/third-party/chpldoc-venv/README.md


.. _Generating documentation:

Generating documentation
------------------------

The easiest way to build the module documentation is to use the top level make
target::

   [g]make module-docs

This will create HTML documentation for the standard modules and place it in
``$CHPL_HOME/modules/docs/``. Open the index.html file in a browser to view it.

To document individual modules, inside the Chapel standard library or
elsewhere, use the chpldoc command directory. For example::

   chpldoc modules/standard/Random.chpl

By default, documentation created by chpldoc will end up in a subdirectory of
the current working directory named 'docs/'. You can specify a different
directory name using the --docs-dir option.

As an example, there is a primer on the chpldoc capability in
``$CHPL_HOME/examples/primers/`` that can be turned into documentation using::

   cd $CHPL_HOME/examples/primers
   chpldoc chpldoc.chpl

The output documentation will be located in::

   $CHPL_HOME/examples/primers/docs/chpldoc.html


.. _Documenting Chapel code:

Documenting Chapel code
-----------------------

To document Chapel code, place a multi-line comment just prior to the
definition of the symbol --- module, class, function, method, global variable,
etc. --- that you wish to comment. All multi-line comments are considered
potential documentation. Documentation is output in the same order that it
exists in the source. Documentation text within comments is parsed as
ReStructed Text.


Comment style
~~~~~~~~~~~~~

For the Chapel standard modules, it is recommended to use multiline comments
without any per-line prefixes. This results in the most consistent, best
looking output. For example:

.. code-block:: chapel

   /* Support for buffers - ...

      Provides bytes and buffer types, which can be used to...
    */
   module Buffers {
     ...


Argument and return/yield types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Arguments and their types can optionally be documented using the ``:arg:`` and
``:type:`` fields. For the Chapel standard modules, ``:arg:`` should be a
concise description of the argument using a sentence, or two. This should
include any constraints and default information. ``:type:`` should be the
literal type for the parameter. A link will be created to the type, if
possible. For example:

.. code-block:: chapel

   /* Fill and array with pseudorandom values ...

      :arg arr: Array to be filled, where T is real(64), imag(64), or complex(128).
      :type arr: [] T

      :arg seed: Integer seed to use for the PRNG. Defaults to :proc:`SeedGenerator.currentTime`.
      :type seed: int(64)
    */
   proc fillRandom(arr: [], seed: int(64) = SeedGenerator.currentTime) {
     ...

Return or yield types can optionally be documented using the ``:returns:`` and
``:yields:`` fields, and the ``:rtype:`` and ``:ytype:`` fields. ``:returns:``
and ``:rtype:`` are for procedures and methods, while ``:yields:`` and
``:ytype:`` are for iterators.

Similar to arguments, the ``:returns:`` and ``:yields:`` should be a concise
description of the return value or yield value, including any constraints,
using a sentence, or two. ``:rtype:`` and ``:ytype:`` should be the literal
types. A link to the type will be created, if possible. For example:

.. code-block:: chapel

   /* Returns the next value in the random stream.

      :returns: Next value in the random integer stream.
      :rtype: int
    */
   proc RandomStream.getNext(): int {
     ...

   /* Simple find-like utility for iterating over files.

      :arg startdir: Directory to start looking for files.
      :arg recursive: Whether or not to descend recursively into directories.
      :arg hidden: Whether or not to yield hidden files.

      :yields: Filenames found in `startdir` based on input arguments.
      :ytype: string
    */
   iter findfiles(startdir=".", recursive=false, hidden=false): string {
     ...

.. note::

   * Leave an empty line above and below all these fields for best results.
   * All of these fields must be left-aligned with the outer most paragraph(s).

For more information see the `Documenting argument, return, and yield values
and types <#documenting-argument-return-and-yield-values-and-types>`_ section.


Stifling documentation
~~~~~~~~~~~~~~~~~~~~~~

To mark a particular symbol to not be output as part of the documentation,
preface the symbol with the pragma "no doc". For example:

.. code-block:: chapel

   pragma "no doc"
   proc foo() { ... }


Creating links to other symbols
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

See the `hyperlinks <#hyperlinks>`_ section for details on creating links, also
known as cross-references, in the documentation to other symbols.


.. _reStructuredText primer:

reStructuredText primer
-----------------------

This section is a brief introduction to reStructuredText (aka reST). It is
intended to provide Chapel developers with enough information to write
documentation in comments in the source code.

The Python and Sphinx projects have thorough primers of reST. Please see those
for more detail.

* `Sphinx reST Primer`_
* `Python reST Primer`_

The authoritative `reStructuredText User Guide`_ is also helpful.

.. _Sphinx reST Primer: http://sphinx-doc.org/rest.html
.. _Python reST Primer: https://docs.python.org/devguide/documenting.html#restructuredtext-primer
.. _reStructuredText User Guide: http://docutils.sourceforge.net/rst.html


Paragraphs
~~~~~~~~~~

Simple chunks of text. Paragraphs can have line breaks to improve source
readability. Separate paragraphs with an empty line. Indentation is significant
in reST. All lines of paragraph must be left-aligned.


Inline markup
~~~~~~~~~~~~~

* one asterisk: ``*text*`` for emphasis (italics),
* two asterisks: ``**text**`` for strong emphasis (boldface), and
* backquotes: ````text```` for code samples.

If asterisks or backquotes appear in running text and could be confused with
inline markup delimiters, they have to be escaped with a backslash.


Lists and Quotes
~~~~~~~~~~~~~~~~

To create a list, put an asterisk at the start of a paragraph and indent
accordingly. Create numbered lists by using the literal numbers, e.g. ``1.``,
``2.``. Automatically numbered lists begin with ``#.``::

   * This is a bulleted list.
   * It has two items, the second
     item uses two lines.

   1. This is a numbered list.
   2. It has two items too.

   #. This is a numbered list.
   #. It has two items too.

Nested lists are possible, but be aware that they must be separated from the
parent list items by blank lines::

   * a
   * b

     * nested
     * blah

   * my final item, in the parent list


Source Code
~~~~~~~~~~~

Literal code blocks are introduced by ending a paragraph with the special marker
``::``.  The literal block must be indented::

   This is a normal text paragraph. The next paragraph is a code sample::

      It is not processed in any way, except
      that the indentation is removed.

      It can span multiple lines.

   This is a normal text paragraph again.

The handling of the ``::`` marker is smart:

* If it occurs as a paragraph of its own, that paragraph is completely left
  out of the document.
* If it is preceded by whitespace, the marker is removed.
* If it is preceded by non-whitespace, the marker is replaced by a single
  colon.

That way, the second sentence in the above example's first paragraph would be
rendered as "The next paragraph is a code sample:".

The highlight language is configured with the ``highlight`` directive. The
configured language is used for all literal blocks until the next highlight
directive. For example::

   .. highlight:: chapel

   Chapel code::

      writeln("Hello from Chapel!");

   More chapel::

      x <=> y;

   .. highlight:: c

   ::

      printf("Hello from C!\n");


Showing code examples
~~~~~~~~~~~~~~~~~~~~~

The ``code-block`` directive can be used to specify the highlight language of a
single code block. For example::

   .. code-block:: chapel

      use Foo;

      proc bar() {
        writeln("Fooy!");
      }

If highlighting with the specified language fails, e.g. if the syntax is not
parsable, the block is not highlighted in anyway. Note that there should be a
blank line between the ``code-block`` directive and the indented code snippet.


Hyperlinks
~~~~~~~~~~


External links
++++++++++++++

Use ```Link text <http://target>`_`` for inline web links.  If the link text
should be the web address, you don't need special markup at all, the parser
finds links and mail addresses in ordinary text.


Internal links
++++++++++++++

Internal linking is done via a special reST role, see the section on specific
markup `cross-references <#inline-markup-1>`_.


Sections
~~~~~~~~

Section headers are created by underlining (and optionally overlining) the
section title with a punctuation character, at least as long as the text::

   This is a heading
   =================

There are no heading levels assigned to certain characters. The structure is
determined from the succession of headings.


Comments
~~~~~~~~

Every explicit markup block, which is not a valid markup construct is regarded
as a comment. For example::

   This is a normal paragraph.
   It is interesting.

   .. TODO: Make it more interesting.

   Another paragraph goes here.

   .. add another paragraph below

You can indent text after a comment start to form multiline comments::

   ..
      This whole indented block
      is a comment.

      Still in the comment.


Inline markup
~~~~~~~~~~~~~

As said before, Sphinx uses interpreted text roles to insert semantic markup in
documents.

Names of builtins, like `true`, `false`, types like `int(64)`, and local
variables, such as function/method arguments, are an exception, they should be
marked simply with ```myVar```.

For all other roles, you have to write ``:rolename:`content```.

There are some additional facilities that make cross-referencing roles more
versatile:

* You may supply an explicit title and reference target, like in reST direct
  hyperlinks: ``:role:`title <target>``` will refer to *target*, but the link
  text will be *title*.

* If you prefix the content with ``!``, no reference/hyperlink will be created.

* For the Chapel roles, if you prefix the content with ``~``, the link text
  will only be the last component of the target.  For example,
  ``:proc:`~Random.RandomStream.fillRandom``` will refer to
  ``Random.RandomStream.fillRandom`` but only display ``fillRandom`` as the
  link text.

  In HTML output, the link's ``title`` attribute (that is e.g. shown as a
  tool-tip on mouse-hover) will always be the full target name.

The following roles refer to objects in modules and are possibly hyperlinked if
a matching identifier is found:

``:mod:``

    Reference a module; a dotted name may be used. See `cross-references <#inline-markup-1>`_ for
    details on dotted and non-dotted names.

``:proc:``
``:iter:``

    Reference a Chapel function or iterator. The role text needs not include
    trailing parentheses to enhance readability.

    These can also be used to reference a method or iterator on an object
    (class or record instance). The role text can include the type name and the
    method, in those cases. If it occurs within the description of a type, the
    type name can be omitted.

    Dotted names may be used for any form.

``:data:``
``:const:``
``:var:``
``:param:``
``:type:``

    Reference a module-level variable, constant, compiler param, or type.

``:class:``
``:record:``

    Reference a class or record; a dotted name may be used.

``:attr:``

    Reference a data attribute (const, var, param, generic type) of an object.

For example::

    Uses :proc:`Random.RandomStream.fillRandom` and real->int casts to
    generate a vector of random integers. See :attr:`RandomStream.seed`
    and description of :mod:`Random` for details on PRNG.

    Relies on :iter:`MyModule.Set.these` to iterate over all values in the
    given :record:`MyModule.Set`.


.. _documenting-args-returns-yields:

Documenting argument, return, and yield values and types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Inside Chapel description directives, reST field lists with these fields are
recognized and formatted nicely:

* ``arg``, ``argument``: Description of a parameter.
* ``type``: Type of a parameter. Creates a link if possible.
* ``returns``, ``return``: Description of the return value.
* ``rtype``: Return type. Creates a link if possible.
* ``yields``, ``yield``: Description of the yield value, often used for
  iterators.
* ``ytype``: Yield type. Creates a link if possible.

``type``, ``rtype``, and ``ytype`` should be concise and literal type
definitions, like ``int``, ``int(64)``, ``bool``, ``[] int``, ``RandomStream``,
etc. More verbose descriptions, qualifications, and limitations of those types
should go in the corresponding ``arg``, ``returns``, or ``yields`` field.

For example, when documenting a Chapel proc:

.. code-block:: chapel

   /*
    * Calculates number of pipes and returns fooy.
    * 
    * :arg bars: Number of bars. Must be more than 1 and less than 1000.
    * :type bars: int
    * 
    * :arg hours: Hours available. Default is 1.0.
    * :type hours: real
    * 
    * :returns: Amount of fooy available.
    * :rtype: Foo
    */
   proc foo(bars, hours=1.0): Foo
   {
     ...
   }

.. note::

   * Leave an empty line above and below all these fields for best results.
   * All of these fields must be left-aligned with the outer most paragraph(s).


Paragraph level markup
~~~~~~~~~~~~~~~~~~~~~~

These directives create short paragraphs and can be used inside information
units as well as normal text:

``note``

   An especially important bit of information about an API that a user should be
   aware of when using whatever bit of API the note pertains to.  The content of
   the directive should be written in complete sentences and include all
   appropriate punctuation.

   Example::

      .. note::

         This function is not suitable for high precision calculations.

``warning``

   An important bit of information about an API that a user should be aware of
   when using whatever bit of API the warning pertains to.  The content of the
   directive should be written in complete sentences and include all appropriate
   punctuation.  In the interest of not scaring users away from pages filled
   with warnings, this directive should only be chosen over ``note`` for
   information regarding the possibility of crashes, data loss, or security
   implications.

``versionadded``

   This directive documents the version of Chapel which added the described
   feature, or a part of it, to the library or API. When this applies to an
   entire module, it should be placed at the top of the module section before
   any prose.

   The first argument must be given and is the version in question; if the
   addition is only part of the described API element, you should add a second
   argument consisting of a *brief* explanation of the change.

   Example::

      .. versionadded:: 2.1
         Multi-precision integer support added.

   Note that there must be no blank line between the directive head and the
   explanation; this is to make these blocks visually continuous in the markup.

``versionchanged``

   Similar to ``versionadded``, but describes when and what changed in the named
   feature in some way (changed side effects, platform support, etc.).  This one
   *must* have the second argument (explanation of the change).

``seealso``

   Many sections include a list of references to module documentation or
   external documents.  These lists are created using the ``seealso`` directive.

   The ``seealso`` directive is typically placed in a section just before any
   sub-sections.  For the HTML output, it is shown boxed off from the main flow
   of the text.

   The content of the ``seealso`` directive should be a reST definition list.
   Example::

      .. seealso::

         Module :mod:`Random`
            Documentation of the :mod:`Random` standard module.

         `Mersenne Twister pseudo random number generator <http://link>`_
            Documentation for the PRNG.


.. _Advanced chpldoc options:

Advanced chpldoc options
------------------------

If you would like to restrict documentation to multi-line comments starting
only with a special character sequence (say, ``/***``) use the
``--docs-comment-style`` flag to indicate the desired prefix (e.g.,
``--docs-comment-style='/***'``). Setting a comment style in this way also
establishes that the closing comment style should have the same number of
characters (though they can be different ones).


.. _How chpldoc works:

How chpldoc works
-----------------

The Chapel standard library documentation is generated by running chpldoc over
all Chapel source files in ``$CHPL_HOME/modules/standard/``.

The markup used in the comments is reSTructuredText_. reStructuredText is
developed by the docutils_ projects and is amended by custom directives to
support documenting Chapel code. Sphinx_ is used by ``chpldoc`` to render
reStructuredText as HTML.

.. _reStructuredText: http://docutils.sf.net/rst.html
.. _docutils: http://docutils.sourceforge.net/
.. _Sphinx: http://sphinx-doc.org/


.. _Future directions:

Future directions
-----------------

If there are other features you would like, please let us know. These are
currently on our backlog:

* Add visibility control (public/private) to Chapel and have chapeldoc only
  represent public elements by default (with options for including private
  elements).

* Ability to include doctests, which would be code snippets in documentation
  that can be tested. This is similar to Python's doctest feature.
