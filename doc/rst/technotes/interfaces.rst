.. _readme-interfaces:

.. default-domain:: chpl


=========================================
Interfaces in the Chapel Standard Library
=========================================

Interfaces in Chapel are similar to a feature of the same name in object-oriented
languages such as Java, and to `traits in Rust <https://doc.rust-lang.org/book/ch10-02-traits.html>`_.
They provide a way to mark a type as exhibiting certain behavior or implementing
certain functionality. For instance, a ``hashable`` interface marks a type as
specifying a ``hash`` method (which would allow it to be used in
:ref:`associative domains <primers-associative>` or sets). The Chapel standard
library provides several interfaces that it expects code authors to implement
to opt in to various functionality.

The syntax for implementing an interface is similar to that for extending a
class; however, both records and classes can implement an interface. The
following example demonstrates a record ``R`` that implements the ``hashable``
interface mentioned in the previous paragraph.

.. code-block:: chapel

   record R : hashable {
     proc hash(): uint {
       return 0;
     }
   }

If a type implements multiple interfaces, they can be listed in a comma-separated
list:

.. code-block:: chapel

   record R : hashable, contextManager {
     proc hash(): uint { /* ... */ }

     proc enterThis() { /* ... */ }
     proc exitThis(in err: owned Error?) { /* ... */ }
   }

The same syntax also applies to classes. If a class extends another class in
addition to implementing an interface, the parent class and implemented interfaces
can be combined into a single list. It's recommended that the parent class
be listed first.

.. code-block:: chapel

   class Child : Parent, hashable, contextManager {
     proc hash(): uint { /* ... */ }

     proc enterThis() { /* ... */ }
     proc exitThis(in err: owned Error?) { /* ... */ }
   }

Semantics
~~~~~~~~~

Generally, an interface can require any number and combination of the following
three things from a type:

* A method or procedure with a particular type signature. For instance, the
  ``hashable`` interface requires a method called ``hash`` that accepts no
  arguments and returns a value of type ``uint``.

  For a type's function or method to match a function or method required by
  an interface, it must have the same name, formals, and return type.

* An associated type. For instance, the ``contextManager`` interface requires
  an associated type ``contextReturnType`` which represents the type of resource
  handled by the context manager. For ``contextManager`` specifically, this type
  can usually be inferred. However, in general, associated types must be
  explicitly specified by the user.

  See `Providing Associated Types`_ for an example of providing a
  ``contextReturnType`` explicitly.

* Another interface. For instance, the ``serializable`` interface represents
  a combination of the ``writeSerializable``, ``readDeserializable``, and
  ``initDeserializable`` interfaces. For a type to implement ``serializable``,
  it must satisfy the other three interfaces as well.

A function or an associated type that serves to satisfy a requirement of an
interface is typically called a `witness`.

When a type is marked as implementing an interface, the Chapel compiler will
ensure that it satisfies all of the interface's requirements. The compiler will
do so by checking the current scope and resolving any functions that fit the
interface criteria. Note that because only the current scope is searched,
methods defined in other files (and not made available by a ``use`` or ``import``
statement) cannot be used as witnesses for an interface.

Providing Associated Types
~~~~~~~~~~~~~~~~~~~~~~~~~~

An associated type is defined using a paren-less type-returning method. The
following code snippet demonstrates specifying ``myAssociatedType`` of record
``R`` to be ``int``.

.. code-block:: chapel

   record R {
     proc myAssociatedType type do return int;
   }

   var r: R;
   writeln(r.myAssociatedType : string); // prints 'int(64)'

Currently, ``contextManager`` is the only interface that requires an associated
type. This type, named ``contextReturnType``, is used to determine the expected
return type of the ``enterContext`` method. Because of this, it's usually
sufficient to declare this associated type in the following manner:

.. code-block:: chapel

   record R : contextManager {
     proc contextReturnType type do return this.enterContext().type;

     // contextManager methods
     proc enterContext() { return 0; }
     proc exitContext(in err: owned Error?) {}
   }

