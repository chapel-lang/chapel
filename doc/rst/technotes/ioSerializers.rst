.. _ioSerializers:

.. default-domain:: chpl

================================
IO Serializers and Deserializers
================================

Overview
--------

Historically, Chapel's IO module supported formatting options for reading and
writing values in non-standard formats via the ``readf`` and ``writef`` methods
(e.g., ``%jt`` for JSON). Chapel 1.32 introduced a new API that allows for
user-defined formatting with ``fileReader`` and ``fileWriter``, rather than
relying solely on built-in support in the standard library. This new API allows
for the configuration of ``fileReader`` and ``fileWriter`` with user-defined
types that can define the format used by methods like ``read`` and ``write``.

For example, if a user wishes to write a record in JSON format they can now
use the ``JSON`` standard module in Chapel 1.32:

.. code-block:: chapel

   use IO, JSON;

   record Person {
     var name : string;
     var age : int;
   }

   var f = open("output.txt", ioMode.cw);

   // configure 'w' to always write in JSON format
   var w = f.writer(serializer=new jsonSerializer());

   // writes:
   // {"name":"Sam", "age":20}
   var p = new Person("Sam", 20);
   w.write(p);

Serializers and Deserializers interact with user-defined types like ``Person``
by invoking particular methods whose API will be discussed in more detail
later in this technote. By default, the compiler will generate methods on
user-defined types capable of interacting with Serializers and Deserializers
such that most types will simply work out of the box. For more complicated
cases, users can implement their own methods on their types to customize
serialization and deserialization.

In Chapel 1.32, Serializers and Deserializers are enabled by default. Users
wishing to opt-out of this capability can recompile their programs with
the config param ``useIOSerializers`` set to ``false``. This config param
will be available through the Chapel 1.34 release at minimum.

API Changes to Standard IO
--------------------------

Before diving into the API that Serializers and Deserializers must implement,
there are additions to the API of standard IO types. For the purposes of this
document, "Serializer" or "Deserializer" refer to types that implement the
appropriate API that the standard IO types will invoke.

Creating fileReaders and fileWriters
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``fileReader`` and ``fileWriter`` types can now be created with a specified
Serializer or Deserializer. The following methods now contain new optional
``serializer`` or ``deserializer`` arguments that accept a record by the
``in`` intent. The copy of the record will be stored inside of the
``fileReader/Writer``. The default value for these arguments when
``-suseIOSerializers`` is used will be an instance of ``DefaultSerializer`` or
``DefaultDeserializer``.

.. code-block:: chapel

  proc openWriter(path:string,
                  param kind=iokind.dynamic, param locking=true,
                  hints = ioHintSet.empty,
                  in serializer: ?st = new DefaultSerializer())

  proc file.writer(param kind=iokind.dynamic, param locking=true,
                   region: range(?) = 0.., hints = ioHintSet.empty,
                   in serializer: ?st = new DefaultSerializer())

  proc openReader(path:string,
                  param kind=iokind.dynamic, param locking=true,
                  region: range(?) = 0.., hints=ioHintSet.empty,
                  in deserializer: ?dt = new DefaultDeserializer())

  proc file.reader(param kind=iokind.dynamic, param locking=true,
                   region: range(?) = 0.., hints = ioHintSet.empty,
                   in deserializer: ?dt = new DefaultDeserializer())

New Fields on fileReader and fileWriter
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The ``fileReader`` and ``fileWriter`` types each have a new ``type`` field
named ``deserializerType`` and ``serializerType`` respectively. These fields
can be used to constrain arguments to better separate code dedicated to
particular serialization formats:

.. code-block:: chapel

   proc readData(data: [],
                 reader: fileReader(deserializerType=jsonDeserializer, ?)) {
   }

   proc readData(data: [],
                 reader: fileReader(deserializerType=binaryDeserializer, ?)) {
   }

Accessing Serializers and Deserializers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The instance of a Serializer or Deserializer can be accessed with new methods
on ``fileReader`` and ``fileWriter``, which will return the stored instance
by ``ref``:

