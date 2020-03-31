.. default-domain:: chpl

.. module:: SysError
   :synopsis: This module helps handle system errors.

SysError
========
**Usage**

.. code-block:: chapel

   use SysError;



This module helps handle system errors.

This module defines the type ``syserr``, which can encode an error code or
error message and be returned from routines generating an error. Additional
routines are provided to print a useful error message from a ``syserr``.

The IO module uses these routines in a way that supports error inspection
and rapid prototyping. Most routines in the IO module have two forms.
In one form, a :class:`SystemError` is thrown when an error occurs.
In the second form, a ``syserr`` is returned in an out error argument.



.. class:: SystemError : Error

   
   
   :class:`SystemError` is a base class for :class:`ChapelError.Error` s
   generated from ``syserr``. It provides factory methods to create different
   subtypes based on the ``syserr`` that is passed.
   


   .. attribute:: var err: syserr

   .. attribute:: var details: string

   .. method:: proc init(err: syserr, details: string = "")

   .. method:: override proc message()

      
      Provides a formatted string output for :class:`SystemError`, generated
      from the internal ``err`` and the ``details`` string.
      

   .. method:: proc type fromSyserr(err: syserr, details: string = "")

      
      Return the matching :class:`SystemError` subtype for a given ``syserr``,
      with an optional string containing extra details.
      
      :arg err: the syserr to generate from
      :arg details: extra information to include with the error
      

   .. method:: proc type fromSyserr(err: int, details: string = "")

      
      Return the matching :class:`SystemError` subtype for a given error number,
      with an optional string containing extra details.
      
      :arg err: the number to generate from
      :arg details: extra information to include with the error
      

.. class:: BlockingIOError : SystemError

   
   
   :class:`BlockingIOError` is the subclass of :class:`SystemError`
   corresponding to :const:`SysBasic.EAGAIN`, :const:`SysBasic.EALREADY`,
   :const:`SysBasic.EWOULDBLOCK`, and :const:`SysBasic.EINPROGRESS`.
   


   .. method:: proc init(details: string = "", err: syserr = EWOULDBLOCK: syserr)

.. class:: ChildProcessError : SystemError

   
   
   :class:`ChildProcessError` is the subclass of :class:`SystemError`
   corresponding to :const:`SysBasic.ECHILD`.
   


   .. method:: proc init(details: string = "", err: syserr = ECHILD: syserr)

.. class:: ConnectionError : SystemError

   
   
   :class:`ConnectionError` is the subclass of :class:`SystemError` that serves
   as the base class for all system errors regarding connections.
   


   .. method:: proc init(err: syserr, details: string = "")

.. class:: BrokenPipeError : ConnectionError

   
   
   :class:`BrokenPipeError` is the subclass of :class:`ConnectionError`
   corresponding to :const:`SysBasic.EPIPE` and :const:`SysBasic.ESHUTDOWN`.
   


   .. method:: proc init(details: string = "", err: syserr = EPIPE: syserr)

.. class:: ConnectionAbortedError : ConnectionError

   
   
   :class:`ConnectionAbortedError` is the subclass of :class:`ConnectionError`
   corresponding to :const:`SysBasic.ECONNABORTED`.
   


   .. method:: proc init(details: string = "", err: syserr = ECONNABORTED: syserr)

.. class:: ConnectionRefusedError : ConnectionError

   
   
   :class:`ConnectionRefusedError` is the subclass of :class:`ConnectionError`
   corresponding to :const:`SysBasic.ECONNREFUSED`.
   


   .. method:: proc init(details: string = "", err: syserr = ECONNREFUSED: syserr)

.. class:: ConnectionResetError : ConnectionError

   
   
   :class:`ConnectionResetError` is the subclass of :class:`ConnectionError`
   corresponding to :const:`SysBasic.ECONNRESET`.
   


   .. method:: proc init(details: string = "", err: syserr = ECONNRESET: syserr)

.. class:: FileExistsError : SystemError

   
   
   :class:`FileExistsError` is the subclass of :class:`SystemError`
   corresponding to :const:`SysBasic.EEXIST`.
   


   .. method:: proc init(details: string = "", err: syserr = EEXIST: syserr)

