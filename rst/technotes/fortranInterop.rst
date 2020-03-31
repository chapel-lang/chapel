.. _readme-fortranInterop:

.. default-domain:: chpl

========================
Fortran Interoperability
========================

This README describes support for calling Chapel functions from Fortran code
and vice-versa.

.. note::

    The features described here are still a work in progress.

Calling Fortran from Chapel
===========================

The process for calling Fortran procedures and subroutines from Chapel is
similar to the process for calling C functions with the explicit strategy
described in :ref:`C Interoperability <readme-extern>`.

The procedures or subroutines that will be called are declared in the Chapel
code using :ref:`extern declarations <readme-extern-extern-declarations>`.
The Fortran code containing the procedures and subroutines should be compiled
into an object file using a fortran compiler. Then the object file can be
included in the Chapel compiler command line. Since the generated code will
be linked with a C compiler, it is necessary to specify the library path for
the Fortran runtime library.

.. code-block:: bash

    chpl fortranObjs.o chapelCode.chpl -LpathToFortranLib -lgfortran

Calling Chapel from Fortran
===========================

Chapel functions declared as ``export`` can be called from Fortran by using
the ``--library-fortran`` flag as described in
:ref:`Using your Library in Fortran<readme-libraries.Fortran>`
