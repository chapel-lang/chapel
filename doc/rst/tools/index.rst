.. _tools-index:

Tools
=====

Chapel comes with a variety of tools to help with users write better Chapel
code. All of these are distributed with Chapel in source form and most of them
are distributed with Chapel in binary form as well.

Documentation
-------------

``chpldoc`` is Chapel's documentation generator, which generates API
documentation for your code from inline docstrings. It is also used to generate
the documentation for the Chapel language and standard library.

.. toctree::
   :maxdepth: 1
   :glob:

   chpldoc/chpldoc

Development
-----------

Chapel has several tools to help with Chapel code development. Chapel's
language server (``chpl-language-server``) and linter (``chplcheck``) can help
provide real-time feedback on your code as you write it. Mason is Chapel's
package manager and serves as a tool for building and managing your Chapel
code, as well as distributing your libraries to the community.

.. toctree::
   :maxdepth: 1
   :glob:

   chpl-language-server/chpl-language-server
   chplcheck/chplcheck
   mason/mason

Debugging
---------

Chapel provides a few tools to help with debugging Chapel code. While Chapel
code can be directly debugged inside of standard tools like LLDB and GDB, this
can become challenging when debugging multilocale code. ``chpl-parallel-dbg``
is a wrapper around lldb that makes it easier to debug multilocale code. When
debugging communication issues, ``chplvis`` can be used to visualize the
communication patterns of your program.

.. toctree::
   :maxdepth: 1
   :glob:

   chpl-parallel-dbg/chpl-parallel-dbg
   chplvis/chplvis

Interoperability
----------------

Chapel provides several tools to help interoperate with other languages.
``c2chapel`` can automatically translate C header files to Chapel ``extern``
declarations, reducing much of the manual boilerplate required to write those
declarations by hand. ``protoc-gen-chpl`` is a plugin to the protocol buffer
compiler that generates Chapel code from ``.proto`` files, allowing you to
easily serialize and deserialize structured data in Chapel.

.. toctree::
   :maxdepth: 1
   :glob:

   c2chapel/c2chapel
   protoc-gen-chpl <protoc-gen-chpl/protoc-gen-chpl>


Other
-----

Chapel provides many other tools as well. ``chapel-py`` is an extremely powerful
tool that provides a Python interface to the Chapel frontend compiler library.
This lets you write custom Python tools that leverage the existing Chapel
compiler infrastructure. Several of the tools mentioned here are powered by
``chapel-py``.

The Unstable Warning Anonymizer is a script written in Chapel that can analyze
the output of your Chapel program to generate reports on the use of unstable
warnings in your code. This is useful for users to share what unstable features
they rely on without sharing any implementation details of their code to Chapel
developers to help prioritize stabilization efforts.

.. toctree::
   :maxdepth: 1
   :glob:

   chapel-py/chapel-py
   Unstable Warning Anonymizer <unstableWarningAnonymizer/unstableWarningAnonymizer>
