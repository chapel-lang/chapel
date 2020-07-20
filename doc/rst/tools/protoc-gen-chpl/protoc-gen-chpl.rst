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
  buffer compiler. It provides arguments to the ``protoc`` compiler so that it knows how to
  generate Chapel specific code for a given ``.proto`` file.
  
* Chapel library: These module has the runtime implementation of protobufs
  in Chapel and provides functionality to serialize messages in wire format.


Installation Instructions
=========================

In ``$CHPL_HOME`` run the following:

.. code-block:: sh

   make protoc-gen-chpl
  
It builds the protoc-gen-chpl binary so that the command line interface can be used.
This installs protoc-gen-chpl in the same place as the chapel compiler (``chpl``) so that
it can be used anywhere in the user's file system.

To remove protobuf suppot, change directory to ``$CHPL_HOME/tools/protoc-gen-chpl`` and run:

.. code-block:: sh

   make clean


Defining protocol format
========================

To make use of the Chapel protobuf implementation you need to start by defining
a ``.proto`` file. The definitions in a ``.proto`` file contain a message for each
data structure you want to serialize, then specify a name and a type for each 
field in the message. Here is an example of an ``addressbook`` for a person.

The ``.proto`` file starts with an optional package declaration, which helps to prevent
naming conflicts between different projects.

.. code-block:: proto

  syntax = "proto3";
  package addressbook;

In Chapel the generated records will be placed in a module matching the ``package``
name. If the ``package`` name is not specified the module takes the name of the
proto file with all non-aplhanumeric characters replaced by an ``underscore``.

Next, you have to define your messages. A message is just an aggregate containing
a set of typed fields. Many standard simple data types are available as field types,
including int64, int32, float, double, and string.

.. code-block:: proto

  enum Contact {
    FAMILY = 0;
    FRIENDS = 1;
    WORK = 2;
  }

  message Person {
    string name = 1;
    int32 id = 2;  // Unique ID number for this person.
    string email = 3;

    Contact contact = 4;

    repeated string number = 5; // Person might have more than one number.
  }
  
In the above example the ``Person`` message contains 3 simple fields, ``string`` type ``name``,
``int32`` type ``id`` and ``string`` type ``email``. You can also define enum types if
you want one of your fields to have one of a predefined list of values - here you want
to specify that a contact type can be one of FAMILY, FRIENDS, or WORK. The ``Contact`` type
``contact`` is an example. If a field is repeated, the field may be repeated any number
of times (including zero). For example, the ``string`` type ``number`` field. The order of
the repeated values will be preserved in the protocol buffer. If a field is not set, a
default value is assigned to the field by Chapel.


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
* A record with the name ``Person``.
* An enum with the name ``Contact``.
* ``name_``, ``id_``, ``email_``, ``contact_`` and ``number_`` field initializers.
* ``writeToOutputFile`` and ``parseFromInputFile`` functions for serialization/parsing.

You can import this module to a ``chpl`` file and can create an instance of ``Person``
for populating data.

.. code-block:: chpl

  use addressbook;
  use IO;

  var messageObj = new Person();
  messageObj.name = "John";
  messageObj.id = 429496729;
  messageObj.email = "John@a.com";

  messageObj.contact = Contact.WORK;
  
  messageObj.number.append("555-4321");
  messageObj.number.append("555-7890");


Serialization and parsing
=========================

The whole purpose of using protocol buffers is to serialize your data so that it
can be parsed elsewhere. You can serialize your message object using the 
``IO`` module and the ``writeToOutputFile`` function.

.. code-block:: chpl
  
  var file = open("out", iomode.cw);
  var writingChannel = file.writer();

  messageObj.writeToOutputFile(writingChannel);
  
Parsing is also similar, each generated record has a ``parseFromInputFile``
function. So to parse the file we have just created we can use:

.. code-block:: chpl

  use addressbook;
  use IO;
  
  var file = open("out", iomode.r);
  var readingChannel = file.reader();
  
  var messageObj = new Person();
  messageObj.parseFromInputFile(readingChannel);


Features
========

The following features are currently supported

#. `Message definitions`_
#. `Scalar value types`_
#. `Unknown fields`_
#. `Packages`_
#. `Enumerations`_
#. `Repeated fields`_


.. _Protocol Buffers: https://developers.google.com/protocol-buffers
.. _Message definitions: https://developers.google.com/protocol-buffers/docs/proto3#simple
.. _Scalar value types: https://developers.google.com/protocol-buffers/docs/proto3#scalar
.. _Unknown fields: https://developers.google.com/protocol-buffers/docs/proto3#unknowns
.. _Packages: https://developers.google.com/protocol-buffers/docs/proto3#packages
.. _Enumerations: https://developers.google.com/protocol-buffers/docs/proto3#enum
.. _Repeated fields: https://developers.google.com/protocol-buffers/docs/proto3#specifying_field_rules
