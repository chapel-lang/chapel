.. _readme-forwarding:

========================
Forwarding Methods Calls
========================

Chapel 1.15 includes a preview version of a method forwarding feature.
This feature allows a ``record`` or ``class`` to specify that certain
method calls will be forwarded to a particular expression. The most
typical use case is to forward methods to a particular field.

Why Forwarding?
---------------

While Chapel supports inheritance between classes as a way to reuse
method implementations, inheritance is not always appropriate. In
particular, inheritance has the drawback that it affects the types
of the classes and therefore changes what users can do with those
classes. Due to some of these drawbacks, there is a well-known design
pattern *composition over inheritance*.

The basic strategy for using composition instead of inheritance is to
declare a member field that stores the type that would be inherited from.
Then, declare methods that forward to the member field. This can allow
the implementation to be more flexible and re-usable. In particular,
changing how many classes participate in the implementation of a
particular type need not change its public interface or impact the users
of that type.

Besides these issues, composition is a more general strategy in Chapel
due to the following language design decisions:

 * records don't support inheritance.
 * multiple inheritance for classes is not supported.
 * inheriting a record from a class or a class from a record is not
   supported.

Composition can help in each of these cases where inheritance cannot
apply.

Example of Forwarding
---------------------

Suppose, for example, ``MyCircleImpl`` is a Chapel class, but the author
of that type wishes to present it as a record (say, ``MyCircle``).  Such
a division allows the type author to control what happens when the type
is copied and to manage the deletion of the class instances.

Consider the following example:

.. (comment) Note, these examples are tested here:
   test/classes/ferguson/forwarding/readme-forwarding-examples.chpl

.. code-block:: chapel

  class MyCircleImpl {
    var radius:real;
    proc area() {
      return pi*radius*radius;
    }
    proc circumference() {
      return 2.0*pi*radius;
    }
  }

  record MyCircle {
    var impl: MyCircleImpl;

    // forwarding methods
    proc area() {
      return impl.area();
    }
    proc circumference() {
      return impl.circumference();
    }
  }

Since writing these forwarding methods is repetitive and boring, it's a
good candidate for automation. The Chapel language allows automating the
creation of these forwarding methods by using the `forwarding` keyword.
For example, the following is equivalent to the previous ``MyCircle``
record.

.. code-block:: chapel

  record MyCircle {
    forwarding var impl: MyCircleImpl;
  }

The ``forwarding`` keyword instructs the compiler to forward method
invocations on a ``MyCircle`` that would otherwise not resolve to
the field ``impl``.

Using ``forwarding``
--------------------

Besides the ``forwarding var`` style, the ``forwarding`` syntax
can be applied to forward to a particular expression. For example,
one might wish to generate a different error in the event that ``impl``
is ``nil``:

.. code-block:: chapel

  record MyCircle {
    var impl: MyCircleImpl;

    proc getImplOrFail() {
      if impl == nil then
        halt("impl is nil");
      else
        return impl;
    }

    forwarding getImplOrFail();
  }

  var empty = new MyCircle(nil);
  empty.area(); // halts with "impl is nil"

Another direction that a user of ``forwarding`` might go is that they
might decide to only forward certain methods. For example, the following
are equivalent ways to forward only the ``area()`` method, assuming that
the only methods of ``MyCircleImpl`` are ``area()`` and
``circumference()``.

.. code-block:: chapel

  record MyCircle {
    var impl: MyCircleImpl;

    forwarding impl only area;
  }

.. code-block:: chapel

  record MyCircle {
    var impl: MyCircleImpl;

    forwarding impl except circumference;
  }

As with ``use``, ``forwarding`` supports comma-separated ``only`` and
``except`` lists.

Additionally, note that multiple ``forwarding`` declarations can be
specified.

Resolving Forwarded Methods
---------------------------

Forwarded methods resolve only when regular methods on a particular type
do not resolve. For example, in the above cases, if ``MyCircle`` declared
a ``area()`` method that could be called with no arguments, that method
would be called in preference to the forwarded method on ``MyCircleImpl``
that is available. Other than that, the forwarded methods participate
normally in the function resolution process. As with other ambiguous
function declarations, ambiguity errors will be reported at the call
site.

Note that special IO methods such as ``writeThis``,
``readThis``, and ``readWriteThis`` are not forwarded since the compiler
generates these for the type by default. Similarly, a field accessor will not
get forwarded if the class defines a conflicting field or method name. Consider
the example below:

.. code-block:: chapel

  record ArrayWrapper {
    var array: [1..0] int;

    proc shape {
      return 1;
    }

    forwarding array;
  }

The ``shape`` method above will resolve, meaning ``array.shape`` will not be
forwarded.