.. code-block:: chapel

   proc fileReader.deserializer ref : deserializerType

   proc fileWriter.serializer ref : serializerType

These instances are returned by ``ref`` in case complex implementations require
modification of some internal state.

Switching Formats In-Place
^^^^^^^^^^^^^^^^^^^^^^^^^^

The IO library now supports the ability to create an alias of a ``fileReader``
or ``fileWriter`` with a new Deserializer or Serializer. This new alias will
point to the same place in the file as the original, but will use the newly
specified format when reading or writing. These methods accept either a record
by ``in`` intent, or a ``type``.

.. code-block:: chapel

   proc fileWriter.withSerializer(type serializerType) :
     fileWriter(this.kind, this.locking, serializerType)

   proc fileWriter.withSerializer(in serializer: ?st) :
     fileWriter(this.kind, this.locking, st)

   proc fileReader.withDeserializer(type deserializerType) :
     fileReader(this.kind, this.locking, deserializerType)

   proc fileReader.withDeserializer(in deserializer: ?dt) :
     fileReader(this.kind, this.locking, dt)

With these methods, mixing serialization formats within the same file is
a simple process:

.. code-block:: chapel

   // An imaginary 'Connection' object that wishes to log the data it sends
   // as JSON in the form "[INFO] {...}"
   proc Connection.sendData(data: [] Info, log: fileWriter) {
     log.writeln("[DEBUG] Sending Info data...");

     for d in data {
       log.write("[INFO] ");
       log.withSerializer(new jsonSerializer()).writeln(d);

       this.sendInfo(d);
     }

     log.writeln("[DEBUG] Done sending Info data.");
   }

The ``type`` versions of these methods exist for convenience in the case that
the user wishes for the ``fileReader`` or ``fileWriter`` to create the instance
itself. The Serializer or Deserializer in such cases must support
initialization without any arguments.

.. code-block:: chapel

   // Replacing the line from the previous example
   log.withSerializer(jsonSerializer).writeln(d);

Methods That Invoke Serializers and Deserializers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The current methods on ``fileReader`` and ``fileWriter`` that will invoke
Serializers or Deserializers are:

- fileWriter.write
- fileWriter.writeln
- fileReader.read
- fileReader.readln

Reading Generic Types and Borrowed Classes
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

As of Chapel 1.31 generic types and borrowed classes are no longer valid
arguments to the versions of ``read`` and ``readln`` that accept a ``type``
argument. Note that fully-instantiated generic types are still allowed.

.. _io-serializer-API:

Serializer API
--------------

The API for a Serializer can be split into a few parts:

1. The interface invoked by a ``fileWriter`` to serialize a value
2. The user-defined ``serialize`` method describing how values of the type should be serialized
3. The serializer interface used to implement ``serialize`` methods

The fileWriter-Facing Serializer API
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A Serializer must implement the ``serializeValue`` method, which will be used
to serialize values passed to ``fileWriter.write`` and ``fileWriter.writeln``.

The signature of the ``serializeValue`` method is:

.. code-block:: chapel

   proc Serializer.serializeValue(writer: fileWriter, const val: ?) throws

The ``serializeValue`` method returns nothing, and once invoked has complete
control over how the provided value is serialized. The given ``fileWriter`` is
guaranteed to have a ``serializerType`` identical to the type whose
``serializeValue`` method was called. The ``fileWriter`` is also defined to be
non-locking.

By convention Serializers will invoke a ``serialize`` method on records and
classes, but notably may choose not to do so if the class instance is ``nil``.

The implementation of ``serializeValue`` is expected to handle primitive types
directly. Those primitive types are:
- ``numeric`` types (e.g., integers, reals, complex numbers)
- ``bool`` types
- ``string`` and ``bytes`` types
- ``nil`` and ``none`` values
- ``enum`` types

