.. _readme-throwing-initializers:

.. default-domain:: chpl

=====================
Throwing Initializers
=====================

Overview
--------

Initializers (see :ref:`Class_Initializers`) and error handling (see
:ref:`Chapter-Error_Handling`) are currently in the process of being integrated
together.  Support for the overlap of these features is currently limited, but
key functionality has been enabled.

Using catch-less try!
~~~~~~~~~~~~~~~~~~~~~




Declaring initializers as throws
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Calling throwing functions
~~~~~~~~~~~~~~~~~~~~~~~~~~


Future Work
-----------

We intend to fully support throwing initializers in the future.  This will
include:

- being able to ``throw`` from anywhere in the body of an initializer
- being able to write ``try`` / ``try!`` with ``catch`` blocks anywhere in the
  body of an initializer
- being able to call functions that ``throw`` prior to ``this.complete()`` calls
  (see :ref:`Limitations_on_Instance_Usage_in_Initializers` for a description)
