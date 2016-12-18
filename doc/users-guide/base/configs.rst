:title: Users Guide: Configuration Declarations

.. _ug-typeAliases:

**config** Declarations: command-line overrides
===============================================

The four main declaration keywords that we've covered so far —
``var``, ``const``, ``param``, and ``type`` — each support a mode in
which their symbols' initial values can be easily overridden on the
command-line.  This is achieved by prefixing the statement with the
keyword ``config``.

``config var`` and ``config const``
-----------------------------------

We'll start with configuration variables, which are typically set on
the executable's command-line.  Here's a simple example:

.. literalinclude:: examples/users-guide/base/configs.chpl
  :caption:
  :language: chapel
  :lines: 1

Like a traditional ``var`` declaration, this creates a variable named
*n* of type ``int``, initialized to the value 10.  However, the
``config`` prefix causes the compiler to generate support for a
command-line flag with the variable's name, permitting its value to be
overridden on the generated executable's command-line.  For example,
if we run the program as follows:

.. literalinclude:: examples/users-guide/base/configs.execopts
  :language: text
  :lines: 1

then *n* will be initialized to the value 1000 for that execution of
the program.  The arguments for these flags can also be specified
using an ``=`` as the separator between the argument name and its
value, as follows:

.. literalinclude:: examples/users-guide/base/configs.execopts
  :language: text
  :lines: 2

There is no difference between these two forms—it's simply a matter of
taste.

As with normal variables, a variable declared as a ``config var`` can
be re-assigned throughout its lifetime.  In contrast, ``config const``
declarations cannot be changed once they are initialized, whether that
initial value comes from the source code's default or the
command-line.  The following statement declares a pair of configurable
constants:

.. literalinclude:: examples/users-guide/base/configs.chpl
  :language: chapel
  :lines: 2-3

As with the previous case, we can override their default values on
the executable's command-line.  For instance:

.. literalinclude:: examples/users-guide/base/configs.execopts
  :language: text
  :lines: 3

Boolean ``config`` declarations, like *verbose* above, also support a
mode in which they can be set to ``true`` simply by using the flag
without a supporting value.  Thus, the previous command-line could
also have been written:

.. literalinclude:: examples/users-guide/base/configs.execopts
  :language: text
  :lines: 4


``config param``
----------------

``param`` declarations also support a configurable command-line form.
However, since ``param`` values must be known at compile-time, the
source code value is overridden using command-line flags on the
compiler rather than the generated executable.



TODO
----
* ability to specify vars and consts on compiler line too using symbolic exprs
* ability to still re-override on executable
* must be file-level module-level
