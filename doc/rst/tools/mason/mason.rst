.. _readme-mason:

=====
Mason
=====

Overview
--------

This guide on Mason Packages contains everything that you need to
know to hit the ground developing in Chapel using mason, whether
that be to create a library to contribute to the community, use an
existing package, or just to help with the process of building your
Chapel programs.

.. note::

   **Pardon Our Dust**

   This mason guide is a work-in-progress as mason development itself
   is still a work-in-progress.  If you're not already using the `main
   branch <https://chapel-lang.org/docs/main/mason-packages/>`_ of the
   documentation (which tracks real-time GitHub development), you may
   find that it contains a more complete set of sections.  We welcome
   feedback and requests on the guide as we go.


Quick Start Instructions
------------------------

To install mason, run ``make mason`` from your Chapel home directory.

To create a new mason application run: ``mason new <MyAppName>``. This will
create a mason project in a new directory `MyAppName` with the following
structure:

.. code-block:: none

   MyAppName/
   ├── src/
   │   └── MyAppName.chpl
   └── Mason.toml

The app can be run by calling ``mason build`` followed by ``mason run`` from
within `MyAppName`.

Or, to create a mason package that can be used as a dependency in other mason
projects, run: ``mason new <MyPackageName> --lib``. This will create a similar
file structure; however, the primary source file, `MyPackageName.chpl`, won't
have a `main` routine because the library is not intended to be run as a
standalone program:

.. code-block:: none

   MyPackageName/
   ├── src/
   │   └── MyPackageName.chpl
   └── Mason.toml

For either type of mason project, add new dependencies with ``mason add
<package name>``. This will add the new dependency to the list in the `toml`
file. When the project is built, mason will download source files for all
Chapel dependencies using the `Mason-Registry
<https://github.com/chapel-lang/mason-registry>`_, allowing them to be used
in the application or library.

Note that mason also supports numerous other features such as unit testing,
examples, and documentation. See the sections below for more information.


Getting Started with Mason
--------------------------

In this section, you will find more detailed information about installing mason
and a basic HelloWorld example to get you started using mason packages in your
own code.

.. toctree::
   :maxdepth: 1
   :glob:

   start/installation
   start/helloworld
   start/whymason


.. _index-mason-guide:

Mason Guide
-----------

This section aims to explain why a user would want to use mason,
point out what types of users would benefit from using mason, outline
some typical use-cases of mason, and introduce some of the features
that make using mason worthwhile.

.. toctree::
   :maxdepth: 1
   :glob:

   guide/basicusage
   guide/buildingandrunning
   guide/buildinglargerpackages
   guide/runtimearguments
   guide/testing
   guide/examples
   guide/documentation
   guide/chapeldependencies
   guide/gitdependencies
   guide/externaldependencies
   guide/masonregistry
   guide/submitting
   guide/manifestfile
   guide/environmentvariables
   guide/namespacing