.. class:: FileNotFoundError : SystemError

   
   
   :class:`FileNotFoundError` is the subclass of :class:`SystemError`
   corresponding to :const:`SysBasic.ENOENT`.
   


   .. method:: proc init(details: string = "", err: syserr = ENOENT: syserr)

.. class:: InterruptedError : SystemError

   
   
   :class:`InterruptedError` is the subclass of :class:`SystemError`
   corresponding to :const:`SysBasic.EINTR`.
   


   .. method:: proc init(details: string = "", err: syserr = EINTR: syserr)

.. class:: IsADirectoryError : SystemError

   
   
   :class:`IsADirectoryError` is the subclass of :class:`SystemError`
   corresponding to :const:`SysBasic.EISDIR`.
   


   .. method:: proc init(details: string = "", err: syserr = EISDIR: syserr)

.. class:: NotADirectoryError : SystemError

   
   
   :class:`NotADirectoryError` is the subclass of :class:`SystemError`
   corresponding to :const:`SysBasic.ENOTDIR`.
   


   .. method:: proc init(details: string = "", err: syserr = ENOTDIR: syserr)

.. class:: PermissionError : SystemError

   
   
   :class:`PermissionError` is the subclass of :class:`SystemError`
   corresponding to :const:`SysBasic.EACCES` and :const:`SysBasic.EPERM`.
   


   .. method:: proc init(details: string = "", err: syserr = EPERM: syserr)

.. class:: ProcessLookupError : SystemError

   
   
   :class:`ProcessLookupError` is the subclass of :class:`SystemError`
   corresponding to :const:`SysBasic.ESRCH`.
   


   .. method:: proc init(details: string = "", err: syserr = ESRCH: syserr)

.. class:: TimeoutError : SystemError

   
   
   :class:`TimeoutError` is the subclass of :class:`SystemError` corresponding
   to :const:`SysBasic.ETIMEDOUT`.
   


   .. method:: proc init(details: string = "", err: syserr = ETIMEDOUT: syserr)

.. class:: IOError : SystemError

   
   
   :class:`IOError` is the subclass of :class:`SystemError` that serves as the
   base class for all errors regarding inputs and their formatting.
   They are typically not directly generated by the OS, but they are
   used and emitted by the IO module.
   


   .. method:: proc init(err: syserr, details: string = "")

.. class:: EOFError : IOError

   
   
   :class:`EOFError` is the subclass of :class:`IOError` corresponding to
   :const:`SysBasic.EEOF`.
   


   .. method:: proc init(details: string = "", err: syserr = EEOF: syserr)

.. class:: UnexpectedEOFError : IOError

   
   
   :class:`UnexpectedEOFError` is the subclass of :class:`IOError`
   corresponding to :const:`SysBasic.ESHORT`.
   


   .. method:: proc init(details: string = "", err: syserr = ESHORT: syserr)

.. class:: BadFormatError : IOError

   
   
   :class:`BadFormatError` is the subclass of :class:`IOError` corresponding
   to :const:`SysBasic.EFORMAT`.
   


   .. method:: proc init(details: string = "", err: syserr = EFORMAT: syserr)

.. function:: proc ioerror(error: syserr, msg: string, path: string, offset: int(64)) throws

   Create and throw a :class:`SystemError` if an error occurred, formatting a
   useful message based on the provided arguments. Do nothing if the error
   argument does not indicate an error occurred.
   
   :arg error: the error code
   :arg msg: extra information to include in the thrown error
   :arg path: optionally, a path to include in the thrown error
   :arg offset: optionally, an offset to include in the thrown error
   
   :throws SystemError: A subtype is thrown when the error argument indicates an
                        error occurred
   

.. function:: proc ioerror(errstr: string, msg: string, path: string, offset: int(64)) throws

   Create and throw an :class:`IOError` and include a formatted message based on
   the provided arguments.
   
   :arg errstr: the error string
   :arg msg: extra information to print after the error description
   :arg path: a path to print out that is related to the error
   :arg offset: an offset to print out that is related to the error
   
   :throws IOError: always throws an IOError
   

.. function:: proc errorToString(error: syserr): string

   Convert a syserr code to a human-readable string describing the error.
   
   :arg error: the error code
   :returns: a string describing the error
   

