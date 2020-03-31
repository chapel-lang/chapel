.. default-domain:: chpl

.. module:: TomlParser
   :synopsis: Parser module with the Toml class for the Chapel TOML library.

TomlParser
==========
**Usage**

.. code-block:: chapel

   use TOML.TomlParser;


Parser module with the Toml class for the Chapel TOML library.

.. data:: config const debugTomlParser = false

   Prints a line by line output of parsing process 

.. class:: Toml

   
   Class to hold various types parsed from input
   used to recursively hold tables and respective values


   .. method:: proc init()

   .. method:: proc init(s: string)

   .. method:: proc init(A: [?D] unmanaged Toml)

   .. method:: proc init(ld: date)

   .. method:: proc init(ti: time)

   .. method:: proc init(dt: datetime)

   .. method:: proc init(i: int)

   .. method:: proc init(boo: bool)

   .. method:: proc init(re: real)

   .. method:: proc init(arr: [?dom] unmanaged Toml)

   .. method:: proc init(lst: list(unmanaged Toml))

   .. method:: proc init(root: unmanaged Toml)

   .. method:: proc this(tbl: string) ref: unmanaged nilable Toml throws

      Returns the index of the table path given as a parameter 

   .. method:: proc set(tbl: string, toml: unmanaged nilable Toml)

   .. method:: proc set(tbl: string, s: string)

   .. method:: proc set(tbl: string, i: int)

   .. method:: proc set(tbl: string, b: bool)

   .. method:: proc set(tbl: string, r: real)

   .. method:: proc set(tbl: string, ld: date)

   .. method:: proc set(tbl: string, ti: time)

   .. method:: proc set(tbl: string, dt: datetime)

   .. method:: proc set(tbl: string, A: [?D] unmanaged nilable Toml)

   .. method:: proc set(tbl: string, arr: [?dom] unmanaged nilable Toml)

   .. method:: override proc writeThis(f) throws

      Write a Table to channel f in TOML format 

   .. method:: proc writeTOML(f)

      Write a Table to channel f in TOML format 

   .. method:: proc writeJSON(f)

      Write a Table to channel f in JSON format 

   .. method:: proc toString(): string

      
      For the user to write values of a node as follows:
      Toml[key].toString()
      

   .. method:: proc tomlType: string throws

      Return Toml type as a string.
      
      Valid types include:
      
      - *empty*
      - *string*
      - *integer*
      - *float*
      - *boolean*
      - *datetime*
      - *array*
      - *toml* (inline table)
      
      