Since the above definition of ``contextReturnType`` works for most context
managers, the compiler will attempt to automatically provide the ``contextReturnType``.
Thus, the following definition is equivalent to the one above:

.. code-block:: chapel

   record R : contextManager {
     // contextManager methods
     proc enterContext() { return 0; }
     proc exitContext(in err: owned Error?) {}
   }

Built-In Interfaces
~~~~~~~~~~~~~~~~~~~

The Chapel standard library defines several interfaces. Implementing these
interfaces is required to opt in to various language features. These interfaces,
as well as the features they are used to implement, are as follows:

* The ``hashable`` interface is used to expose a type's hash function, making
  it usable for constructs in the language backed by a hash table or a similar
  data structure; such constructs include
  :ref:`associative domains <primers-associative>` or sets from the :mod:`Set` module.

  For more information on the ``hash`` method and the ``hashable`` interface,
  please see :ref:`Hashing_a_Record`.

* The ``contextManager`` interface is used with types that support the
  ``manage`` statement.

  For more information on the ``manage`` statement and the ``contextManager``
  interface, please see :ref:`the tech note for the 'manage' statement <readme-manage>`,
  as well as the relevant :ref:`section of the spec <The_Manage_Statement>`.

* The ``serializable`` interface is used with types that can be written out
  and read back in. For more details on what this means, see the
  :ref:`IO serializers tech note <ioSerializers>`. The ``serializable`` interface
  logically consists of the following three interfaces:

  * The ``writeSerializable`` interface is used with types that can be written
    out using a ``serialize`` method.

  * The ``readDeserializable`` interface is used with types that can be populated
    from a serializer once they have been created.

  * The ``initDeserializable`` interface is used with types that can be constructed
    from a deserializer. This differs from the 'read-serialization' because
    such types need not be allocated in advance of being deserialized.

Auto-Implemented Interfaces
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The Chapel compiler attempts to automatically generate certain methods for types,
to preclude the need for users to implement their own. For instance, records
without user-defined ``==`` and ``!=`` operators that lack a ``hash`` method
are automatically supplied with a compiler-generated ``hash``. This serves to
reduce the boilerplate for types that don't require custom logic. For some
interfaces (described in this section), the compiler will also automatically
generate an implementation if the required methods were generated.

For instance, consider the following snippet:

.. code-block:: chapel

   record R {
     var x: int;
   }

   var D: domain(R);
   writeln(D);
   writeln(new R());

In the snippet, a newly-defined record ``R`` could be used in an associative
domain and printed out using ``writeln``. Using these features requires the
record to be hashable and serializable, respectively. However, the user was
not required to manually implement ``hashable`` and ``writeSerializable``.

Generally (except for transitory measures; see `Migration Support`_),
the compiler will not generate methods if a user-defined method with
the same name exists; it will therefore also not generate the corresponding
interface. Additionally, certain other situations will stop the compiler from
generating methods and interface implementations; consult the documentation for
specific features to learn more.

The following interfaces are automatically implemented by the compiler:

* ``hashable``
* ``writeSerializable``
* ``readDeserializable``
* ``initDeserializable``

Migration Support
~~~~~~~~~~~~~~~~~

Prior to the Chapel 1.32 release, features that currently require interfaces
were handled by duck-typing. For example, the presence of a ``hash`` method
was sufficient to make a type hashable, and there was no need to also implement
an interface. To aid the gradual migration from duck-typed to interface-based
code, the compiler will automatically generate interface instances from
user-defined methods, without requiring an explicit ``record R : hashable``
declaration. This functionality is transitory, and should issue warnings.

Future Work
~~~~~~~~~~~

Planned work for interfaces includes stabilizing the following features:

* a syntax for declaring an interface
* a way to implement an interface for a type declared elsewhere
* functions whose arguments are constrained to implement an interface
