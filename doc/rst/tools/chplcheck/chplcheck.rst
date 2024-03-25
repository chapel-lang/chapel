.. _readme-chplcheck:

chplcheck
=========

``chplcheck`` is a linter for the Chapel programming language implemented in
Python using the :ref:`Python bindings for the compiler frontend <readme-chapel-py>`.
It is intended to catch stylistic mistakes and bad practices in Chapel programs.
It is also intended to be customizable and extensible, using a system of named
'rules' that lead to warnings.

``chplcheck`` supports the Language Server Protocol, allowing it to be used as
part of your favorite editor. The following image demonstrates its' use in
Neovim:

.. image:: neovim.png
  :alt: Screenshot of code using ``chplcheck``

Getting Started
---------------

The easiest way to make ``chplcheck`` available on your command line is by using the
``chplcheck`` Makefile target. This will build the Dyno compiler frontend and the
Python bindings for Dyno if needed, and place ``chplcheck`` into ``$CHPL_HOME/bin``.
Make sure that you satisfy :ref:`the requirements for building the Python bindings <chapel-py-installation>`.

.. code-block:: bash

   cd $CHPL_HOME
   make chplcheck
   chplcheck --help

Saving the following file into ``myfile.chpl``:

.. code-block:: chapel
   :linenos:

   record MyRecord {}

   for i in 1..10 do {
     writeln("Hello, world!");
   }

The linter is run as follows:

.. code-block:: bash

   > chplcheck myfile.chpl
   path/to/myfile/myfile.chpl:1: node violates rule CamelCaseRecords
   path/to/myfile/myfile.chpl:3: node violates rule DoKeywordAndBlock
   path/to/myfile/myfile.chpl:3: node violates rule UnusedLoopIndex


Enabling / Disabling Rules
--------------------------

Each rule, such as ``CamelCaseRecords``, can be individually enabled or
disabled from the command line using ``--enable-rule`` and ``--disable-rule``.
To silence the warning about unused loop indices such as ``i`` in the above
code, we can invoke ``chplcheck`` as follows:

.. code-block:: bash

   > chplcheck myfile.chpl --disable-rule UnusedLoopIndex
   path/to/myfile/myfile.chpl:1: node violates rule CamelCaseRecords
   path/to/myfile/myfile.chpl:3: node violates rule DoKeywordAndBlock


Some rules are disabled by default. One such rule is ``UseExplicitModules``, which
warns against letting Chapel automatically create the top-level module in a file.

.. code-block:: bash

   > chplcheck myfile.chpl --enable-rule UseExplicitModules
   path/to/myfile/myfile.chpl:1: node violates rule CamelCaseRecords
   path/to/myfile/myfile.chpl:1: node violates rule UseExplicitModules
   path/to/myfile/myfile.chpl:3: node violates rule DoKeywordAndBlock
   path/to/myfile/myfile.chpl:3: node violates rule UnusedLoopIndex


All of the rules present in ``chplcheck`` right now are defined in
``tools/chplcheck/src/rules.py``. You may reference this file to get a complete
list of available rules. Rules marked with ``default=False`` are not enabled by
default, and need to be turned on using the ``--enable-rule`` flag; rules
without the above setting are enabled by default, and can be turned off using
``--disable-rule``.

Rules can also be ignored on a case-by-case basis by adding a ``@chplcheck.ignore`` attribute with a string argument stating the rule to ignore. For example:

.. code-block:: chapel

   @chplcheck.ignore("CamelCaseRecords")
   record MyRecord {}

This will suppress the warning about ``MyRecord`` not being in camelCase.

.. note::

   ``chplcheck.ignore`` is not fully implemented yet. It is currently only
   available for basic rules and usage of it may cause compile-time warnings.

Setting Up In Your Editor
-------------------------

``chplcheck`` uses the Language Server Protocol (LSP) to integrate with compatible
clients. Thus, if your editor supports LSP, you can configure it to display
linting warnings via ``chplcheck``. The following sections describe how to set
up ``chplcheck`` in various editors, and will be updated as the Chapel team
tests more editors. If your preferred editor is not listed, consider opening an
`issue <https://github.com/chapel-lang/chapel/issues/new>`_ or `pull request
<https://github.com/chapel-lang/chapel/pull/new>`_ to add it.

Neovim
~~~~~~

The built-in LSP API can be used to configure ``chplcheck`` as follows:

.. code-block:: lua

   local lspconfig = require 'lspconfig'
   local configs = require 'lspconfig.configs'
   local util = require 'lspconfig.util'

   configs.chplcheck = {
     default_config = {
       cmd = {"chplcheck", "--lsp"},
       filetypes = {'chpl'},
       autostart = true,
       single_file_support = true,
       root_dir = util.find_git_ancestor,
       settings = {},
     },
   }

   lspconfig.chplcheck.setup{}
   vim.cmd("autocmd BufRead,BufNewFile *.chpl set filetype=chpl")

