.. _readme-chapel-py:

chapel-py
=========

Python bindings for Chapel's frontend library, Dyno.

The purpose of this library is to allow programmers to easily create tools
for the Chapel programming language that would be more difficult to write
using C++ (the native language in which the Chapel frontend library is written).

For example, here's a toy program that prints the names of all things declared
anywhere in ``myfile.chpl``.

.. code-block:: python

   from chapel import *

   ctx = Context()
   ast = ctx.parse("myfile.chpl")

   all_names = set()
   for node in postorder(ast):
       if isinstance(node, NamedDecl):
           all_names.add(node.name())

   for name in all_names:
       print(name)


If ``myfile.chpl`` contains the following:

.. code-block:: chapel

   module myfile {
       var x = 1+1;
       var y = x+1;

       record R {
 
       }
   }


Then the program will print out something like the following:

.. code-block::

   R
   y
   x
   myfile


.. note::

  The Chapel Python bindings are not yet fully mature. The API is subject to
  change and is not considered stable. Users of this library should be prepared
  to update their code as the library evolves.


.. _chapel-py-installation:

Installation
------------

Make sure that you have a from-source build of Chapel available in your
``CHPL_HOME``. Currently, the build script also requires having LLVM available
in your path. The build script also requires that the development package of
Python be installed (for many package managers this is called
``python3-devel``). With those constraints met, you can just run ``make
chapel-py-venv``:

This will allow you to use the Python bindings from a Python script run with
``$CHPL_HOME/util/config/run-in-venv-with-python-bindings.bash python3``,
instead of just ``python3``. If you wish to install the Python bindings in your
system Python, run ``python3 -m pip install $CHPL_HOME/tools/chapel-py``.

Usage
-----

The library is split into several major components:

* The ``chapel`` module provides the AST node class hierarchy and the ``Context``
  object. It also provides some higher-level, pure Python implementations of
  certain utility functions. For instance, it provides the ``preorder`` and
  ``postorder`` traversal iterators and a ``match`` function to perform AST
  :ref:`pattern matching <chapel-py-pattern-matching>`.
* The ``chapel.replace`` module provides the "replacer API", which is intended
  to perform transformations on existing Chapel files for various reasons. For
  instance, deprecations and syntax changes can be performed automatically using
  the replacer API, by finding AST patterns and performing string substitution.
* The ``chapel.lsp`` module provides a few helpers to transform Dyno types to
  LSP (Language Server Protocol) types. This module requires the `lsprotocol
  <https://pypi.org/project/lsprotocol/>`_ package.

``chapel``
~~~~~~~~~~

The main entry point to the Chapel Python API is the ``Context`` object. This
is a wrapper around the C++ construct of the same name. The Context in Dyno
is responsible for memoizing computations, interning strings, and more. The
Python wrapper around this object has methods that can be used to get started
with processing Chapel programs; one such method is ``parse``, which accepts
a file path and returns a list of Chapel AST nodes that represent this file.

At compile-time, ``chapel-py`` generates a class hierarchy of Python objects,
with base class ``AstNode``. A class is created for each of the AST nodes understood
by Dyno. Thus, one might write:

.. code-block:: python

   from chapel import *

   ctx = Context()
   asts = ctx.parse("myfile.chpl")
   print("it's a module" if isinstance(asts[0], Module) else "it's not a module")

Here, the Python ``isinstance`` builtin is used to determine if the first
AST node returned by ``parse`` is a module (it should be!)

Because the AST nodes form a class hierarchy, it's possible to perform a more
general check; the following example prints "It's a loop!" for all loops
(``do``- ``while``, ``for``, ``foreach``, etc.), and "It's not a loop..." otherwise.

.. code-block:: python

   def check(node):
       if isinstance(node, Loop):
           print("It's a loop!")
       else:
           print("It's not a loop...")

The AST nodes from the Python API are iterable; iterating over a node yields
its children. Thus, one might print all the declarations in a module as follows:

.. code-block:: python

   def print_decls(mod):
       for child in mod:
           if isinstance(child, NamedDecl):
               print(child.name())

