.. default-domain:: chpl

.. module:: ZMQ
   :synopsis: Lightweight messaging with ZeroMQ (or ØMQ)

ZMQ
===
**Usage**

.. code-block:: chapel

   use ZMQ;



Lightweight messaging with ZeroMQ (or ØMQ)

This module provides high-level Chapel bindings to the
`ZeroMQ messaging library <https://zeromq.org/>`_.

Dependencies
------------

The ZMQ module in Chapel is dependent on ZeroMQ.  For information on how to
install ZeroMQ, see the
`ZeroMQ installation instructions <https://zeromq.org/download/>`_.

.. note::

   Chapel's support for ZeroMQ is a work in progress and may not (yet) support
   the full functionality of the ZeroMQ C API.

.. note::

   Chapel's ZMQ module was developed for compatibility with ZeroMQ v4.x.

Using ZMQ in Chapel
-------------------

.. _using-contexts:

Contexts
++++++++

In ZeroMQ proper, a `context` is an opaque, thread-safe handle to manage
ØMQ's resources within a process.  Typically, a process will allocate only one
context, although more than one context per process is allowable
`[ref] <http://zguide.zeromq.org/page:all#Getting-the-Context-Right>`__.

In Chapel, a :record:`Context` is a reference-counted wrapper around the
underlying ØMQ context. To create a context, it is sufficient to write:

.. code-block:: chapel

   var context: Context;

.. _using-sockets:

Sockets
+++++++

In ZeroMQ, a `socket` is an opaque handle to an asynchronous, message-based
communication channel that is "typed" to provide one of a series of common
communication patterns (i.e., the socket type).

In Chapel, as with a :record:`Context`, a :record:`Socket` is a
reference-counted wrapper around the underlying ØMQ socket.  Sockets are
created via :proc:`Context.socket()` and maintain a reference to the parent
context, so that the parent context may go out of scope and the context will
not be reclaimed while any sockets are still in use.

.. note::

   As with ØMQ's C API, a :record:`Socket` object is not thread safe.
   That is, a :record:`Socket` object should not be accessed concurrently by
   multiple Chapel tasks.  (This may change in a future ZMQ module.)

A :record:`Socket` may be one of the socket types in the following list of
compatible pairs of socket types
`[ref] <http://zguide.zeromq.org/page:all#Messaging-Patterns>`__:

* :const:`PUB`  and :const:`SUB`
* :const:`REQ`  and :const:`REP`
* :const:`PUSH` and :const:`PULL`
* :const:`PAIR`

.. code-block:: chapel

   // create a PUB socket
   var context: Context;
   var socket = context.socket(ZMQ.PUB);

Sending and Receiving Messages
++++++++++++++++++++++++++++++

Sending a message is as simple as passing the object to send as the argument
to :proc:`Socket.send()`.  Receiving a message requires that the type to be
received be passed as the argument to :proc:`Socket.recv()`.
In either case, if the object sent or type to be received cannot be serialized
by ZMQ, the following error shall be produced at compile time.

.. code-block:: chapel

   // send or receive an int
   var val = 42;
   socket.send(val);
   val = socket.recv(int);

   // error: Type "Foo" is not serializable by ZMQ
   class Foo { var val: int; }
   socket.recv(Foo);

Multilocale Support
+++++++++++++++++++

Chapel's ZMQ module supports multilocale execution of ZeroMQ programs.  The
locale on which the :type:`Context` object is created sets the "home" locale
for all sockets created from this context and all operations performed on the
socket.  For example, a send call on a socket from a locale other than the
home locale will migrate a task to the home locale that will remotely access
the data to send it over the socket.

Examples
--------

Example: "Hello, World"
+++++++++++++++++++++++++

This "Hello, World" example demonstrates a :const:`PUSH`-:const:`PULL` socket
pair in two Chapel programs that exchange a single string message.

.. code-block:: chapel

   // pusher.chpl
   use ZMQ;
   config const to: string = "world!";
   var context: Context;
   var socket = context.socket(ZMQ.PUSH);
   socket.bind("tcp://*:5555");
   socket.send(to);

.. (comment) the above started a nested comment, so here we end it */

.. code-block:: chapel

   // puller.chpl
   use ZMQ;
   var context: Context;
   var socket = context.socket(ZMQ.PULL);
   socket.connect("tcp://localhost:5555");
   writeln("Hello, ", socket.recv(string));

Implementation Notes
--------------------

As noted previously, the ZMQ module is a work in progress.  The implementation
notes below describe some of how the ZMQ module is implemented and how future
versions may expose more features native to ZeroMQ.

Serialization
+++++++++++++

