:title: Users Guide: Configuration Declarations

.. _ug-typeAliases:

**config** Declarations: command-line overrides
===============================================

The four main declaration keywords that have been introduced so far —
``var``, ``const``, ``param``, and ``type`` — each support a mode in
which their symbols' initial values as specified in the source code
can be overridden on the command-line.  This supports the ability to
specify and override default values, and is achieved by prefixing the
statement with the keyword ``config``.

``config var`` and ``config const``
-----------------------------------

Configuration variables are variables whose default values can be
overridden the executable's command-line.  Here's a simple example:

.. literalinclude:: examples/users-guide/base/configs.chpl
  :caption:
  :language: chapel
  :lines: 1

Like a traditional ``var`` declaration, this creates a variable named
*n* of type ``int``, initialized to the value 10.  However, the
``config`` prefix causes the compiler to add a command-line flag to
the generated executable that supports overriding that default value.
For example, if we run the program as follows:

.. literalinclude:: examples/users-guide/base/configs.execopts
  :language: text
  :lines: 1

then *n* will be initialized to the value 1000 for that execution of
the program.  The arguments for these flags can also be specified
using ``=`` (with no spaces) as the separator between the argument
name and its value, as follows:

.. literalinclude:: examples/users-guide/base/configs.execopts
  :language: text
  :lines: 2

There is no difference between these two forms—it's simply a matter of
taste.

As with normal variables, a variable declared as a ``config var`` can
be re-assigned throughout its lifetime.  In contrast, ``config const``
declarations cannot be changed once they are initialized, whether that
initial value comes from the source code expression or the
command-line.  The following statement declares a pair of configurable
constants:

.. literalinclude:: examples/users-guide/base/configs.chpl
  :language: chapel
  :lines: 2-3

As with the previous case, we can override their default values on the
executable's command-line.  For instance the following switches to a
smaller value of *epsilon* and flips *debug* to ``true``:

.. literalinclude:: examples/users-guide/base/configs.execopts-fake
  :language: text
  :lines: 3

Boolean ``config`` declarations, like *debug* above, also support a
mode in which they can be set to ``true`` simply by using the flag
without a supporting value.  Thus, the previous command-line could
also have been written:

.. literalinclude:: examples/users-guide/base/configs.execopts-fake
  :language: text
  :lines: 4


``config param`` and ``config type``
------------------------------------

Chapel also supports configurable ``param`` and ``type`` declarations
via command-line flags.  However, since ``param`` values and types
must be known to the compiler, the default values are overridden using
flags on the compiler rather than the generated executable.

As an example, the following statements declare a series of
configurable ``param`` and ``type`` symbols:

.. literalinclude:: examples/users-guide/base/configs2.chpl
  :language: chapel
  :lines: 3-7

When overriding ``config`` declarations in the compiler the ``-s``
flag is used to set the new value.  As an example, to set **verbose**
to ``true``, we could compile as follows:

.. literalinclude:: examples/users-guide/base/configs2.compopts-fake
  :language: text
  :lines: 1

As with the generated executable, boolean values can also be set on
the compiler's command line simply using the name of the flag.  Thus,
the following would be equivalent to the previous line:

.. literalinclude:: examples/users-guide/base/configs2.compopts-fake
  :language: text
  :lines: 2

Needless to say, multiple ``config`` declarations can be overridden
at once.  For example, this command-line overrides the default
initializers for *bitMask*, *size*, and *age*:

.. literalinclude:: examples/users-guide/base/configs2.compopts-fake
  :language: text
  :lines: 3

One of the nice things about setting ``config`` declarations in the
compiler is that the specified value can take the form of reasonably
arbitrary Chapel code since the compiler is involved in interpreting
it.  Thus, we can use more interesting expressions when setting
compile-time ``config`` values.  For example, the following compiler
lines specify *bitMask*, *size*, and *age* not using simple literals
and types, but with complex expressions:

.. literalinclude:: examples/users-guide/base/configs2.compopts-fake
  :language: text
  :lines: 5-6

Note the use of quotes in this example to preserve whitespace in the
initial value.  This relates to having the command shell pass the
entire expression to Chapel as a single argument rather than being
a Chapel-specific syntax.

Setting ``config`` defaults on the compiler's command-line also
supports more radical alterations to the code.  For example, we
could change *age* from an integral type to a floating point type:

.. literalinclude:: examples/users-guide/base/configs2.compopts-fake
  :language: text
  :lines: 4

Note that ``config`` declarations are processed in the order that they
appear in the source code and that any initializing expressions must
make sense when considered in that original context.  Thus, the
command-line overrides for a given ``config`` can refer to other
declarations that precede that declaration in the source code.  For
instance, we can set *age* in terms of the ``config param`` *size*
since it appeared earlier in the code:

.. literalinclude:: examples/users-guide/base/configs2.compopts-fake
  :language: text
  :lines: 7

However, the reverse would not be possible since *age* had not been
established when *size* is declared.

Similarly, subsequent declarations in the source code can themselves
be based on ``config`` declarations.  For instance, here are some
declarations that are based on the ``config param`` and ``config
type`` statements shown above and could follow them:

.. literalinclude:: examples/users-guide/base/configs2.chpl
  :language: chapel
  :lines: 9-11


Compile time overrides for ``config var`` and ``config const``
--------------------------------------------------------------

Users can also specify default expressions for ``config var`` and
``config const`` declarations on the compiler's command-line.  As with
other compile-time overrides, complex or symbolic expressions can be
used since the compiler is involved.  As an example, let's look at
a variation on one of our earlier declarations:

.. literalinclude:: examples/users-guide/base/configs3.chpl
  :language: chapel
  :lines: 1-2

The followng compiler command can be used to override the source
code initializer for *epsilon* so that it will be a function of
*n* by default:

.. literalinclude:: examples/users-guide/base/configs3.compopts
  :language: text

Such an initializer could not be specified on the executable
command-line since the compiler is out of the picture by then.  At
that point, the values for a ``config var`` or ``config const`` are
restricted to the set of strings that can legally be cast to that
type.

Having compiled *epsilon* to be set in this way, if we specify
a value for *n* on the executable's command line:

.. literalinclude:: examples/users-guide/base/configs3.execopts-fake
  :language: text
  :lines: 2

then *epsilon* will be computed in terms of it, resulting in the value:

.. literalinclude:: examples/users-guide/base/configs3-n1000.good
  :language: text
  :lines: 2

Note that the compiler's command-line override can still be
re-overridden on the executable's command-line as usual:

.. literalinclude:: examples/users-guide/base/configs2.compopts-fake
  :language: text
  :lines: 4


Scope of ``config`` declarations
--------------------------------

Because all ``config`` declarations are processed at program startup
time, they must appear at *module scope*.  We haven't talked about
modules yet, so for now, think of them as needing to appear as
top-level statements in a file.

