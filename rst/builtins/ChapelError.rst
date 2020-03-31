.. default-domain:: chpl

.. module:: ChapelError
   :synopsis: :class:`Error` is the parent type for errors in Chapel.  :class:`TaskErrors`

Errors
======
:class:`Error` is the parent type for errors in Chapel.  :class:`TaskErrors`
is the type of errors thrown from parallel constructs where more than one
error can be simultaneously encountered.



.. class:: Error

   :class:`Error` is the base class for errors 


   .. method:: proc init()

      Construct an Error 

   .. method:: proc init(msg: string)

      Construct an :class:`Error` with a message. 

   .. method:: proc message()

      Override this method to provide an error message
      in case the error is printed out or never caught.
      

.. class:: NilThrownError : Error

   
   If a `nil` :class:`Error` is thrown, :class:`NilThrownError`
   will be thrown instead.
   


.. class:: NilClassError : Error

.. class:: ClassCastError : Error

.. class:: DecodeError : Error

   
   A `DecodeError` is thrown if an attempt to create a string with non-UTF8 byte
   sequences are made at runtime. This includes calling the
   `bytes.decode(decodePolicy.strict)` method on a bytes with non-UTF8 byte
   sequences.
   


.. class:: IllegalArgumentError : Error

   .. method:: proc init()

   .. method:: proc init(info: string)

   .. method:: proc init(formal: string, info: string)

.. class:: TaskErrors : Error

   
   :class:`TaskErrors` stores multiple errors when they can come up.
   For example, a ``coforall`` loop might throw errors from multiple
   tasks at the same time. These errors will be reported to the parent
   task at the end of the ``coforall`` in the form of
   :class:`TaskErrors`.
   
   Note that errors thrown within a ``coforall``, ``cobegin``, or
   ``forall`` are represented as elements of ``TaskErrors``. In the
   case of nesting, all errors will be stored in a single ``TaskErrors``.
   
   Errors thrown in ``begin`` tasks will be reported within a ``TaskErrors``
   to the task that uses ``sync`` to wait for those ``begin`` tasks.
   
   


   .. method:: proc init(err: unmanaged Error)

      Create a :class:`TaskErrors` containing only the passed error 

   .. method:: proc init()

      Create a :class:`TaskErrors` not containing any errors 

   .. method:: proc deinit()

   .. itermethod:: iter these() ref: owned nilable Error

      Iterate over the errors contained in this :class:`TaskErrors`.
      For example
      
        .. code-block:: chapel
      
          var taskErrors:TaskErrors = ...;
          for containedError in taskErrors {
            // Do something with the contained error
          }
      
      Yields references to ``owned Error?`` so that one of the
      yielded errors might be re-thrown. Only yields values
      that are not storing ``nil`` at the time of the call.
      

   .. method:: proc first() ref: owned nilable Error

      Returns the first non-nil error contained in this TaskErrors group 

   .. method:: override proc message(): string

      
      
      Returns a string summarizing the errors contained in this
      :class:`TaskErrors`. The summary is intended to be concise: it will not
      grow arbitrarily long if the :class:`TaskErrors` contains many errors.
      
      

   .. itermethod:: iter filter(type t) ref: owned nilable Error

      
      Iterate over those errors contained that are the passed type
      or a subclass of that type.
      
      Note that this iterator yields values of type ``owned Error?``
      but only those that are non-nil and have dynamic type ``t``.
      

   .. method:: proc contains(type t)

      Returns `true` if this :class:`TaskErrors` contains an error
      of the given type or a subclass of that type. 

