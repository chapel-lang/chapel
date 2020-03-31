.. default-domain:: chpl

.. module:: TestError

TestError
=========
**Usage**

.. code-block:: chapel

   use UnitTest.TestError;

.. class:: TestError : Error

   
   :class:`TestError` is a base class.
   


   .. attribute:: var details: string

   .. method:: proc init(details: string = "")

   .. method:: override proc message()

.. class:: AssertionError : TestError

   Assertion Error class. Raised when assert Function Failed


   .. method:: proc init(details: string = "")

.. class:: TestSkipped : TestError

   TestSkipped Error Class. Raised when a test is skipped.


   .. method:: proc init(details: string = "")

.. class:: DependencyFound : TestError

   DependencyFound Error Class. Raised when a all dependency
   of a test are not met.
   


   .. method:: proc init(details: string = "")

.. class:: TestIncorrectNumLocales : TestError

   TestIncorrectNumLocales Error Class. Raised when test is not run with
   expected number of Locales.
   


   .. method:: proc init(details: string = "")

.. class:: UnexpectedLocales : TestError

   UnexpectedLocales Error Class. Raised when test has
   locales with which it can't be run.
   Eg: MaxLocales < MinLocales
   


   .. method:: proc init(details: string = "")