In Chapel, sending or receiving messages is supported for a variety of types.
Primitive numeric types, strings and bytes are supported as the
foundation.  In addition, user-defined ``record`` types may be serialized
automatically as `multipart messages
<http://zguide.zeromq.org/page:all#Multipart-Messages>`_ by internal use of the
:chpl:mod:`Reflection` module.  Currently, the ZMQ module can serialize records
of primitive numeric types, strings, bytes and other serializable records.

.. note::

   The serialization protocol for strings changed in Chapel 1.16 in order to
   support inter-language messaging through ZeroMQ. (See
   :ref:`notes on interoperability <interop>` below.)
   As a result, programs using ZMQ that were compiled by Chapel 1.15 or
   earlier cannot communicate using strings with programs compiled by
   Chapel 1.16 or later, although such communication can be used between
   programs compiled by the same version without issue.

   Prior to Chapel 1.16, ZMQ would send a string as two multipart messages:
   the first sent the length as `int`; the second sent the character array
   (in bytes).  It was identified that this scheme was incompatible with how
   other language bindings for ZeroMQ serialize and send strings.

   As of Chapel 1.16, the ZMQ module uses the C-level ``zmq_msg_send()`` and
   ``zmq_msg_recv()`` API for :proc:`Socket.send()` and :proc:`Socket.recv()`,
   respectively, when transmitting strings.  Further, ZMQ sends the string as
   a single message of only the byte stream of the string's character array.
   (Recall that Chapel's ``string`` type currently only supports ASCII
   strings, not full Unicode strings.)

.. _interop:

Interoperability
++++++++++++++++

The ZeroMQ messaging library has robust support in many programming languages
and Chapel's ZMQ module intends on providing interfaces and serialization
protocols suitable for exchanging data between Chapel and non-Chapel programs.

As an example (and test) of Chapel-Python interoperability over ZeroMQ, the
following sources demonstrate a :const:`REQ`-:const:`REP` socket pair between
a Chapel server and a Python client using the
`PyZMQ Python bindings for ZeroMQ <https://pyzmq.readthedocs.io/en/latest/>`_.

.. literalinclude:: ../../../../test/library/packages/ZMQ/interop-py/server.chpl
   :language: chapel
   :lines: 10-

.. literalinclude:: ../../../../test/library/packages/ZMQ/interop-py/client.py
   :language: python

Tasking-Layer Interaction
+++++++++++++++++++++++++

As noted previously, a :record:`Socket` object is not thread safe.  As the
mapping of Chapel tasks to OS threads is dependent on the Chapel tasking layer
and may be cooperatively scheduled, a :record:`Socket` object should not be
accessed concurrently by multiple Chapel tasks.

The ZMQ module is designed to "play nicely" with the Chapel tasking layer.
While the C-level call ``zmq_send()`` may be a blocking call (depending on
the socket type and flag arguments), it is desirable that a
semantically-blocking call to :proc:`Socket.send()` allow other Chapel tasks
to be scheduled on the OS thread as supported by the tasking layer.
Internally, the ZMQ module uses non-blocking calls to ``zmq_send()`` and
``zmq_recv()`` to transfer data, and yields to the tasking layer via
`chpl_task_yield()` when the call would otherwise block.

Limitations and Future Work
+++++++++++++++++++++++++++

Currently, the ZMQ module does not provide interfaces for working with
`ZeroMQ message objects <http://zguide.zeromq.org/page:all#Working-with-Messages>`_,
`handling errors <http://zguide.zeromq.org/page:all#Handling-Errors-and-ETERM>`_,
or making explicitly non-blocking send/receive calls.  These are
active-but-incomplete areas of work that are intended to be supported in
future Chapel releases.

One interaction of these features is worth noting explicitly: because multipart
messages are used to automatically serialize non-primitive data types (e.g.,
strings and records) and a partially-sent multi-part message cannot be
canceled (except by closing the socket), an explicitly non-blocking send call
that encountered an error in the ZeroMQ library during serialization would not
be in a recoverable state, nor would there be a matching "partial receive".

References
----------

* `ZeroMQ C API Reference <http://api.zeromq.org/>`_
* `ZeroMQ, The Guide <http://zguide.zeromq.org/page:all>`_


.. data:: const PUB = ZMQ_PUB

   
   The publisher socket type for a publish-subscribe messaging pattern.
   

.. data:: const SUB = ZMQ_SUB

   
   The subscriber socket type for a publish-subscribe messaging pattern.
   

.. data:: const REQ = ZMQ_REQ

   
   The requester socket type for a paired request-reply messaging pattern.
   

.. data:: const REP = ZMQ_REP

   
   The replier socket type for a paired request-reply messaging pattern.
   

.. data:: const PUSH = ZMQ_PUSH

   
   The pusher socket type for a pipeline messaging pattern.
   

