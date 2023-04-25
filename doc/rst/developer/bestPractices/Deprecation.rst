.. _best-practices-deprecation:

Chapel's Deprecation Policy
===========================

When removing a feature in Chapel, the feature is to be deprecated and
supported for at least 1 release cycle.

Major feature removals can be supported for more than 1 release cycle.

Non-public interface features can be removed or modified without deprecation.

This is the deprecation process:

1. Provide a warning or error message when a deprecated feature is used. This
   may be in the module code, compiler, or runtime, and can be emitted at
   compile-time or runtime.

The deprecation message uses the following format:

``foo is deprecated - please use bar``

For compile-time messaging you can use ``compilerWarning()`` to do this, but
when deprecating symbols the ``@deprecated`` attribute often works better because
it can be applied to any symbol in a uniform way.  Look in the test/deprecated
and test/deprecated-keyword directories for many examples.

Currently, the ``@deprecated`` attribute optionally takes a string literal,
which is used for the deprecated symbol's documentation and compiler warning
message. If there is also a documentation comment applied to the symbol that
comment will take precedence when producing documentation.

When we produce a compiler warning we filter it to remove any inline markup.
This means if your deprecation message points to the preferred feature using
syntax like this: ``foo is deprecated - please use :proc:`bar```, then in the
generated documentation "bar" will link to the documentation for the "bar"
procedure, and when producing the compiler warning the ":proc:" syntax and
backticks will be filtered out.

2. Update the deprecated feature documentation to state that it is deprecated
   and point to the preferred feature.

For example:

.. code-block:: rst

    Foo module short description.

    .. warning::

        This module has been deprecated - please use :mod:`bar` instead.

    Foo module long description.

.. note::

   This step is not necessary when using the ``@deprecated`` attribute to
   generate the documentation.

3. Search for error messages that refer to the deprecated feature.
   If appropriate, update them to refer to the new feature.

4. Add a deprecation test to ``test/deprecated/`` to:

  - confirm the deprecation warning works as intended
  - remind us to to remove the deprecated feature in the following release

Optionally include deprecation timeline in comments of deprecation test:

.. code-block:: chapel

    // Deprecated in Chapel 1.14, to be removed in Chapel 1.15

Examples
--------

The following section contains some examples of how to deprecate symbols in
unusual situations.

Changing A Function's Return Type
+++++++++++++++++++++++++++++++++

When nothing about a function except its return type needs to change, it can
be tricky for a user to opt in to the new behavior.

.. code-block:: chapel

   @deprecated("foo returning 'int' is deprecated")
   proc foo(): int { ... }

   // As written, this replacement would conflict with the original, how can a
   // user call it?
   proc foo(): bool { ... }

In this situation, we recommend adding a ``config param`` and a ``where`` clause
that responds to it to the deprecated function and its replacement:

.. code-block:: chapel

   // The default state should result in the deprecated behavior, so users can
   // adjust their code at their leisure
   config param fooReturnsBool = false;

   // The deprecation message should alert the user to the new flag
   @deprecated("foo returning 'int' is deprecated, please compile with '-sfooReturnsBool=true' to get the new return type")
   proc foo(): int where !fooReturnsBool { ... }

   // The new function should also use a 'where' clause to opt in to the new
   // behavior
   proc foo(): bool where fooReturnsBool { ... }

When the deprecated function is removed, the flag should also be deprecated (and
removed from the new function to avoid generating noise for the user):

.. code-block:: chapel

   @deprecated("'fooReturnsBool' is deprecated and no longer has an effect")
   config param fooReturnsBool = false;

   // The old version has been removed, and the flag is no longer needed, so
   // the new function can be in its final state.
   proc foo(): bool { ... }

There is a drawback with this approach - config params are not adjustable during
the lifetime of a program, so updates must be made all at once before they can
be tested by the user.  So, if you can think of a better solution, please
suggest it and we will update this example!

Changing A Function's Argument Name
+++++++++++++++++++++++++++++++++++