The argument ``val`` is defined to be a "primitive" type or a type that
implements either the ``writeSerializable`` or ``serializable`` interfaces,
both of which define a ``serialize`` method that a Serializer may invoke to
allow for user-defined serialization of a type.

The 'serialize' Method
^^^^^^^^^^^^^^^^^^^^^^

The ``serialize`` method has the following signature, whose API includes the
named arguments "writer" and "serializer":

.. code-block:: chapel

   proc T.serialize(writer: fileWriter(?), ref serializer: ?st) throws

Types implementing this method must also indicate that they satisfy the
``writeSerializable`` interface in the type declaration. For example:

.. code-block:: chapel

   record R : writeSerializable {
     // ...
   }

Please refer to the :ref:`interfaces technote<readme-interfaces>` for more
information on interfaces and how they can be used.

For classes, the ``serialize`` method signature must include ``override`` to
account for the ``serialize`` method on the ``RootClass`` type.

The ``writer`` and ``serializer`` are passed separately to help distinguish the
method signature from other possible implementations named "serialize", as well
as to make it slightly more convenient to call methods on the Serializer.

The ``serializer`` argument does not necessarily need to be of the same type as
``writer.serializerType``. Instead, the argument simply needs to implement the
Serializer API and must serialize in a compatible format with
``writer.serializerType``. This constraint exists to allow for child classes to
pass helper objects created by Serializers to parent class ``serialize``
methods. See the :ref:`serializer inheritance<serializerInheritance>` section
for more information.

.. _io-serializer-user-API:

The User-Facing Serializer API
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The user-facing part of the Serializer API is intended to allow users to
serialize their types in a format-agnostic way. This is done by invoking a
variety of API methods, instead of printing specific characters for a specific
format.

The user-facing part of the Serializer API is much larger, and is designed to
support serializing various "kinds" of types. In particular, the API currently
supports serializing Classes, Records, Tuples, Arrays, Lists, and Maps. A given
implementation of a Serializer determines how to represent each kind of type
in its format. For example, JSON lacks a native representation of tuples, and
so the ``JSON`` Serializer represents both "list" and "tuple" type-kinds as
JSON lists (e.g. ``[1, 2, 3]``).

To begin serializing a kind of type, users will invoke one of six available
"start" methods on a Serializer, each of which return a "helper" object that
implements an API specific to that kind of type. Note that any of the "start"
methods may return the same "helper" type as another method, in the case that
it is useful for the helper to share logic among certain type kinds. For
example, in the Chapel 1.32 release the ``defaultSerializer`` type returned
the same helper object type for both Class and Record type kinds.

.. note::

   In each of these methods, unless otherwise stated, it is entirely up to the
   author of the Serializer to define their behavior. For example, ``name``
   arguments for classes and records may not apply to a particular format, and
   might be ignored.

.. note::

  In each of these groups of methods, it should be noted that the name of each
  helper object is purely illustrative, and does not indicate the name of a
  stable interface to be implemented in the future.

The Record Helper
~~~~~~~~~~~~~~~~~

Users may begin serializing a Record type kind by invoking the ``startRecord``
method on a Serializer. This method takes a ``name`` argument that represents
the name of the record type, and a ``size`` argument that represents the
number of fields to be serialized.

.. code-block:: chapel

  proc Serializer.startRecord(writer: fileWriter(false, this.type), name: string, size: int) : RecordHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Serialize a field named 'name'
  proc RecordHelper.writeField(name: string, const field: ?) throws;

  // End the record according to the serialization format.
  proc RecordHelper.endRecord() throws;

The Tuple Helper
~~~~~~~~~~~~~~~~~

Users may begin serializing a Tuple type kind by invoking the ``startTuple``
method on a Serializer. This method takes a ``size`` argument that represents
the number of elements to be serialized.

.. code-block:: chapel

  proc Serializer.startTuple(writer: fileWriter(false, this.type), size: int) : TupleHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Serialize an element of the tuple.
  proc TupleHelper.writeElement(const element: ?) throws;

  // End the tuple according to the serialization format.
  proc TupleHelper.endTuple() throws;

