.. default-domain:: chpl

.. module:: Path
   :synopsis: A file utilities library, specifically related to path operations

Path
====
**Usage**

.. code-block:: chapel

   use Path;

A file utilities library, specifically related to path operations

The Path module focuses on manipulation of the path to a file or directory.
Also provided are constant values representing common idioms that may vary
across operating systems (though rarely in the modern era), such as general
references to a parent directory or the current directory.

.. note::

   This module is currently missing the implementation for `expandUser
   <https://github.com/chapel-lang/chapel/issues/6008>`_, `normCase
   <https://github.com/chapel-lang/chapel/issues/6013>`_, Once those are
   implemented, it will be considered complete.

Operations which occur on the files or directories referred to by these paths
may be found in :mod:`FileSystem` (for operations *on* the file) or :mod:`IO`
(for operations *within* the file).

Path Computations
-----------------
:proc:`commonPath`
:proc:`normPath`
:proc:`realPath`
:proc:`file.realPath`
:proc:`relPath`
:proc:`file.relPath`

Path Manipulations
------------------
:proc:`absPath`
:proc:`file.absPath`
:proc:`expandVars`
:proc:`joinPath`
:proc:`splitPath`

Path Properties
---------------
:proc:`basename`
:proc:`dirname`
:proc:`file.getParentName`
:proc:`isAbsPath`

Constant and Function Definitions
---------------------------------

.. data:: const curDir = "."

   Represents generally the current directory. This starts as the directory
   where the program is being executed from.

.. data:: const parentDir = ".."

   Represents generally the parent directory. 

.. data:: const pathSep = "/"

   Denotes the separator between a directory and its child. 

.. function:: proc absPath(name: string): string throws

   
   Creates a normalized absolutized version of a path. On most platforms, when
   given a non-absolute path this function is equivalent to the following code:
   
   .. code-block:: Chapel
   
     normPath(joinPath(here.cwd(), name))
   
   See :proc:`normPath()`, :proc:`joinPath()`, :proc:`~FileSystem.locale.cwd()`
   for details.
     
   .. warning::
   
     This function is unsafe for use in a parallel environment due to its
     reliance on :proc:`~FileSystem.locale.cwd()`. Another task on the current
     locale may change the current working directory at any time.
   
   :arg name: The path whose absolute path is desired.
   :type name: `string`
   
   :return: A normalized, absolutized version of the path specified.
   :rtype: `string`
   
   :throws SystemError: Upon failure to get the current working directory.

.. method:: proc file.absPath(): string throws

   
   Creates a normalized absolutized version of the path in this
   :type:`~IO.file`. On most platforms, when given a non-absolute path this
   function is equivalent to the following code:
   
   .. code-block:: Chapel
   
       normPath(joinPath(here.cwd(), file.path))
       
   See :proc:`normPath()`, :proc:`joinPath()`, :proc:`~FileSystem.locale.cwd()`,
   :proc:`~IO.file.path` for details.
   
   .. warning::
   
     This method is unsafe for use in a parallel environment due to its
     reliance on :proc:`~FileSystem.locale.cwd()`. Another task on the current
     locale may change the current working directory at any time.
   
   :return: A normalized, absolutized version of the path for this file.
   :rtype: `string`
   
   :throws SystemError: Upon failure to get the current working directory.

.. function:: proc basename(name: string): string

   Returns the basename of the file name provided.  For instance:
   
   .. code-block:: Chapel
   
      writeln(basename("/foo/bar/baz")); // Prints "baz"
      writeln(basename("/foo/bar/")); // Prints "", because of the empty string
   
   Note that this is different from the Unix basename function.
   
   :arg name: A string file name.  Note that this string does not have to be
              a valid file name, as the file itself will not be affected.
   :type name: `string`

.. function:: proc commonPath(paths: string ...?n): string

   Determines and returns the longest common path prefix of
   all the string pathnames provided.
   
   :arg paths: Any number of paths.
   :type paths: `string`
   
   :return: The longest common path prefix.
   :rtype: `string`

.. function:: proc commonPath(paths: []): string

   Determines and returns the longest common path prefix of
   all the string pathnames provided.
   
   :arg paths: Any number of paths as an array.
   :type paths: `array`
   
   :return: The longest common path prefix.
   :rtype: `string`

.. function:: proc dirname(name: string): string

   Returns the parent directory of the file name provided.  For instance:
   
   .. code-block:: Chapel
   
      writeln(dirname("/foo/bar/baz")); // Prints "/foo/bar"
      writeln(dirname("/foo/bar/")); // Also prints "/foo/bar"
   
   :arg name: A string file name.  Note that this string does not have to be
              a valid file name, as the file itself will not be affected.
   :type name: `string`

.. function:: proc expandVars(path: string): string

   Expands any environment variables in the path of the form ``$<name>`` or
   ``${<name>}`` into their values.  If ``<name>`` does not exist, they are left
   in place. Returns the path which includes these expansions.
   
   :arg path: A string representation of a path, which may or may not include
              ``$<name>`` or ``${<name>}``.
   :type path: `string`
   
   :return: `path`, having replaced all references to environment variables with
            their values.
   :rtype: `string`

.. method:: proc file.getParentName(): string throws

   
   Returns the parent directory of the :type:`~IO.file` record.  For instance:
   
   .. code-block:: Chapel
   
      var myFile = open("/foo/bar/baz.txt", iomode.r);
      writeln(myFile.getParentName()); // Prints "/foo/bar"
   
   :return: The parent directory of the file.
   :rtype: `string`
   :throws SystemError: If one occurs.