Full AST traversals can be built on top of this functionality. Below
is the definition for ``postorder`` from the ``chapel`` module:

.. code-block:: python

   def postorder(node):
       """
       Recursively visit the given AST node, going in post-order (children-then-parent)
       """
       for child in node:
           yield from postorder(child)
       yield node

To find the list of available methods for each AST node class, the current best
practice is to consult the generated
``$CHPL_HOME/tools/chapel-py/src/chapel/core/__init__.pyi`` file. This is
generated by default when the Python bindings are built and is the most
up-to-date source of information on the API.


The ``chapel`` module also provides convenience functions for working with the library.
For instance, it provides the ``postorder`` and ``preorder`` iterators (the
implementation of the former is included above). It also provides a couple
of more advanced helpers for dealing with Chapel ASTs.

``chapel.parse_attribute``
^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``parse_attribute`` function, given a "description" of an attribute (its
name and formal list), tries to parse an ``Attribute`` AST node. This function
accounts for named and unnamed actuals, reordering, etc. For instance, given
an "attribute schema":

.. code-block:: python

   attr = ("doXYZ", ["x", "y", "z"])


And the following Chapel attribute:

.. code-block:: chapel

   @doXYZ("hello", z = "world", y = "!")

The function returns:

.. code-block:: python

   {"x": "hello", "y": "!", "z": "world"}


This function is used in the :ref:`Chapel linter <readme-chplcheck>` to handle
silencing warnings:

.. code-block:: python

   def ignores_rule(node, rulename):
       ag = node.attribute_group()

       if ag is None: return False
       for attr in ag:
           attr_call = chapel.parse_attribute(attr, IgnoreAttr)
           if attr_call is None: continue

           ignored_rule = attr_call["rule"]
           if ignored_rule is not None and ignored_rule.value() == rulename:
               return True

       return False

.. _chapel-py-pattern-matching:

``chapel.match_pattern``
^^^^^^^^^^^^^^^^^^^^^^^^

This function provides general pattern matching functionality to enable users
of the Python API to rapidly find "interesting" locations in the AST. It
supports arbitrary levels of nesting, as well as "named variables" to easily
retrieve deeply nested AST nodes after a pattern has matched.

Generally, a call to ``match_pattern`` takes an AST node and a pattern (what
constitutes a pattern is explained below). If the pattern matches, the function
returns a Python ``dict`` that maps pattern variables to their corresponding
AST nodes; an empty ``dict`` is returned if no pattern variables were used. If
the pattern does not match, the function returns ``None``.

To explain the possible patterns, it's helpful to have an example AST.
We will use the following Chapel function, and its AST ``myFn``, in the following
examples:

.. code-block:: chapel

   proc f(x: int, y: int) {
       var z = 1+2;
       return x+y+z;
   }

The simplest example of pattern is an AST class. For instance:

.. code-block:: python

   print(chapel.match_pattern(myFn, Function)) # Prints "{}"

When a function AST node ``myFn`` is matched against the ``Function`` pattern,
the match is successful; since no pattern variables were used, this results
in an empty dictionary ``{}``.

A list can be used to represent patterns of AST nodes with children. To match a
function with exactly two arguments:

.. code-block:: python

   print(chapel.match_pattern(myFn, [Function, Formal, Formal, Block])) # Prints "{}"


Since we still haven't used any pattern variables, the result of
``match_pattern`` still returns ``{}``. However, having matched the pattern, it
would be cumbersome to attempt to retrieve each of the two formals from the
list: we'd need to access the function's children again, by their index. Since
``Formal`` nodes are direct children of the ``Function``, this isn't too
inconvenient; however, conceivably, the nodes of interest could be further down
in the AST from the parent. This is where pattern variables come in. Instead of
writing ``Formal``, we can write ``("?f1", Formal)``. This will cause the
pattern matcher to save the AST node matching ``Formal`` into the variable
``f1``.

.. code-block:: python

   pat = [Function, ("?f1", Formal), ("?f2", Formal), Block]
   print(chapel.match_pattern(myFn, pat)) # Prints "{ 'f1': ..., 'f2': ... }"

