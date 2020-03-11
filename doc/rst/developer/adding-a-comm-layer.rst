=============================
Adding Communication Layer
=============================
This document briefly describes how to add a new communication layer
to the runtime.

To implement a new comm layer called ``speedy``, add ``comm/speedy`` to the
build as described in ``adding-a-runtime-component.rst``.

The comm layer is responsible for setting the linker (LD) for the
generated code, and should do so in ``runtime/etc/Makefile.comm-speedy``.
Typically LD should be set to ``CXX`` or a ``C++-compatible`` linker in order
to ensure that standard C++ libraries are linked in to support
third-party packages that may use C++ (like ``re2``).

The following header files define the comm layer API and also serve as
documentation for the core API:

- ``runtime/include/chpl-comm.h``
- *runtime/include/chpl-comm-locales.h*

The APIs should be should be implemented in ``runtime/comm/speedy``.  The
filenames should follow the general naming convention, which is to
start with comm-speedy.  Specifically,

- **runtime/src/comm/speedy/comm-speedy.c**: 
  
  For APIs defined in ``chpl-comm.h``

- **runtime/src/comm/speedy/comm-speedy-locales.c**: 

  For APIs defined in ``chpl-comm-locales.h``

Additional C files can be used, but their names should also start with
comm-speedy.

The following header files define optional APIs that may be useful for
a comm layer implementation.

- **runtime/include/chpl-comm-launch.h**: 

  Defines the macro ``CHPL_COMM_PRELAUNCH`` to be executed before launching the user
  program.

- **runtime/include/comm/chpl-comm-heap-macros.h**: 

  Defines the macro ``CHPL_HEAP_REGISTER_GLOBAL_VAR_EXTRA`` which is used to do anything special needed to register global heap variables.

- **runtime/include/comm/chpl-comm-impl.h**: 

  This file contains any additional runtime interface declarations that need to be provided by the specific comm layer implementation.

- **runtime/include/comm/chpl-comm-task-decls.h**: 

  Defines data structures needed for non-blocking communication and remote value caching.

These are default (``no-op``) versions of the implementation which will be
used if no comm layer specific versions are defined.

If any of the above are desirable for the speedy comm layer, define
the appropriate data structures or macros in a file with the same name
as the default version and place it in ``runtime/include/comm/speedy``.
