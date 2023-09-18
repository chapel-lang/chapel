.. _ioSerializers:

.. default-domain:: chpl

================================
IO Serializers and Deserializers
================================

Overview
--------

Historically, Chapel's IO module supported formatting options for reading and
writing values in non-standard formats via the ``readf`` and ``writef`` methods
(e.g., ``%jt`` for JSON). Chapel 1.31 introduces a new API that allows for
user-defined formatting with ``fileReader`` and ``fileWriter``, rather than
relying solely on built-in support in the standard library. This new API allows
for the configuration of ``fileReader`` and ``fileWriter`` with user-defined
types that can define the format used by methods like ``read`` and ``write``.

For example, if a user wishes to write a record in JSON format they can now
use the ``Json`` package module in Chapel 1.31:

.. code-block:: chapel

   use IO, Json;

   record Person {
     var name : string;
     var age : int;
   }

   var f = open("output.txt", ioMode.cw);

   // configure 'w' to always write in JSON format
   var w = f.writer(serializer=new JsonSerializer());

   // writes:
   // {"name":"Sam", "age":20}
   var p = new Person("Sam", 20); 
   w.write(p);

Serializers and Deserializers interact with user-defined types like ``Person``
by invoking a particular set of methods which will be discussed in more detail
later in this technote. By default, the compiler will generate methods on
user-defined types capable of interacting with Serializers and Deserializers
such that most types will simply work out of the box. For more complicated
cases, users can implement their own methods on their types to customize
serialization and deserialization.

In the interest of supporting gradual updating of code, in Chapel 1.31 the
standard IO library will *not* use Serializers or Deserializers by default.
Users can opt-in to particular formats by creating ``fileReader`` and
``fileWriter`` instances with new Serializer and Deserializer types. Users
wishing to experiment with exclusively using this new feature can set the
boolean config param ``useIOSerializers``:

.. code-block:: sh

   chpl foo.chpl -suseIOSerializers

This config param configures ``fileReader`` and ``fileWriter`` to use the
``DefaultDeserializer`` and ``DefaultSerializer`` types by default, which
implement the exact same formatting as past releases.

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
                 reader: fileReader(deserializerType=JsonDeserializer, ?)) {
   }

   proc readData(data: [],
                 reader: fileReader(deserializerType=BinaryDeserializer, ?)) {
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
       log.withSerializer(new JsonSerializer()).writeln(d);

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
   log.withSerializer(JsonSerializer).writeln(d);

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

In Chapel 1.31 generic types and borrowed classes are no longer valid arguments
to the versions of ``read`` and ``readln`` that accept a ``type`` argument.
Note that fully-instantiated generic types are still allowed.

Serializer API
--------------

A Serializer must implement the ``serializeValue`` method:

.. code-block:: chapel

   proc Serializer.serializeValue(writer: fileWriter, const val: ?) throws

The ``serializeValue`` method returns nothing, and once invoked has complete
control over how the provided value is serialized. The given ``fileWriter`` is
guaranteed to have a ``serializerType`` identical to the type whose
``serializeValue`` method was called. The ``fileWriter`` is also defined to be
non-locking.

By convention Serializers will invoke a ``serialize`` method on records and
classes, but notably may choose not to do so if the class instance is ``nil``.

The 'serialize' Method
^^^^^^^^^^^^^^^^^^^^^^

The ``serialize`` method has the following signature, whose API includes the
named arguments "writer" and "serializer":

.. code-block:: chapel

   proc T.serialize(writer: fileWriter(?),
                    ref serializer: writer.serializerType) throws

The ``writer`` and ``serializer`` are passed separately to help distinguish the
method signature from other possible implementations named "serialize", as well
as to make it slightly more convenient to call methods on the Serializer. A
future release will standardize other methods on a Serializer that provide ways
to serialize into common types, like lists or maps.

It is an error for ``writer.serializer`` to refer to a different Serializer
instance than the ``serializer`` argument. The Serializer is responsible for
either passing itself to the 'serializer' argument, or if applicable can create
a new instance of itself to pass. The appropriate choice here depends on the
degree to which the Serializer relies on internal state, and how that internal
state must be managed. If a copy must be made, then the ``withSerializer``
method may be used to provide an alias.

.. note::

   The set of standard builtin types (e.g. ranges and domains) on which this
   method may be invoked is currently unstable.

Deserializer API
----------------

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

In both methods, the given ``fileReader`` is guaranteed to have a
``deserializerType`` identical to the type whose method was called. The
``fileReader`` is also defined to be non-locking.

Note that while both methods may invoke initializers or methods that pass
control back to the user, Deserializers may ignore those options in the case
that a class is nilable and can be read as ``nil``.

The Deserializing Initializer
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

An initializer invoked by a Deserializer must have the following signature,
including the argument names "reader" and "deserializer":

.. code-block:: chapel

   proc T.init(reader: fileReader(?),
               ref deserializer: reader.deserializerType) throws

By default, the compiler will generate a suitable initializer with this
signature provided that no other user-defined initializers exist.

The ``reader`` and ``deserializer`` are passed separately to help distinguish
the method signature from other possible initializers, as well as to make it
slightly more convenient to call methods on the Deserializer. A future release
will standardize other methods on a Deserializer that provide ways to
deserialize into common types, like lists or maps.

As with the ``serialize`` method, it is an error for ``reader.deserializer`` to
refer to a Deserializer other than the ``deserializer`` argument.

Generic types have a slightly more complex initializer signature, in that there
must be a ``type`` or ``param`` argument for each ``type`` or ``param`` field.
For example:

.. code-block:: chapel

   record G {
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
                          ref deserializer: reader.deserializerType) throws

By default, the compiler will generate a suitable ``deserialize`` method with
this signature provided.

As with the ``serialize`` method, it is an error for ``reader.deserializer`` to
refer to a Deserializer other than the ``deserializer`` argument.

.. note::

   The set of standard builtin types (e.g. ranges and domains) on which this
   method may be invoked is currently unstable.

Compiler-Generated Methods
--------------------------

Generation of the deserializing initializer, or the ``serialize`` and
``deserialize`` methods can be disabled with the flag
``--no-io-gen-serialization``.

If the compiler sees a user-defined implementation of the ``serialize`` method,
the ``deserialize`` method, or the deserializing initializer, then the compiler
may choose to not automatically generate any of the other unimplemented
methods. This is out of concern that the user has intentionally deviated from
the compiler's default implementation of serialization and deserialization.

Until it is determined that ``readThis`` and ``writeThis`` will be deprecated,
the compiler-generated versions of ``serialize`` and ``deserialize`` methods
will call any user-defined ``readThis`` or ``writeThis`` methods available on
the same type. If this behavior is undesirable, users may implement their own
``serialize`` and ``deserialize`` methods, or they may use the following
compiler flags:
- ``--no-io-serialize-writeThis``
- ``--no-io-deserialize-readThis``