The Array Helper
~~~~~~~~~~~~~~~~

Users may begin serializing an Array type kind by invoking the ``startArray``
method on a Serializer. This method takes a ``size`` argument that represents
the number of array elements to be serialized.

.. code-block:: chapel

  proc Serializer.startArray(writer: fileWriter(false, this.type), size: int) : ArrayHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Serialize the start of a new dimension of size ``size``
  proc ArrayHelper.startDim(size: int) throws;

  // Serialize the end of the current dimension
  proc ArrayHelper.endDim() throws;

  // Serializer an element of the array.
  proc ArrayHelper.writeElement(const element: ?) throws;

  // End the array according to the serialization format.
  proc ArrayHelper.endArray() throws;

ArrayHelpers may also optionally implement a ``writeBulkElements`` method for
performance:

.. code-block:: chapel

  // If the format permits, write 'numElements' of 'data' in bulk.
  proc ArrayHelper.writeBulkElements(data: c_ptr(?eltType), numElements: int) throws;

.. note::

   Currently users can only test for ``writeBulkElements`` support by using
   :mod:`Reflection`. Improvements to interfaces *may* provide a more elegant
   approach to the 'optional' aspect of this method in the future.

The List Helper
~~~~~~~~~~~~~~~

Users may begin serializing a List type kind by invoking the ``startList``
method on a Serializer. This method takes a ``size`` argument that represents
the number of list elements to be serialized.

.. code-block:: chapel

  proc Serializer.startList(writer: fileWriter(false, this.type), size: int) : ListHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Serialize the list element.
  proc ListHelper.writeElement(const element: ?) throws;

  // End the list according to the serialization format.
  proc ListHelper.endList() throws;

The Map Helper
~~~~~~~~~~~~~~

Users may begin serializing a Map type kind by invoking the ``startMap``
method on a Serializer. This method takes a ``size`` argument that represents
the number of map entries to be serialized.

.. code-block:: chapel

  proc Serializer.startMap(writer: fileWriter(false, this.type), size: int) : MapHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Serialize a map key.
  proc MapHelper.writeKey(const key: ?) throws;

  // Serialize a map value.
  proc MapHelper.writeValue(const val: ?) throws;

  // End the map according to the serialization format.
  proc MapHelper.endMap() throws;

.. _serializerInheritance:

The Class Helper, Serializers, and Inheritance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Users may begin serializing a Class type kind by invoking the ``startClass``
method on a Serializer. The ``writer`` argument is passed in and will be used
by the returned ClassHelper to write serialized output. The ``name`` argument
is expected to be the name of the class type being serialized. The ``size``
argument is the number of fields being serialized in the current class,
excluding any parent fields. Parent fields are not included to preserve
encapsulation of class implementations and to avoid the inextricable coupling
of parent and child classes.

.. code-block:: chapel

  proc Serializer.startClass(writer: fileWriter(false, this.type), name: string, size: int) : ClassHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Serialize a field named 'name'
  proc ClassHelper.writeField(name: string, const field: ?) throws;

  // End the class according to the serialization format
  proc ClassHelper.endClass() throws;

ClassHelpers are also required to implement *the rest* of the Serializer API
since they may be passed to parent ``serialize`` methods in the
compiler-generated default implementation of ``serialize`` methods on classes.

This may be achieved without too much extra effort by using
:ref:`forwarding<readme-forwarding>` on the stored ``fileWriter``'s
``.serializer`` accessor. By allowing ClassHelpers to be passed to parent
``serialize`` methods, formats may capture an inheritance hierarchy if such is
relevant to their format.

The following code snippet is an example of writing ``serialize`` methods for
a parent and child class:

