:title: Mason Guide: *Manifest*

The Manifest File
=================

The ``Mason.toml`` manifest file is written in TOML (more information on the TOML format below).
Each time a new package is created in Mason a standard TOML file is included in the top-level
directory of the package.

For example, ``Mason.toml``:

.. code-block:: text

    [brick]
    authors = ["Sam Partee <Sam@Partee.com>"]
    chplVersion = "1.16.0"
    license = "None"
    name = "MyPackage"
    type = "application"
    version = "0.1.0"

    [dependencies]
    curl = '1.0.0'


Format
~~~~~~

TOML is the configuration language chosen by the chapel team for
configuring programs written in chapel. A TOML file contains the
necessary information to build a chapel program using mason.
View documentation for the TOML format here: `TOML Spec <https://github.com/toml-lang/toml>`_.

Mason understands specific fields in the TOML file, which are described below.

* ``[brick]``: This is the root table of the manifest file. It contains metadata
  about the package itself.

   * ``name``: The name of the package. This should be a unique identifier for
     the package.

   * ``type``: The type of the package. This can be
     :ref:`application <mason-applications>`, :ref:`library <mason-libraries>`, or
     :ref:`light <mason-lightweight-projects>`.

   * ``version``: The version of the package, following semantic versioning
     (e.g., "0.1.0").

   * ``authors``: A list of authors of the package, formatted as strings.
     Each author can be represented as a string with their name and
     email address.

   * ``chplVersion``: Indicates Chapel releases compatible with this
     package. There are a number of accepted formats:

        .. code-block:: text

           "2.1.0"         # 2.1.0 or later
           "2.1"           # 2.1.0 or later
           "2.1.0..2.4.0" # 2.1 through 2.4, inclusive

     By default, ``chplVersion`` is set to represent the current Chapel release or
     later. For example, if you are using the 2.1 release, chplVersion will be
     ``2.1.0``.

   * ``license``: Indicates the software license under which the package is
     distributed. Any of the licenses available at the
     `SPDX License List <https://spdx.org/licenses/>`_ can be used for
     Mason packages. The license field defaults to ``None``.

   * ``tests``: A list of test files that are part of the package. These files
     will be run when a user executes `mason test`.
     See :ref:`testing-with-mason` for more information.

* ``[dependencies]``: This section lists the dependencies of the package.
  Dependencies can be listed in a few ways.

   * Mason dependencies can be specified by name and version, like
     ``myDep = '1.0.0'``. This specifies that the package depends on the
     `myDep` package at version `1.0.0`.

   * External dependencies are listed in the separate ``[system]`` section,
     which is described below.

   * Git dependencies are specified as an inline subtable with the name of
     the dependency, for example:

        .. code-block:: text

           myDep = { git = "https://github.com/username/myDep" }

     This specifies that the package depends on the `myDep` package located at
     the specified git repository URL. You can also specify a branch or a
     specific revision of the git repository by adding `branch` or `rev` keys,
     respectively. For example:

        .. code-block:: text

           myDep = { git = "https://github.com/username/myDep", branch = "main" }
           myDep = { git = "https://github.com/username/myDep", rev = "43d462682851dd2fed6edf123e8fb699db124183" }

     Instead of using the inline subtable syntax, you can also use the subtable
     syntax to specify git dependencies:

        .. code-block:: text

           [dependencies.myDep]
           git = "https://github.com/username/myDep"
           branch = "main"  # or rev = "43d462682851dd2fed6edf123e8fb699db124183"

     See :ref:`mason-git-dependencies` for more information.

* ``[system]``: This section is used to specify external dependencies that are
  required for the package to build and run. These are specified as key value
  pairs, where the key is the name of the system dependency and the value is
  the version or other relevant information. For example:

    .. code-block:: text

       [system]
       cmake = "3.18"

  See :ref:`mason-external-dependencies` for more information on how to
  specify external dependencies.

* ``[examples]``: This section is used to specify example files that are
  part of the package.

   * ``examples``: A list of example files that are part of the package.
     Files are listed relative to the ``example/`` directory
     in the package root.

* ``[examples.<example_name>]``: This section is used to specify compile
  time and runtime arguments for a specific example file. ``<example_name>``
  should be the name of the example module (this is the name of the file
  without the path and without the ``.chpl`` extension).

   * ``compopts``: A list of compile time arguments to be passed to the Chapel
     compiler when building the example.

   * ``execopts``: A list of runtime arguments to be passed to the example when
     it is run.

  See :ref:`mason-examples` for more information.


Any other sections or fields in the TOML file will be ignored by Mason.

The Lock File
=============

The lock file ``Mason.lock`` is generated after running a ``mason update`` command. The user should
never manually edit the lock file as it is intended to "lock" in the settings of a certain
package build iteration. ``Mason.lock`` is added by default to the .gitignore when a new package
is created. If your intention is to create a binary application package thats lock file should not
change, removing ``Mason.lock`` from your ``.gitignore`` could be a good idea. An example of
a lock file is written below as if generated from the earlier example of a ``Mason.toml``:


.. code-block:: text

     [root]
     authors = ["Sam Partee <Sam@Partee.com>"]
     chplVersion = "1.16.0..1.16.0"
     dependencies = ['curl 1.0.0 https://github.com/username/curl']
     license = "None"
     name = "MyPackage"
     source = "https://github.com/Spartee/MyPackage"
     type = "application"
     version = "0.1.0"


     [curl]
     chplVersion = "1.16.0..1.16.0"
     license = "None"
     name = 'curl'
     source = 'https://github.com/username/curl'
     version = '1.0.0'


