===============
Chapel Protobuf
===============

Chapel Protobuf is a Google Protocol Buffers implementation for Chapel.
`Protocol Buffers`_ is a language-neutral, platform-neutral, extensible mechanism
for serializing structured data. The protocol buffer language is a language for 
specifying the schema for structured data. This schema is compiled into language
specific bindings.

This project is comprised of two components:

* Code generator: The ``protoc-gen-chpl`` tool is a compiler plugin to ``protoc``, the protocol
  buffer compiler. It enables the ``protoc`` compiler to generate Chapel code.
  
* Chapel library: A module that has the runtime implementation of protobufs
  in Chapel and provides functionality to serialize messages in wire format.


Installation Instructions
=========================

You should have the protocol buffer compiler, ``v3.0.0`` or higher, installed on your system.
Please see the official installation `guide`_.

Once you have the compiler installed, in ``$CHPL_HOME`` run the following:

.. code-block:: sh

   make protoc-gen-chpl
  
It builds the ``protoc-gen-chpl`` binary so that the command line interface can be used.
This installs ``protoc-gen-chpl`` in the same place as the chapel compiler (``chpl``) so that
it can be used anywhere in the user's file system.

To remove protobuf suppot, change directory to ``$CHPL_HOME/tools/protoc-gen-chpl`` and run:

.. code-block:: sh

   make clean

Defining protocol format
========================

To make use of the Chapel protobuf implementation you need to start by defining
a ``.proto`` file. The definitions in a ``.proto`` file contain a message for each
data structure you want to serialize, then specify a name and a type for each 
field in the message.

Below is an example of an ``addressbook`` for a person. This section descibes a
simple ``.proto`` file and the corresponding generated chapel code. For complete
details on ``.proto`` files see the links at the end of this document.

.. note::

    The Chapel implementation only supports the `proto3`_ syntax of protobuf.

The ``.proto`` file starts with an optional package declaration, which helps to prevent
naming conflicts between different projects.

.. code-block:: proto

  syntax = "proto3";
  package addressbook;

In Chapel the generated records will be placed in a module matching the ``package``
name. If the ``package`` name is not specified the module takes the name of the
proto file with all non-alphanumeric characters replaced by an ``underscore``.

Next, you have to define your messages. A message is just an aggregate containing
a set of typed fields. Many standard simple data types are available as field types,
including int64, int32, float, double, and string.

.. code-block:: proto

  message Person {
    string name = 1;
    int32 id = 2;  // Unique ID number for this person.
    string email = 3;

    enum PhoneType {
      MOBILE = 0;
      HOME = 1;
      WORK = 2;
    }

    message PhoneNumber {
      string number = 1;
      PhoneType phntype = 2;
    }

    repeated PhoneNumber phones = 4;

  }
  
In the above example the ``Person`` message contains a ``string`` typed ``name``
field with field number ``1``, a ``int32`` typed ``id`` field with field number
``2`` as well as a ``string`` typed ``email`` field with field number ``3``. You
can also define enum types if you want one of your fields to have one of a
predefined list of values - here you want to specify that a phone type can be
one of MOBILE, HOME, or WORK. You can also define nested messages and use these
as field types, like the ``PhoneNumber`` message containing a ``string`` typed
``number`` field with field number ``1`` and a ``phntype`` enum field with field
number ``2``. The ``phones`` field with field number ``4`` is an example of a repeated message
field. If a field is repeated, the field may be repeated any number of times
(including zero). The order of the repeated values will be preserved in the protocol
buffer. If a field is not set, a default value is assigned to the field by Chapel.

Compiling your protocol buffers
===============================

The code generator is integrated with the protoc compiler toolchain
included in the default Protocol Buffers distribution. Use the ``protoc`` command
with the ``--chpl_out`` flag to invoke the Chapel code generator and write the
output ``chpl`` file to a specific location.

.. code-block:: console

  protoc --chpl_out=$DST_DIR $SRC_DIR/addressbook.proto

This generates ``addressbook.chpl`` in your specified directory.


The generated file
==================

The generated ``addressbook.chpl`` file will contain:

* A wrapper module with the name ``addressbook``.
* ``Person`` record with ``name``, ``id``, ``email`` and ``phones`` fields.
* ``Person_PhoneNumber`` record with ``number`` and ``phntype`` fields.
* An enum with the name ``Person_PhoneType``.
* ``serialize`` and ``deserialize`` functions for serialization/parsing.

You can import this module to a ``chpl`` file and can create an instance of ``Person``
for populating data;

.. code-block:: chpl

  use addressbook;
  use IO;

  var messageObj: Person;
  messageObj.name = "John";
  messageObj.id = 429496729;
  messageObj.email = "John@a.com";

  var phoneNumber1: Person_PhoneNumber;
  phoneNumber1.number = "555-4321";
  phoneNumber1.phntype = Person_PhoneType.HOME;
  messageObj.phones.append(phoneNumber1);
  
  var phoneNumber2: Person_PhoneNumber;
  phoneNumber2.number = "555-4444";
  phoneNumber2.phntype = Person_PhoneType.WORK;
  messageObj.phones.append(phoneNumber2);

Serialization and parsing
=========================

The whole purpose of using protocol buffers is to serialize your data so that it
can be parsed elsewhere. You can serialize your message object using the 
``IO`` module and the ``serialize`` function.

.. code-block:: chpl
  
  var file = open("out", iomode.cw);
  var writingChannel = file.writer();

  messageObj.serialize(writingChannel);
  
Parsing is also similar, each generated record has a ``deserialize``
function. So to parse the file we have just created we can use:

.. code-block:: chpl

  use addressbook;
  use IO;
  
  var file = open("out", iomode.r);
  var readingChannel = file.reader();
  
  var messageObj = new Person();
  messageObj.deserialize(readingChannel);


Features
========

The following features are currently supported

#. `Message definitions`_
#. `Scalar value types`_
#. `Unknown fields`_
#. `Packages`_
#. `Enumerations`_
#. `Repeated fields`_
#. `Nested types`_
#. `Using other message types`_
#. `Any message type`_
#. `Oneofs`_
#. `Maps`_


.. _Protocol Buffers: https://developers.google.com/protocol-buffers
.. _proto3: https://developers.google.com/protocol-buffers/docs/proto3
.. _guide: https://github.com/protocolbuffers/protobuf#protocol-compiler-installation
.. _Message definitions: https://developers.google.com/protocol-buffers/docs/proto3#simple
.. _Scalar value types: https://developers.google.com/protocol-buffers/docs/proto3#scalar
.. _Unknown fields: https://developers.google.com/protocol-buffers/docs/proto3#unknowns
.. _Packages: https://developers.google.com/protocol-buffers/docs/proto3#packages
.. _Enumerations: https://developers.google.com/protocol-buffers/docs/proto3#enum
.. _Repeated fields: https://developers.google.com/protocol-buffers/docs/proto3#specifying_field_rules
.. _Nested types: https://developers.google.com/protocol-buffers/docs/proto3#nested
.. _Using other message types: https://developers.google.com/protocol-buffers/docs/proto3#other
.. _Any message type: https://developers.google.com/protocol-buffers/docs/proto3#any
.. _Oneofs: https://developers.google.com/protocol-buffers/docs/proto3#oneof
.. _Maps: https://developers.google.com/protocol-buffers/docs/proto3#maps