.. code-block:: chapel

  class Parent : writeSerializable {
    var x : int;
  }

  class Child : Parent, writeSerializable {
    var y : int;
  }

  // When serializing an instance of 'Parent', 'serializer' could be the same
  // type as 'writer.serializerType'.
  //
  // When serializing an instance of 'Child', 'serializer' could be a
  // ClassHelper type, and so the ClassHelper must satisfy the Serializer API.
  override proc Parent.serialize(writer: fileWriter(?), ref serializer) {
    var ser = serializer.startClass(writer, "Parent", 1);
    ser.writeField("x", x);
    ser.endClass();
  }

  override proc Child.serialize(writer: fileWriter(?), ref serializer) {
    var ser = serializer.startClass(writer, "Child", 1);

    // pass the ClassHelper 'ser' to the parent 'serialize' method
    super.serialize(writer, ser);

    ser.writeField("y", y);
    ser.endClass();
  }

User Facing API Notes
~~~~~~~~~~~~~~~~~~~~~

.. note::

   This document does not define what errors these methods may or may not
   throw.

.. _io-deserializer-API:

Deserializer API
----------------

The API for a Deserializer can be split into a few parts:

1. The interface invoked by a ``fileReader`` to deserialize a value
2. The user-defined ``deserialize`` method and initializer describing how values of the type should be deserialized
3. The deserializer interface used to implement ``deserialize`` methods and deserializing initializers

The fileReader-Facing Serializer API
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

A Deserializer must implement the following methods, corresponding to the
versions of ``fileReader.read`` that accept either a type or a value:

.. code-block:: chapel

   proc Deserializer.deserializeType(reader: fileReader,
                                     type readType) : readType throws

   proc Deserializer.deserializeValue(reader: fileReader,
                                      ref val: ?readType) : void throws

The ``deserializeType`` method is responsible for creating a new instance of
the given type, and returning that new instance. By convention
``deserializeType`` will invoke a initializer by passing in the ``reader`` and
a Deserializer. This technote will refer to such initializers with the desired
signature as "deserializing initializers", which can be generated by the
compiler. If a suitable initializer is not available, this method may attempt
to invoke a ``deserialize`` method on a default-initialized value.

The ``deserializeValue`` method must modify an existing value, which can be
useful for types that are not cheap to allocate and benefit from re-use (e.g.
arrays). By convention ``deserializeValue`` will invoke a ``deserialize``
method on records and classes. If a suitable ``deserialize`` method is not
available, this method may attempt to invoke a suitable initializer and assign
the result into the value.

For classes, the ``deserializeValue`` method has the freedom to potentially
free the given class and/or reassign it, depending on the needs of the
Deserializer.

The arguments ``val`` or ``readType`` are defined to be a "primitive" type or a
type that implements at least one of the following interfaces:

- ``readDeserializable``
- ``initDeserializable``
- ``serializable`` (combines ``writeSerializable`` with the two above)

In both methods, the given ``fileReader`` is also defined to be non-locking.

Note that while both methods may invoke initializers or methods that pass
control back to the user, Deserializers may ignore those options in the case
that a class is nilable and can be read as ``nil``.

The Deserializing Initializer
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

An initializer invoked by a Deserializer must have the following signature,
including the argument names "reader" and "deserializer":

.. code-block:: chapel

   proc T.init(reader: fileReader(?),
               ref deserializer: ?dt) throws

Types implementing this method must also indicate that they satisfy the
``initDeserializable`` interface. Please refer to the
:ref:`interfaces technote<readme-interfaces>` for more information on
interfaces and how they can be used.

By default, the compiler will generate a suitable initializer with this
signature provided that no other user-defined initializers exist.

The ``reader`` and ``deserializer`` are passed separately to help distinguish
the method signature from other possible initializers, as well as to make it
slightly more convenient to call methods on the Deserializer.

The ``deserializer`` argument must implement the Deserializer API and must
deserialize in a compatible format with ``reader.deserializerType``. This
constraint exists to allow for child classes to pass helper objects created
by Deserializers to parent initializers. See the previous section on
:ref:`serializer inheritance<serializerInheritance>` for more information.

