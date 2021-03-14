Adding Runtime Component
========================

This document briefly describes how to set up the build for a new
Chapel runtime component such as a tasking model or communication
layer.

For the purposes of this document, ``<component>`` will refer to the
specific runtime component, e.g., *tasks* or *comm*, and ``<name>`` will be
the name of the new component, e.g., **qthreads** or **gasnet**.

chplenv
-------

Add any changes or additions to default settings.  For example, if you
are adding a tasking model and it will be the default or only model
for some Chapel target architecture, modify ``util/chplenv/chpl_tasks.py``
to select your tasking layer under the appropriate circumstances.  If
you are adding a comm layer and it requires a substrate value, modify
``util/chplenv/chpl_comm_substrate.py`` to set the default substrate type.

Makefiles
---------

For the various makefiles described below, it's a good idea to start
with a copy of the same makefile for a similar, existing component.

Component-specific:

- **make/<component>/Makefile.<name>**: 

  This makefile should include component-specific flags for building the runtime, e.g.,
  ``RUNTIME_INCLS`` and ``RUNTIME_CFLAGS``.  For tasking layers, you will need to define ``CHPL_MAKE_THREADS`` to specify the appropriate threading model.

The following makefiles are usually clones from an existing component
layer, with the various directory names changed appropriately.

``Makefile.share`` may contain additional build rules for the runtime
components.

- **runtime/src/<component>/<name>/Makefile**: 

  Makefile for the component

- **runtime/src/<component>/<name>/Makefile.include**: 

  Included by the runtime top-level Makefile to define to identify component
  subdirectories and source and object files.

- **runtime/src/<component>/<name>/Makefile.share**: 

  Included by Makefile and Makefile.include.

Generated code:
~~~~~~~~~~~~~~~
- **runtime/etc/Makefile.<component>-<name>**: 

  This makefile includes flags for the Chapel generated code, e.g., ``GEN_LFLAGS``, ``LIBS``.
  
  The ``Makefile.comm-<name>`` file in particular is responsible for setting ``LD``.