The dictionary returned by ``match_pattern`` can then be queried by the variables
to retrieve the formals.

We can further restrict our pattern to require formals with 'simple' type
expressions (identifiers).

.. code-block:: python

   pat = [Function, [Formal, Identifier], [Formal, Identifier], Block]
   print(chapel.match_pattern(myFn, pat)) # Prints "{}"

Pattern variables can still be used with list patterns like ``[Formal,
Identifier]``. This is done by adding the pattern variable string to the front
of the list.

.. code-block:: python

   pat = [Function, ["?f1", Formal, Identifier], ["f2", Formal, Identifier], Block]
   print(chapel.match_pattern(myFn, pat)) # Prints "{ 'f1': ..., 'f2': ... }"

Some other useful features of the pattern library:

* The ``rest`` pattern can be used as part of a list to indicate that you don't
  care about the remaining AST children, however many there are.
* A variable without an AST node, (``"?x"``) can be used by itself to match any
  AST node and store it in ``x``.
* A ``set`` can be used to represent a disjunctive or pattern. For example,
  ``set([Begin, Cobegin])`` will match either a ``Begin`` or a ``Cobegin`` node.

``chapel.each_matching``
^^^^^^^^^^^^^^^^^^^^^^^^

This function combines the operation of ``preorder`` and ``match_pattern`` to iterate
an AST and yield all nodes matching the given pattern. Concretely, given
a pattern, it yields the node and the variables resulting from the match.
The following snippet will print one line for each binary operation, listing
the operation itself and the operation's two operands.

.. code-block:: python

   for (op, variables) in chapel.each_matching(myFn, [OpCall, "?l", "?r"]):
       print("Found an operation ", op.op(), "with operands: ", variables["l"], variables["r"])

``chapel.replace``
~~~~~~~~~~~~~~~~~~

The ``chapel.replace`` module is used for writing "replacer scripts". The motivation
for this module is evolving the Chapel language. As the language develops, we
tend to shift the patterns we prefer, and modify unstable language syntax. When
modifying existing code to match the new standards, we either have to perform
the changes manually, or resort to "general" tools like ``sed`` or ``awk``. However,
the trouble with these tools is that they are suited for modifying streams of
text. When modifying programs in a language, it is more natural to work with
a structured representation -- the AST. Thus, the ``chapel.replacer`` module
provides utilities to modify a file by traversing the ASTs contained within,
and generating a list of substitutions.

Although the AST is used to find places in the code where changes must be made,
the changes themselves are performed using string operation. This is motivated
by many reasons:

* Dyno does not have good support for modifying ASTs (they are immutable).
* If an AST is modified, it should be printed back to the file with only those
  modifications; however, this would require the Chapel syntax printer to
  be able to preserve most of the original format of the text.
* Writing plain code, such as ``1+1``, is often easier than creating an AST node
  (``PlusNode(IntNode(1), IntNode(1))``).

The ``chapel.replace`` module provides a driver function ``run``, which, when given
a source of changes (described below), takes over the execution of the program.
It registers and parses command line arguments, and handles file modifications.
Thus, given an AST traversal, you end up with a fully-featured command-line
script ready to be pointed at files.

