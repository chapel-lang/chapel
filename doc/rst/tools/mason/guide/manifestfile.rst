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
    version = "0.1.0"

    [dependencies]
    curl = '1.0.0'

The ``chplVersion`` field indicates Chapel releases compatible with this
package. There are a number of accepted formats:

.. code-block:: text

    "1.16.0"         # 1.16.0 or later
    "1.16"           # 1.16.0 or later
    "1.16.0..1.19.0" # 1.16 through 1.19, inclusive

By default, ``chplVersion`` is set to represent the current Chapel release or
later. For example, if you are using the 1.16 release, chplVersion will be
``1.16.0``.

The ``license`` field indicates the software license under which the package is distributed.
Any of the licenses available at the `SPDX License List <https://spdx.org/licenses/>`_ can be used for Mason packages.
The license field defaults to ``None``.

TOML is the configuration language chosen by the chapel team for
configuring programs written in chapel. A TOML file contains the
necessary information to build a chapel program using mason.
View documentation for the TOML format here: `TOML Spec <https://github.com/toml-lang/toml>`_.

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
     version = "0.1.0"


     [curl]
     chplVersion = "1.16.0..1.16.0"
     license = "None"
     name = 'curl'
     source = 'https://github.com/username/curl'
     version = '1.0.0'