When only the name of a function argument needs to change and not its type, a
new overload will encounter conflicts when a user relies solely on positional
ordering:

.. code-block:: chapel

   @deprecated("argument name 'a' is deprecated, use 'b' instead")
   proc foo(a: int) { ... }

   proc foo(b: int) { ... }

   ...

   // This will obviously use the deprecated version
   foo(a=3);
   // And this will obviously use the new version
   foo(b=3);
   // But which overload will this call?  The compiler can't choose between them
   foo(3);

In this case, we still want to generate warnings when the old argument name is
used, but we want positional ordering to work without indicating anything has
changed.  To accomplish this, mark the deprecated version with ``pragma "last
resort"`` - this will avoid conflicts in the positional ordering case while
still keeping the old argument name available to generate the deprecation
warning:

.. code-block:: chapel

   pragma "last resort"
   @deprecated("argument name 'a' is deprecated, use 'b' instead")
   proc foo(a: int) {
     foo(a); // Call function with new argument name
   }

   proc foo(b: int) { ... }

   ...

   // The behavior of these two calls is unchanged
   foo(a=3);
   foo(b=3);
   // Now this will call the new version without conflict
   foo(3);

Replacing A Field
+++++++++++++++++

While the obvious strategy for replacing a field would be to add an additional
one with the new name, following this strategy can lead to a number of problems.
Maintaining both the old field and the new field impacts the memory footprint of
the type in which it lives.  In the case of ``param`` and ``type`` fields, it
can also lead to difficulties storing old and new versions in the same data
structure or break explicit declarations of the enclosing type.  It can make
default initializers no longer compatible with the original uses, requiring the
addition of explicit initializers.  Additionally, keeping the old and new fields
in sync with each other to maintain behavior and enable incremental replacement
is burdensome and might trigger deprecation warnings the user can't resolve
themselves.

With all of that in mind, the ideal strategy for replacing a renamed field with
another one is to make the old field into a paren-less method.  For instance:

.. code-block:: chapel

   record Foo {
     type oldName;
   }

can be transformed into:

.. code-block:: chapel

   record Foo {
     type newName;

     proc oldName type {
       return this.newName;
     }
   }

When replacing var fields, remember that fields are capable of being explicitly
set outside of the contents of the type.  Thus, the paren-less method should
use the ``ref`` return intent so that users are still able to update the field
using the old name:

.. code-block:: chapel

   record Foo {
     var newName: int;

     proc oldName ref: int {
       return this.newName;
     }
   }

   var f = new Foo(30);
   f.oldName += 3; // Should warn, but still function
   writeln(f.newName); // Should be 33

While this strategy enables positional initialization calls to continue to work
without adjustment, it does still break named initialization calls, e.g ``new
Foo(oldName=30)``.  The only solution for this is to add an initializer overload
with the old name that will generate a deprecation warning.  However, without
support for opting in to maintaining the default initializer (which is planned
but not currently implemented), this will also require the addition of an
equivalent replacement for the default initializer, which is a burden.

.. code-block:: chapel

   record Foo {
     var newName: int;

     proc oldName ref: int {
       return this.newName;
     }

     pragma "last resort"
     @deprecated("'new Foo(oldName=val)' is deprecated, please use 'new Foo(newName=val)' or 'new Foo(val)' instead")
     proc init(oldName: int) {
       this.newName = oldName;
     }

     // Required because previous initializer prevented the generation of the
     // default initializer
     // Could also write `newName: int = 0` to avoid writing two initializers
     proc init(newName: int) {
       this.newName = newName;
     }

     // Required because first initializer prevented the generation of the
     // default initializer
     // Relies on omitted field initialization
     proc init() {
     }
   }

   var f = new Foo(oldName=30); // Now warns
   var f2 = new Foo(newName=30); // Still works without warning
   var f3 = new Foo(30); // Still works without warning
   var f4 = new Foo(); // Still works without warning
