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

When to Leverage Mason for Chapel Builds
----------------------------------------

Mason simplifies the build process for Chapel applications. Here's a
breakdown of when Mason shines and when alternative approaches might
be better suited.

**Ideal Use Cases for Mason:**

1. Pure Chapel Projects: Mason excels at managing the build process for
   applications written entirely in Chapel. If your program relies solely
   on Chapel dependencies and compiles directly from Chapel modules, Mason
   is the perfect tool.
2. External Dependencies with `Spack` or `pkg-config`: Mason integrates
   seamlessly with external dependencies managed by `Spack` or
   `pkg-config`. If your project uses such dependencies, Mason can handle
   them efficiently.

**Considering Alternatives:**

1. Complex Compilation Needs Beyond `chpl`: For Chapel programs requiring
   compilation steps beyond what the Chapel compiler offers, Mason might not
   be the most suitable choice. Consider alternatives like `Make` for greater
   flexibility in crafting intricate build commands. As an example, the
   Arkouda project, a prominent open-source Chapel application, uses
   `Make` to run preparatory Python scripts before the final binary compilation.

**Combining Mason with Make:**

Mason and Make can co-exist effectively. You can leverage Mason for its dependency
management capabilities while employing Make for broader build commands requiring
more control.

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
