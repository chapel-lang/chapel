:title: Mason Guide: *Building Code in Other Languages*

Building Code in Other Languages
================================

Some Mason packages may include code in languages other than Chapel. A common
scenario for this is when the package needs to interoperate with other
languages. The code in other languages in such a scenario is called
"prerequisites". Chapel 2.7 release includes Mason with initial support for
prerequisites.

Understanding Prerequisites
~~~~~~~~~~~~~~~~~~~~~~~~~~~
While they are similar to dependencies, prerequisites are an inherent part of
the Mason package whereas dependencies need to be built and installed before
Mason can start building a package. Consider a scenario in which your Mason
package has a dependency on a C or C++ library, and that you need to implement a
shim between that library and the Chapel code. That shim needs to be compiled
before the Chapel code and needs to be linked. Obviously, the Chapel compiler
cannot be used in this case. Moreover, depending on the library, build process
can be completely different from one scenario to another.

Mason can use other tools to build such code that is part of your package.

.. warning::

   As of Chapel 2.7, prerequisite support is prototypical. We plan to expand
   the capabilities and make the implementation more robust in future releases.
   If you encounter any issue, or have a feature request related to
   prerequisites, please `create an issue <https://github.com/chapel-lang/chapel/issues/new?template=01-feature_request.md>`_.



A "Hello, world!" Example
~~~~~~~~~~~~~~~~~~~~~~~~~

Consider a Mason application where "Hello, world!" is printed through a C
function that Chapel calls. The key source files in this scenario are:


* Hello.chpl

.. code-block:: chapel

   module prereq1 {
     extern proc hello();

     proc main() {
       hello();
     }
   }

* hello.h

.. code-block:: c
 
   void hello(void);

* hello.c

.. code-block:: c
 
   #include <stdio.h>

   #include "hello.h"
  
   void hello(void) {
     printf("Hello world!\n");
   }

As of Chapel 2.7, building this project with Mason requires:

* A ``prereqs`` directory at the project root,

* Each prerequisite to have a subdirectory within ``prereqs``,

* Within each prerequisite's directory a ``Makefile``,

* Each ``Makefile`` to support two targets

    * The default target, invoked by ``make``: To build the external code into
      something that Chapel compiler link against, typically an object file

    * The ``printchplflags`` target, invoked by ``make printchplflags``: To
      print flags and files to be passed to ``chpl`` for this
      prerequisite to be linked with the Chapel application or library.

Based on these requirements, the file structure should look like:


.. code-block:: none

   Hello/
   ├── src/
   │   └── Hello.chpl
   ├── prereqs/
   │   └── some-c-lib/
   │       └── hello.c
   │       └── hello.h
   │       └── Makefile
   └── Mason.toml


Where a simple `Makefile` could look like:

.. code-block:: Makefile

   all:
        gcc -c -fPIE hello.c

   printchplflags:
        @$(info prereqs/some-c-lib/hello.h prereqs/some-c-lib/hello.o)

   clean:  # not needed or used by Mason
        rm -f hello.o

In this setup, `mason build` will roughly do the following:

1. Walk into ``prereqs/some-c-lib``,
2. Run ``make``, which will result in creating the object file
   ``prereqs/some-c-lib/hello.o``,
3. Go back to the project root,
4. Run ``chpl prereqs/some-c-lib/hello.h prereqs/some-c-lib/hello.o src/Hello.chpl``


Special Variables
~~~~~~~~~~~~~~~~~

Mason will pass some information to the prereqs through environment variables.
This allows the prereqs to be more flexible and dynamic. The following list of variables is available:

* ``MASON_PACKAGE_HOME``: The absolute path to the root of the package. In the
  example above, it will be the absolute path to the ``Hello`` directory.

Next Steps
~~~~~~~~~~

This is a new feature added to Mason as of Chapel 2.7 and it is expected to get
more robust and capable. One important direction we want to take is to enable
more custom build process for each prerequisites, rather than prescriptive
nature of the current implementation. `This feature request <https://github.com/chapel-lang/chapel/issues/28174>`_ captures the wish and
has potential directions for it. Please comment under it if you have any
thoughts. If you have other requests on this relatively new feature, please
`create an issue <https://github.com/chapel-lang/chapel/issues/new?template=01-feature_request.md>`_.