VSCode
~~~~~~

Install the ``chapel`` extension from the `Visual Studio Code marketplace
<https://marketplace.visualstudio.com/items?itemName=chpl-hpe.chapel-vscode>`_.

.. note::

   The extension is not yet available at the time of writing and the above link
   may not work until then. This section will be updated when it is available.

Writing New Rules
-----------------

Rules are written using the :ref:`Python bindings for Chapel's compiler frontend<readme-chapel-py>`. In
essence, a rule is a Python function that is used to detect issues with the
AST. When registered with ``chplcheck``, the name of the function becomes the name
of the rule (which can be used to enable and disable the rule, as per the
above sections). To mark a Python function as representing a rule,
``chplcheck``'s Python API provides two decorators. These decorators correspond
to the two 'flavors' of rules in the linter: 'basic' and 'advanced'.

Basic Rules
~~~~~~~~~~~

Basic rules are specified using a :ref:`pattern <chapel-py-pattern-matching>`.
This pattern represents which AST nodes should be scrutinized to check if something.
The ``driver.basic_rule`` decorator is used to specify such rules. For instance,
the following basic rule checks that explicit modules have ``PascalCase`` naming:

.. code-block:: python

   @driver.basic_rule(Module)
   def PascalCaseModules(context, node):
       return node.kind() == "implicit" or check_pascal_case(node)

The ``Module`` argument to ``basic_rule`` specifies that the linter should call
the ``PascalCaseModules`` function with each ``Module`` node it encounters. If
the function returns ``True``, no warning should be emitted. If the function
returns ``False``, the linter should produce a warning. The conditional returns
``True`` for all implicit modules, regardless of their name: this is because
implicit modules are named after the file they are in, so the user cannot "fix"
the code by editing it. For explicit modules, a helper function
``check_pascal_case`` is used to ensure that the node's name is appropriately
cased.

Patterns can be more advanced than simply specifying an AST node type. The
following rule makes more use of patterns by specifying that it should be
applied only to ``if``-statements that just have a boolean literal as their
condition.

.. code-block:: python

   @driver.basic_rule([Conditional, BoolLiteral, chapel.rest])
   def BoolLitInCondStmt(context, node):
       return False

Advanced Rules
~~~~~~~~~~~~~~

Sometimes, specifying a pattern is not precise enough to implement a rule. For
example, a linting check might require considering two sibling nodes or other
less-straightforward relationships than "does it match the pattern?". This is
the purpose of advanced rules. These functions are called with the *root* AST
node (usually a top-level ``Module``). Then, it is the responsibility
of the function to find and ``yield`` AST nodes that should be warned about.
For instance, at the time of writing, the following code implements the rule
checking for unused formals.

.. code-block:: python

   @driver.advanced_rule(default=False)
   def UnusedFormal(context, root):
       formals = dict()
       uses = set()

       for (formal, _) in chapel.each_matching(root, Formal):
           # For now, it's harder to tell if we're ignoring 'this' formals
           # (what about method calls with implicit receiver?). So skip
           # 'this' formals.
           if formal.name() == "this":
               continue

           # extern functions have no bodies that can use their formals.
           if formal.parent().linkage() == "extern":
               continue

           formals[formal.unique_id()] = formal

       for (use, _) in chapel.each_matching(root, Identifier):
           refersto = use.to_node()
           if refersto:
               uses.add(refersto.unique_id())

       for unused in formals.keys() - uses:
           yield formals[unused]


This function performs _two_ pattern-based searches: one for formals, and one
for identifiers that might reference the formals. It then emits a warning for
each formal for which there wasn't a corresponding identifier.

Adding Custom Rules
-------------------

Developers may have their own preferences for their code they would like to be
enforced by a linter. Rather than adding their own rule to ``rules.py``,
developers can load a custom rule file that contains all of their custom rules.

For example, the following code is a complete definition of two new rules for
``chplcheck``. Note that the top-level function must be named ``rules`` and take
one argument.

.. code-block:: python

   # saved in file `myrules.py`
   import chapel

   def rules(driver):

     @driver.basic_rule(chapel.Function)
     def NoFunctionFoo(context, node):
       return node.name() != "foo"

     @driver.basic_rule(chapel.Variable, default=False)
     def NoVariableBar(context, node):
       return node.name() != "bar"

To use these rules with ``chplcheck``, use the ``--add-rules`` command line
argument.

Saving the following file into ``myfile.chpl``:

.. code-block:: chapel
   :linenos:

   proc foo() {
     var bar = 10;
   }

The linter is run as follows:

.. code-block:: bash

   > chplcheck myfile.chpl --add-rules path/to/my/myrules.py --enable-rule NoVariableBar
   path/to/myfile/myfile.chpl:1: node violates rule NoFunctionFoo
   path/to/myfile/myfile.chpl:2: node violates rule NoVariableBar

