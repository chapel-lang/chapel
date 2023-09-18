
.. default-domain:: chpl

.. _Chapter-Bytes:

=====
Bytes
=====

The ``bytes`` type is similar to the :type:`~String.string` type but allows
arbitrary data to be stored in it. Methods on ``bytes`` that interpret the
data as characters assume that the bytes are ASCII characters.

.. _Bytes_Instantiation:

Bytes Instantiation and Casting
-------------------------------

A ``bytes`` instance can be created using the literals similar to strings,
prepended by a ``b`` character:

.. code-block:: chapel

   var b = b"my bytes";

The factory functions shown below, such as :proc:`bytes.createBorrowingBuffer <Bytes.bytes.createBorrowingBuffer>`,
allow you to create a ``bytes`` using a specific buffer (i.e. data in another
``bytes``, a ``c_string`` or a :class:`~CTypes.c_ptr`).

.. _Bytes_and_String:

*bytes* and *string*
~~~~~~~~~~~~~~~~~~~~

As ``bytes`` can store arbitrary data, any :type:`~String.string` can
be cast to ``bytes``. In that event, the bytes will store UTF-8 encoded
character data. However, in general, a *bytes* can contain non-UTF-8 bytes
and needs to be decoded to be converted to a *string*.

.. code-block:: chapel

  var s = "my string";
  var b = s:bytes;  // this is legal

  /*
   The reverse is not. The following is a compiler error:

   var s2 = b:string;
  */

  var s2 = b.decode(); // you need to decode a bytes to convert it to a string

See the :proc:`~Bytes.bytes.decode` method below for details.

Similarly, a ``bytes`` can be initialized using a ``string``:

.. code-block:: chapel

   var s = "my string";
   var b: bytes = s;

Casts from *bytes* to a Numeric Type
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Chapel supports casts from ``bytes`` to numeric types. Such casts will
interpret the *bytes* as ASCII characters, convert it to the numeric type
and then throw an error if the *bytes* does not match the expected format
of a number. For example:

.. code-block:: chapel

  var b = b"a";
  var number = b:int;

throws an error when it is executed, but

.. code-block:: chapel

  var b = b"1";
  var number = b:int;

stores the value ``1`` in ``number``.

To learn more about handling these errors, see the Language-Specification page
on :ref:`Error Handling <Chapter-Error_Handling>`.

.. _Bytes_Methods:

Predefined Routines on Bytes
----------------------------

The *bytes* type:

.. include:: /builtins/Bytes.rst