.. data:: const PULL = ZMQ_PULL

   
   The puller socket type for a pipeline messaging pattern.
   

.. data:: const PAIR = ZMQ_PAIR

   
   The exclusive pair pattern socket type.
   

.. function:: proc version: (int, int, int)

   
   Query the ZMQ library version.
   
   :returns: An `(int,int,int)` tuple of the major, minor, and patch
       version of the ZMQ library.
   

.. record:: Context

   
   A ZeroMQ context. See :ref:`more on using Contexts <using-contexts>`.
   Note that this record contains private fields not listed below.
   


   .. method:: proc init()

      
      Create a ZMQ context.
      

   .. method:: proc socket(sockType: int): Socket

      
      Create a :record:`Socket` of type `sockType` from this context.
      
      :arg sockType: The ZMQ socket type to be created;
          e.g., :const:`PUB`, :const:`PUSH`, etc.
      :type sockType: `int`
      

.. record:: Socket

   
   A ZeroMQ socket. See :ref:`more on using Sockets <using-sockets>`.
   Note that this record contains private fields not listed below.
   


   .. method:: proc close(linger: int = unset)

      
      Close the socket.
      
      :arg linger: Optional argument to specify the linger period for the
          socket prior to closing.  If -1, then the linger period is infinite;
          if non-negative, then the linger period shall be set to the specified
          value (in milliseconds).
      :type linger: `int`
      

   .. method:: proc bind(endpoint: string)

      
      Bind the socket to the specified local `endpoint` and accept incoming
      connections.
      

   .. method:: proc connect(endpoint: string)

      
      Connect the socket to the specified `endpoint`.
      

   .. method:: proc getLastEndpoint(): string throws

      
      Get the last endpoint for the specified socket; see
      `zmq_getsockopt <http://api.zeromq.org/4-0:zmq-getsockopt>`_ under
      ZMQ_LAST_ENDPOINT.
      
      :returns: The last endpoint set, see the link above.
      :rtype: string
      
      :throws ZMQError: Thrown when an error occurs getting the last endpoint.
      

   .. method:: proc getLinger(): c_int throws

      
      Get the linger period for the specified socket; see
      `zmq_getsockopt <http://api.zeromq.org/4-0:zmq-getsockopt>`_ under
      ZMQ_LINGER.
      
      :returns: The linger period for the socket, see the link above.
      :rtype: c_int
      
      :throws ZMQError: Thrown when an error occurs getting the linger.
      

   .. method:: proc setLinger(value: c_int) throws

      
      Set the linger period for the specified socket; see
      `zmq_setsockopt <http://api.zeromq.org/4-0:zmq-setsockopt>`_ under
      ZMQ_LINGER.
      
      :arg value: The new linger period for the socket.
      :type value: c_int
      
      :throws ZMQError: Thrown when an error occurs setting the linger.
      

   .. method:: proc setSubscribe(value: ?T) throws

      
      Set the message filter for the specified ZMQ_SUB socket; see
      `zmq_setsockopt <http://api.zeromq.org/4-0:zmq-setsockopt>`_ under
      ZMQ_SUBSCRIBE.
      
      :arg value: The new message filter for the socket.
      
      :throws ZMQError: Thrown when an error occurs setting the message filter.
      

   .. method:: proc setUnsubscribe(value: ?T) throws

      
      Remove an existing message filter for the specified ZMQ_SUB socket; see
      `zmq_setsockopt <http://api.zeromq.org/4-0:zmq-setsockopt>`_ under
      ZMQ_UNSUBSCRIBE.
      
      :arg value: The message filter to remove from the socket.
      
      :throws ZMQError: Thrown when an error occurs setting the message filter.
      

   .. method:: proc send(data: ?T)

      
      Send an object `data` on a socket.
      
      :arg data: The object to be sent. If `data` is an object whose type
          is not serializable by the ZMQ module, a compile-time error will be
          raised.
      
      :type flags: `int`
      

   .. method:: proc recv(type T): T

      
      Receive an object of type `T` from a socket.
      
      :arg T: The type of the object to be received. If `T` is not
          serializable by the ZMQ module, a compile-time error will be raised.
      
      :returns: An object of type `T`
      

.. class:: ZMQError : Error

   
   A subclass of Error specifically for ZMQ-related errors.
   
   .. warning::
      The design for this subclass is subject to change.  We may look into
      merging it with :class:`~SysError.SystemError`, and/or extend it to have
      subclasses for the various ZMQ-specific failures.
   


   .. attribute:: var strerror: string

   .. method:: proc init(strerror: string)

   .. method:: override proc message()

      
      Provides a formatted string output for :class:`ZMQError` using the value
      that was provided at its creation
      