Generic types have a slightly more complex initializer signature, in that there
must be a ``type`` or ``param`` argument for each ``type`` or ``param`` field.
For example:

.. code-block:: chapel

   record G : initDeserializable {
     type A;
     type B;
     var x : A;
     var y : B;
   }

   proc G.init(type A, type B,
               reader: fileReader, ref deserializer) throws {
     /* ... */
   }

   // With a reader 'r'
   var x = r.read(G(int, real));
   // becomes something like...
   // new G(A=int, B=real, reader=r, deserializer=r.deserializer)

.. warning::

   Generic types with typeless fields, like "var x;", cannot yet be
   deserialized using an initializer.

.. warning::

   Throwing inside an initializer before the type is fully initialized is not
   yet allowed in Chapel.

The 'deserialize' Method
^^^^^^^^^^^^^^^^^^^^^^^^

The ``deserialize`` method has the following signature, and also requires
its arguments to have the names "reader" and "deserializer":

.. code-block:: chapel

   proc ref T.deserialize(reader: fileReader(?),
                          ref deserializer: ?dt) throws

For classes, this signature is slightly different in that it requires the
``override`` keyword and a blank this-intent:

.. code-block:: chapel

   override proc T.deserialize(reader: fileReader(?),
                               ref deserializer: ?dt) throws

By default, the compiler will generate a suitable ``deserialize`` method with
this signature provided.

Types implementing this method must also indicate that they satisfy the
``readDeserializable`` interface. Please refer to the
:ref:`interfaces technote<readme-interfaces>` for more information on
interfaces and how they can be used.

.. _io-deserializer-user-API:

The User-Facing Deserializer API
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Like the Serializer API, the user-facing part of the Deserializer API is
relatively large and supports the same set of type kinds as a Serializer. Also
like the Serializer API, the Deserializer API works through the creation and
use of helper objects returned by various "start" methods.

The Deserializer API is also slightly larger due to the need for "type" and
"by reference" versions of methods like ``readElement``, to match the desired
behavior of the originating ``fileReader.read`` call. The List and Map type
kinds also support a ``hasMore`` method to help users know when they can stop
reading.

.. note::

   In each of these methods, unless otherwise stated, it is entirely up to the
   author of the Deserializer to define their behavior. For example, ``name``
   arguments for classes and records may not apply to a particular format, and
   might be ignored.

.. note::

  In each of these groups of methods, it should be noted that the name of each
  helper object is purely illustrative, and does not indicate the name of a
  stable interface to be implemented in the future.

The Record Helper
~~~~~~~~~~~~~~~~~

Users may begin deserializing a Record type kind by invoking the
``startRecord`` method on a Deserializer. This method takes a ``name`` argument
that represents the name of the record type.

.. code-block:: chapel

  proc Deserializer.startRecord(reader: fileReader(false, this.type), name: string) : RecordHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Deserialize a field named 'name', returns a value of type 'fieldType'
  proc RecordHelper.readField(name: string, type fieldType) : fieldType throws;

  // Deserialize a field named 'name' in-place.
  proc RecordHelper.readField(name: string, ref field :?) throws;

  // End the record according to the deserialization format.
  proc RecordHelper.endRecord() throws;

The Tuple Helper
~~~~~~~~~~~~~~~~

Users may begin deserializing a Tuple type kind by invoking the ``startTuple``
method on a Deserializer.

.. code-block:: chapel

  proc Deserializer.startTuple(reader: fileReader(false, this.type)) : TupleHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Deserialize an element of the tuple, return a value of type 'eltType'
  proc TupleHelper.readElement(type eltType) : eltType throws;

  // Deserialize 'element' as a tuple element in-place.
  proc TupleHelper.readElement(ref element: ?) throws;

  // End the tuple according to the deserialization format.
  proc TupleHelper.endTuple() throws;

The Array Helper
~~~~~~~~~~~~~~~~

Users may begin deserializing an Array type kind by invoking the ``startArray``
method on a Deserializer.