.. function:: proc isAbsPath(name: string): bool

   Determines whether the path specified is an absolute path.
   
   .. note::
   
      This is currently only implemented in a Unix environment.  It will not
      behave correctly in a non-Unix environment.
   
   :arg name: The path to be checked.
   :type name: `string`
   
   :return: `true` if `name` is an absolute path, `false` otherwise.
   :rtype: `bool`

.. function:: proc joinPath(paths: string ...?n): string

   Join and return one or more paths, putting precedent on the last absolute
   path seen.  Return value is the concatenation of the paths with one
   directory separator following each non-empty argument except the last.
   Examples:
   
   .. code-block:: chapel
   
      writeln(joinPath("/foo/bar", "/baz")); // Prints "/baz"
      writeln(joinPath("/foo", "./baz")); // Prints "/foo/./baz"
      writeln(joinPath("/foo/", "", "./baz")); // Prints "/foo/./baz"
   
   :arg paths: Any number of paths.
   :type paths: `string`
   
   :return: The concatenation of the last absolute path with everything
            following it, or all the paths provided if no absolute path is
            present.
   :rtype: `string`

.. function:: proc normPath(name: string): string

   
   Normalize a path by eliminating redundant separators and up-level references.
   The paths ``foo//bar``, ``foo/bar/``, ``foo/./bar``, and ``foo/baz/../bar``
   would all be changed to ``foo/bar``.
   
   .. warning::
   
     May alter the meaning of paths containing symbolic links.
   
   .. note::
   
     Unlike its Python counterpart, this function does not (currently) change
     slashes to backslashes on Windows.
   
   :arg name: A potential path to collapse, possibly destroying the meaning of
              the path if symbolic links were included.
   :type name: `string`
   
   :return: The collapsed version of `name`.
   :rtype: `string`

.. function:: proc realPath(name: string): string throws

   Given a path ``name``, attempts to determine the canonical path referenced.
   This resolves and removes any :data:`curDir` and :data:`parentDir` uses
   present, as well as any symbolic links.  Returns the result.
   
   :arg name: A path to resolve.  If the path does not refer to a valid file
              or directory, an error will occur.
   :type name: `string`
   
   :return: A canonical version of the argument.
   :rtype: `string`
   :throws SystemError: If one occurs.

.. method:: proc file.realPath(): string throws

   Determines the canonical path referenced by the :type:`~IO.file` record
   performing this operation.  This resolves and removes any :data:`curDir` and
   :data:`parentDir` uses present, as well as any symbolic links.  Returns the
   result.
   
   :return: A canonical path to the file referenced by this :type:`~IO.file`
            record.  If the :type:`~IO.file` record is not valid, an error will
            occur.
   :rtype: `string`
   :throws SystemError: If one occurs.

.. function:: proc relPath(name: string, start: string = curDir): string throws

   
   Returns a relative filepath to `name` either from the current directory or an
   optional `start` directory. The filesystem is not accessed to verify the
   existence of the named path or the specified starting location.
   
   .. warning::
   
     This method is unsafe for use in a parallel environment due to its
     reliance on :proc:`~FileSystem.locale.cwd()`. Another task on the current
     locale may change the current working directory at any time.
   
   :arg name: A path which the caller would like to access.
   :type name: `string`
   
   :arg start: The location from which access to name is desired. If no value
     is provided, defaults to :const:`curDir`.
   :type start: `string`
   
   :return: The relative path to `name` from the current directory.
   :rtype: `string`
   
   :throws SystemError: Upon failure to get the current working directory.

.. method:: proc file.relPath(start: string = curDir): string throws

   
   Returns a relative filepath to the path in this :type:`~IO.file` either from
   the current directory or an optional `start` directory. The filesystem is not
   accessed to verify the existence of the named path or the specified starting
   location.
   
   .. warning::
   
     This method is unsafe for use in a parallel environment due to its
     reliance on :proc:`~FileSystem.locale.cwd()`. Another task on the current
     locale may change the current working directory at any time.
   
   :arg start: The location from which access to the path in this
     :type:`~IO.file` is desired. If no value is provided, defaults to
     :const:`curDir`.
   :type start: `string`
   
   :return: The relative filepath to the path in this :type:`~IO.file`.
   :rtype: `string`
   
   :throws SystemError: Upon failure to get the current working directory.

.. function:: proc splitPath(name: string): (string, string)

   Split name into a tuple that is equivalent to (:proc:`dirname`,
   :proc:`basename`).  The second part of the tuple will never contain a slash.
   Examples:
   
   .. code-block:: Chapel
   
      writeln(splitPath("foo/bar")); // Prints "(foo, bar)"
      writeln(splitPath("bar")); // Prints "(, bar)"
      writeln(splitPath("foo/")); // Prints "(foo, )"
      writeln(splitPath("")); // Prints "(, )"
      writeln(splitPath("/")); // Prints "(/, )"
   
   With the exception of a path of the empty string or just "/", the original
   path can be recreated from this function's returned parts by joining them
   with the path separator character, either explicitly or by calling
   :proc:`joinPath`:
   
   .. code-block:: Chapel
   
      var res = splitPath("foo/bar");
      var dirnameVar = res(1);
      var basenameVar = res(2);
      writeln(dirnameVar + "/" + basenameVar); // Prints "foo/bar"
      writeln(joinPath(dirnameVar, basenameVar)); // Prints "foo/bar"
   
   :arg name: Path to be split.
   :type name: `string`

