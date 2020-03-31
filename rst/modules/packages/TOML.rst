.. default-domain:: chpl

.. module:: TOML
   :synopsis: Chapel's Library for `Tom's Obvious, Minimal Language (TOML)

TOML
====
**Usage**

.. code-block:: chapel

   use TOML;

**Submodules**

.. toctree::
   :maxdepth: 1
   :glob:

   TOML/*


Chapel's Library for `Tom's Obvious, Minimal Language (TOML)
<https://github.com/toml-lang/toml>`_.
This module provides support for parsing and writing toml files.


  .. note::

    The planned features and known limitations of this module can be found in
    `Improve Toml issue <https://github.com/chapel-lang/chapel/issues/7104>`_.



.. function:: proc parseToml(input: file): unmanaged Toml

   Receives a TOML file as a parameter and outputs a Toml object.
   
   
   .. code-block:: chapel
   
        use TOML;
   
        const tomlFile = open("example.toml", iomode.r);
        const toml = parseToml(tomlFile);
   
   To read tables of a TOML file, use the same syntax as accessing associative arrays. For example,
   to access to the following TOML file's project name,
   
   .. code-block:: yaml
   
        [root]
        name = "example"
        version = "1.0.0"
        author = "Sam Partee"
   
   Use the following code in chapel.
   
   .. code-block:: chapel
   
        use TOML;
   
        const tomlFile = open("example.toml", iomode.r);
        const toml = parseToml(tomlFile);
        const projectName = ["root"]["name"] // returns a TOML object
        writeln(projectName.toString());     // to turn TOML object into string representation
   
   

.. function:: proc parseToml(input: channel): unmanaged Toml

   Receives a channel to a TOML file as a parameter and outputs a Toml object.

.. function:: proc parseToml(input: string): unmanaged Toml

   Receives a string of TOML format as a parameter and outputs a Toml object 

.. class:: TomlError : Error

   .. attribute:: var msg: string

   .. method:: proc init(msg: string)

   .. method:: override proc message()