.. code-block::

   usage: replace [-h] [--suffix SUFFIX] [--in-place] [filenames ...]

   A tool to search-and-replace Chapel expressions with others
 
   positional arguments:
     filenames
 
   options:
     -h, --help       show this help message and exit
     --suffix SUFFIX
     --in-place`


The 'source of changes' is a Python iterator that should accept two arguments:
``rc`` (for 'replacement context') and ``root`` (for the AST node at which the
traversal begins). A very simple (albeit completely pointless and semantically
incorrect) example is the following script, which replaces all occurrences of
the number ``42`` with ``meaningOfLife``:

.. code-block:: python

   def replace_all_42(rc, root):
       for (num, _) in each_matching(root, IntLiteral):
           if num.text() == '42': yield (num, 'meaningOfLife')

   run(replace_all_42)

A more practical example is renaming methods. Note that this is not
amenable to naive substitution: we don't want to accidentally rename non-methods
that happen to have the same name, or even locally declared functions inside
other functions in a record or class. Consider renaming the ``enterThis``
method to ``enterContext`` (this is a real change that was performed in
the Chapel language in 1.32, though it was not done automatically with the
help of this tool). In the following snippet, only the ``proc`` labeled ``(1)``
should be renamed. This is because ``enterThis`` is actually a method. On
the other hand, ``(2)``, even though it's declared within ``R``'s curly braces,
is actually a locally defined function, and not a method on ``R``.
``(3)`` is a freestanding function, and thus should also not be renamed.

.. code-block:: chapel

   record R {
       proc enterThis() {} // (1)
       proc f() {
           proc enterThis() {} // (2)
       }
   }
   proc enterThis() {} // (3)

This can be implemented using the following iterator:

.. code-block:: python

   def replace_enter_this(rc, root):
       for (fn, _) in each_matching(root, Function):
           if fn.name() == "enterThis" and fn.is_method():
               yield (fn, lambda txt: txt.replace("enterThis", "enterContext", 1))

In this case, instead of yielding a pair of the node-to-replace and the
replacement text, the iterator produces a ``lambda``, which will be fed the
node's current string representation (``proc enterThis() {...``). Using
this lambda, we perform a simple substitution, renaming ``enterThis`` to
``enterContext`` with Python's regular ``replace`` method.

Finally, the following script was used to automatically insert interfaces into
records and classes that provided the corresponding functionality. It was
used to update about 150 files (including Chapel modules and tests). This
script implements the following features:

* Detecting particular signatures of the ``init`` method specific to deserialization
* Always printing the supported interfaces in a specific order
* Supporting both primary and secondary methods, including a mix of both
* Adding a new interface list or modifying an existing one

.. code-block:: python

   def tag_aggregates_with_io_interfaces(rc, root):
       aggrs_to_change = defaultdict(lambda: set())
       names_to_tag = defaultdict(lambda: set())

       for (fn, _) in chapel.each_matching(root, Function):
           if not fn.is_method(): continue
           name = fn.name()
   
           if name == "serialize":
               tag = "writeSerializable"
           elif name == "deserialize":
               tag = "readDeserializable"
           elif name == "init":
               formal_names = []
               for child in fn:
                   if not isinstance(child, Formal): continue
                   if child.name() == "this": continue
                   formal_names.append(child.name())
   
               if len(formal_names) >=2 and formal_names[-1] == "deserializer" and formal_names[-2] == "reader":
                   tag = "initDeserializable"
               else:
                   continue
           else:
               continue
   
           if fn.is_primary_method():
               aggrs_to_change[fn.parent().unique_id()].add(tag)
               continue
   
           this_receiver = fn.this_formal()
           names_to_tag[rc.node_exact_string(this_receiver)].add(tag)
   
       def build_tag_str(tags):
           if len(tags) == 3: return "serializable"
   
           # tags have a preferred order, so just use an if-else chain to make that work
           the_order = ["writeSerializable", "readDeserializable", "initDeserializable"]
           return ", ".join(t for t in the_order if t in tags)
   
       for (record, _) in chapel.each_matching(root, AggregateDecl):
           tags = set()
           if record.unique_id() in aggrs_to_change:
               tags |= aggrs_to_change[record.unique_id()]
           if record.name() in names_to_tag:
               tags |= names_to_tag[record.name()]
   
           if len(tags) == 0: continue
   
           tag_str = build_tag_str(tags)
           record_text = rc.node_exact_string(record)
           curlypos = record_text.find("{")
           colonpos = record_text.find(":")
   
           if colonpos >= 0 and colonpos < curlypos:
               new_text = record_text.replace(" {" , ", " + tag_str + " {" , 1)
           else:
               new_text = record_text.replace(record.name(), record.name() + " : " + tag_str, 1)

           yield (record, new_text)


.. include:: ./chapel-py-api.rst

