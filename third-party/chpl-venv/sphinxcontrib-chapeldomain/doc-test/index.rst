.. Test Chapel Domain documentation master file, created by
   sphinx-quickstart on Tue Dec  9 21:54:48 2014.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Test Chapel Domain!
===================

Chapel language
---------------

Module 'Foo'
~~~~~~~~~~~~

.. toctree::

   functions
   classes
   BitOps
   Vector
   Containers
   foo
   cpp_test

**Does it work???**

* :chpl:chplref:`chplmodindex`

.. default-domain:: chpl

* :chplref:`Stuff... <chplmodindex>`

.. data:: const constTest

    This is a contant. Use :const:`constTest` for this and that.

.. data:: var varTest

    This is some variable. Use :var:`varTest` for everyday needs. Otherwise,
    use :const:`constTest`...

.. data:: config const n

    Problem size, which can be set on command line with ``--n
    <value>``. :chpl:param:`debugAdvancedIters`.

.. data:: config param debugAdvancedIters:bool

    Default is ``false``.

.. type:: type T = domain(1, int(32), true)

    Alias a particular domain to *T*. See also :chpl:type:`U`.

.. type:: U = int(64)

    I can haz *U*?

.. enum:: enum Color { Red, Yellow, Blue }

Do these XRefs work?
--------------------

* :chpl:const:`n`
* :chpl:data:`n`
* :chpl:param:`debugAdvancedIters`
* :chpl:data:`debugAdvancedIters`
* :chpl:var:`varTest`
* :chpl:data:`varTest`
* :chpl:const:`constTest`
* :chpl:data:`constTest`
* :chpl:type:`T`
* :chpl:data:`T`
* :chpl:type:`U`
* :chpl:data:`U`

CPP tests...
------------

.. cpp:class:: myclass

    My class here!

.. cpp:function:: module::myclass::operator std::vector<std::string>(myclass x, int y)

    Some function that does some c++ thing... See also :cpp:class:`myclass`.

.. cpp:function:: inline hello(std::string x)

    Inline hello world function...


Python tests...
---------------

.. py:data:: pyConstTest

    Stuff about the constant!

.. py:data:: pyVarTest

    Info about :py:data:`pyVarTest` and :py:const:`pyConstTest`...


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