.. code-block:: chapel

  proc Deserializer.startArray(reader: fileReader(false, this.type)) : ArrayHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Deserialize an element of the array, return a value of type 'eltType'
  proc ArrayHelper.readElement(type eltType) : eltType throws;

  // Deserialize 'element' as an array element in-place.
  proc ArrayHelper.readElement(ref element: ?) throws;

  // Start deserializing a new dimension
  proc ArrayHelper.startDim() throws;

  // End the array dimension according to the deserialization format.
  proc ArrayHelper.endDim() throws;

  // End the array according to the deserialization format.
  proc ArrayHelper.endArray() throws;

ArrayHelpers may also optionally implement a ``readBulkElements`` method for
performance:

.. code-block:: chapel

  // If the format permits, write 'numElements' of 'data' in bulk.
  proc ArrayHelper.readBulkElements(data: c_ptr(?eltType), n: int) throws;

.. note::

   Currently users can only test for ``readBulkElements`` support by using
   :mod:`Reflection`. Improvements to interfaces *may* provide a more elegant
   approach to the 'optional' aspect of this method in the future.

The List Helper
~~~~~~~~~~~~~~~

Users may begin deserializing a List type kind by invoking the ``startList``
method on a Deserializer.

.. code-block:: chapel

  proc Deserializer.startList(reader: fileReader(false, this.type)) : ListHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Deserialize an element of the list, return a value of type 'eltType'
  proc ListHelper.readElement(type eltType) : eltType throws;

  // Deserialize 'element' as a list element in-place.
  proc ListHelper.readElement(ref element: ?) throws;

  // Returns 'true' if there are more elements to deserialize
  proc ListHelper.hasMore() : bool throws;

  // End the list according to the deserialization format.
  proc ListHelper.endList() throws;

The Map Helper
~~~~~~~~~~~~~~

Users may begin deserializing a Map type kind by invoking the ``startMap``
method on a Deserializer.

.. code-block:: chapel

  proc Deserializer.startMap(reader: fileReader(false, this.type)) : MapHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Deserialize a key of the map, return a value of type 'keyType'
  proc MapHelper.readKey(type keyType) : keyType throws;

  // Deserialize 'key' as a map key in-place.
  proc MapHelper.readKey(ref key: ?) throws;

  // Deserialize a value of the map, return a value of type 'valType'
  proc MapHelper.readValue(type valType) : valType throws;

  // Deserialize 'value' as a map value in-place.
  proc MapHelper.readValue(ref value: ?) throws;

  // Returns 'true' if there are more map entries to deserialize
  proc MapHelper.hasMore() : bool throws;

  // End the map according to the deserialization format.
  proc MapHelper.endMap() throws;

The Class Helper
~~~~~~~~~~~~~~~~

Users may begin deserializing a Class type kind by invoking the ``startClass``
method on a Deserializer. This method takes a ``name`` argument that represents
the name of the class type.

.. code-block:: chapel

  proc Deserializer.startClass(reader: fileReader(false, this.type), name: string) : ClassHelper throws;

The returned object must implement the following API:

.. code-block:: chapel

  // Deserialize a field named 'name', returns a value of type 'fieldType'
  proc ClassHelper.readField(name: string, type fieldType) : fieldType throws;

  // Deserialize a field named 'name' in-place.
  proc ClassHelper.readField(name: string, ref field :?) throws;

  // End the class according to the deserialization format.
  proc ClassHelper.endClass() throws;

Like in the Serializer API, the ClassHelper must implement *the rest* of the
Deserializer API to allow for the ClassHelper to be passed to parent
initializers and parent ``deserialize`` methods.

The 'serializable' Interface
----------------------------

The ``serializable`` interface mentioned on this document is intended to be
an interface that requires implementation of all three kinds of user-defined
methods: ``serialize``, ``deserialize``, and a deserializing initializer.

A formal definition of this interface is pending, following the standardization
of interfaces in the language.

