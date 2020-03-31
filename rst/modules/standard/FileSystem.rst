.. default-domain:: chpl

.. module:: FileSystem
   :synopsis: A file utilities library

FileSystem
==========
**Usage**

.. code-block:: chapel

   use FileSystem;

A file utilities library

The FileSystem module focuses on file and directory properties and
operations.  It does not cover every interaction involving a file--- for
instance, path-specific operations live in the :mod:`Path` module, while
routines for opening, writing to, or reading from a file live in the
:mod:`IO` module.  Rather, it covers cases where the user would prefer a file
or directory to be handled wholesale and/or with minimal interaction.  For
example, this module contains :ref:`file-manip` and functions for determining
the :ref:`file-status`.  Also included are operations relating to the current
process's file system state, which are performed on a specified locale
(:ref:`locale-state`).  The module also contains iterators for traversing the
file system (:ref:`filerator`).

.. _file-manip:

File/Directory Manipulations
----------------------------
:proc:`copy`
:proc:`copyFile`
:proc:`copyTree`
:proc:`mkdir`
:proc:`moveDir`
:proc:`remove`
:proc:`rmTree`
:proc:`symlink`
:proc:`chmod`
:proc:`chown`
:proc:`copyMode`
:proc:`rename`

.. _file-status:

File/Directory Properties
-------------------------
:proc:`getGID`
:proc:`getMode`
:proc:`getFileSize`
:proc:`getUID`
:proc:`exists`
:proc:`isDir`
:proc:`isFile`
:proc:`isLink`
:proc:`isMount`
:proc:`sameFile`

.. _locale-state:

Locale State Functionality
--------------------------
:proc:`locale.chdir`
:proc:`locale.cwd`
:proc:`locale.umask`

.. _filerator:

File System Traversal Iterators
-------------------------------
:iter:`glob`
:iter:`listdir`
:iter:`walkdirs`
:iter:`findfiles`

Constant and Function Definitions
---------------------------------


.. data:: const S_IRUSR: int

   S_IRUSR and the following constants are values of the form
   S_I[R | W | X][USR | GRP | OTH], S_IRWX[U | G | O], S_ISUID, S_ISGID, or
   S_ISVTX, where R corresponds to readable, W corresponds to writable, X
   corresponds to executable, USR and U correspond to user, GRP and G
   correspond to group, OTH and O correspond to other, directly tied to the C
   idea of these constants as visible at `GNU Permissions
   <http://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html>`_.
   They are intended for use when dealing with the permissions of files or
   directories, such as with :proc:`chmod`, :proc:`getMode`, :proc:`mkdir`,
   or :proc:`~locale.umask`
   
   S_IRUSR refers to the user's read permission

.. data:: const S_IWUSR: int

   Refers to the user's write permission, see :data:`S_IRUSR` 

.. data:: const S_IXUSR: int

   Refers to the user's executable permission, see :data:`S_IRUSR` 

.. data:: const S_IRWXU: int

   Refers to all the user's permissions - read, write, and execute.  See
   :data:`S_IRUSR`

.. data:: const S_IRGRP: int

   Refers to the group's read permission, see :data:`S_IRUSR` 

.. data:: const S_IWGRP: int

   Refers to the group's write permission, see :data:`S_IRUSR` 

.. data:: const S_IXGRP: int

   Refers to the group's executable permission, see :data:`S_IRUSR` 

.. data:: const S_IRWXG: int

   Refers to all the permissions - read, write, and execute - of those in this
   group, see :data:`S_IRUSR`

.. data:: const S_IROTH: int

   Refers to everyone else's read permission, see :data:`S_IRUSR` 

.. data:: const S_IWOTH: int

   Refers to everyone else's write permission, see :data:`S_IRUSR` 

.. data:: const S_IXOTH: int

   Refers to everyone else's executable permission, see :data:`S_IRUSR` 

.. data:: const S_IRWXO: int

   Refers to all the permissions - read, write, and execute - of everyone else,
   see :data:`S_IRUSR`

.. data:: const S_ISUID: int

   Use of this would cause the user id to be set on execution.  See
   `GNU Permissions
   <http://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html>`_
   for what that generally means, and :data:`S_IRUSR` for intended uses, etc.

.. data:: const S_ISGID: int

   Use of this would cause the group id to be set on execution.  See
   `GNU Permissions
   <http://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html>`_
   for what that generally means, and :data:`S_IRUSR` for intended uses, etc.

.. data:: const S_ISVTX: int

   This is the sticky bit. See `GNU Permissions
   <http://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html>`_
   for what that generally means, and :data:`S_IRUSR` for intended uses, etc.

.. method:: proc locale.chdir(name: string) throws

   Change the current working directory of the locale in question to the
   specified path `name`.
   
   .. warning::
   
      This is not safe within a parallel context.  A chdir call in one task
      will affect the current working directory of all tasks for that locale.
   
   :arg name: The intended current working directory
   :type name: `string`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. function:: proc chmod(name: string, mode: int) throws

   Set the permissions of the file or directory specified by the argument
   `name` to that indicated by the argument `mode`.
   
   :arg name: The name of the file or directory whose permissions should be
              altered.
   :type name: `string`
   :arg mode: The permissions desired for the file or directory in question.
              See description of :const:`S_IRUSR`, for instance, for potential
              values.
   :type mode: `int`
   
   :throws FileNotFoundError: Thrown when the name specified does not correspond
                              to a file or directory that exists.
   :throws PermissionError: Thrown when the current user does not have
                            permission to change the permissions

.. function:: proc chown(name: string, uid: int, gid: int) throws

   Change one or both of the owner and group id of the named file or directory
   to the specified values.  If `uid` or `gid` are -1, the value in question
   will remain unchanged.
   
   :arg name: The name of the file to be changed.
   :type name: `string`
   :arg uid: The intended new owner(user) id, or -1 if it should remain the
             same.
   :type uid: `int`
   :arg gid: The intended new group owner(id), or -1 if it should remain the
             same.
   :type gid: `int`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. function:: proc copy(src: string, dest: string, metadata: bool = false) throws

   Copies the contents and permissions of the file indicated by `src` into
   the file or directory `dest`. If `metadata` is set to `true`, will also
   copy the metadata (uid, gid, time of last access and time of modification)
   of the file to be copied. A partially copied file or directory may be
   present in `dest` if there is an error in copying.
   
   .. note::
   
      In the future, when the :mod:`Path` module has been expanded further,
      this function will be able to support directories for the dest argument.
   
   :arg src: The source file whose contents and permissions are to be copied
   :type src: `string`
   :arg dest: The name of the destination file for the contents and permissions.
              May or may not exist previously, but will be overwritten if it did
              exist
   :type dest: `string`
   :arg metadata: This argument indicates whether to copy metadata associated
                  with the source file.  It is set to `false` by default.
   :type metadata: `bool`
   
   :throws IsADirectoryError: when `dest` is directory.
   :throws SystemError: thrown to describe another error if it occurs.

.. function:: proc copyFile(src: string, dest: string) throws

   Copies the contents of the file indicated by `src` into the file indicated
   by `dest`, replacing `dest`'s contents if it already exists (and is a
   different file than `src`, i.e. not a symbolic link to `src`).
   Does not copy metadata.
   
   :arg src: The source file whose contents are to be copied.
   :type src: `string`
   :arg dest: The intended destination of the contents.
   :type dest: `string`
   
   :throws FileNotFoundError: when `src` does not exist.
   :throws IsADirectoryError: when `src` or `dest` is a directory.
   :throws SystemError: when `src` and `dest` refer to the same file,
                        when `dest` is not writable,
                        or to describe another error if it occurs.

.. function:: proc copyMode(src: string, dest: string) throws

   Copies the permissions of the file indicated by `src` to the file indicated
   by `dest`, leaving contents, owner and group unaffected.
   
   :arg src: The source file whose permissions are to be copied.
   :type src: `string`
   :arg dest: The intended destination of the permissions.
   :type dest: `string`
   
   :throws FileNotFoundError: Thrown when the name specified does not correspond
                              to a file or directory that exists.
   :throws PermissionError: Thrown when the current user does not have
                            permission to change the permissions

.. function:: proc copyTree(src: string, dest: string, copySymbolically: bool = false) throws

   Will recursively copy the tree which lives under `src` into `dst`,
   including all contents, permissions, and metadata.  `dst` must not
   previously exist, this function assumes it can create it and any missing
   parent directories. If `copySymbolically` is `true`, symlinks will be
   copied as symlinks, otherwise their contents and metadata will be copied
   instead.
   
   :arg src: The root of the source tree to be copied.
   :type src: `string`
   :arg dest: The root of the destination directory under which the contents of
              `src` are to be copied (must not exist prior to this function
              call).
   :type dest: `string`
   :arg copySymbolically: This argument is used to indicate how to handle
                          symlinks in the source directory.  It is set to
                          `false` by default
   :type copySymbolically: `bool`
   
   :throws FileExistsError: when the `dest` already exists.
   :throws NotADirectoryError: when `src` is not a directory.
   :throws SystemError: thrown to describe another error if it occurs.

.. method:: proc locale.cwd(): string throws

   Obtains and returns the current working directory for this locale.
   
   .. warning::
   
      Another task on this locale can change the current working directory from
      underneath this task, so use caution when making use of this method in
      a parallel environment.
   
   :return: The current working directory for the locale in question.
   :rtype: `string`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. function:: proc exists(name: string): bool throws

   Determines if the file or directory indicated by `name` exists and returns
   the result of this check.
   
   :arg name: The file or directory whose existence is in question.
   :type name: `string`
   
   :return: `true` if the provided argument corresponds to an existing file or
            directory, `false` otherwise.  Also returns `false` for broken
            symbolic links.
   :rtype: `bool`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. iterfunction:: iter findfiles(startdir: string = ".", recursive: bool = false, hidden: bool = false): string

   Finds files from a given start directory and yields their names,
   similar to simple invocations of the command-line `find` utility.
   May be invoked in serial or non-zippered parallel contexts.
   
   :arg startdir: The root directory from which to start the search
                  (defaults to ``"."``)
   :type startdir: `string`
   
   :arg recursive: Indicates whether or not to descend recursively into
                   subdirectories (defaults to `false`)
   :type recursive: `bool`
   
   :arg hidden: Indicates whether or not to descend into hidden subdirectories and yield hidden files (defaults to `false`)
   :type hidden: `bool`
   
   :yield:  The paths to any files found, relative to `startdir`, as strings

.. function:: proc getGID(name: string): int throws

   Obtains and returns the group id associated with the file or directory
   specified by `name`.
   
   :arg name: The file or directory whose group id is desired
   :type name: `string`
   
   :return: The group id of the file or directory in question
   :rtype: `int`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. function:: proc getMode(name: string): int throws

   Obtains and returns the current permissions of the file or directory
   specified by `name`.
   
   :arg name: The file or directory whose permissions are desired.
   :type name: `string`
   
   :return: The permissions of the specified file or directory
            See description of :const:`S_IRUSR`, for instance, for potential
            values.
   :rtype: `int`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. function:: proc getFileSize(name: string): int throws

   Obtains and returns the size (in bytes) of the file specified by `name`.
   
   :arg name: The file whose size is desired
   :type name: `string`
   
   :return: The size in bytes of the file in question
   :rtype: `int`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. function:: proc getUID(name: string): int throws

   Obtains and returns the user id associated with the file or directory
   specified by `name`.
   
   :arg name: The file or directory whose user id is desired
   :type name: `string`
   
   :return: The user id of the specified file or directory
   :rtype: `int`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. iterfunction:: iter glob(pattern: string = "*"): string

   Yields filenames that match a given `glob` pattern.  May be invoked
   in serial or parallel contexts (zippered or non-).
   
   :arg pattern: The glob pattern to match against (defaults to ``"*"``)
   :type pattern: `string`
   
   :yield: The matching filenames as strings

.. function:: proc isDir(name: string): bool throws

   Determine if the provided path `name` corresponds to a directory and return
   the result
   
   :arg name: A path that could refer to a directory.
   :type name: `string`
   
   :return: `true` if the path is a directory, `false` if it is not
   :rtype: `bool`
   
   :throws SystemError: Thrown to describe an error if one occurs,
                        including the case where the path does not refer
                        to a valid file or directory.

.. function:: proc isFile(name: string): bool throws

   Determine if the provided path `name` corresponds to a file and return
   the result
   
   :arg name: A path that could refer to a file.
   :type name: `string`
   
   :return: `true` if the path is a file, `false` if it is not
   :rtype: `bool`
   
   :throws SystemError: Thrown to describe an error if one occurs,
                        including the case where the path does not refer
                        to a valid file or directory.

.. function:: proc isLink(name: string): bool throws

   Determine if the provided path `name` corresponds to a link and return the
   result.  If symbolic links are not supported, will return `false`.
   
   :arg name: A path that could refer to a symbolic link.
   :type name: `string`
   
   :return: `true` if the path is a symbolic link, `false` if it is not or
            if symbolic links are not supported.
   :rtype: `bool`
   
   :throws SystemError: Thrown to describe an error if one occurs,
                        including the case where the path does not refer
                        to a valid file or directory.

.. function:: proc isMount(name: string): bool throws

   Determine if the provided path `name` corresponds to a mount point and
   return the result.
   
   :arg name: A path that could refer to a mount point.
   :type name: `string`
   
   :return: `true` if the path is a mount point, `false` if it is not.
   :rtype: `bool`
   
   :throws SystemError: Thrown to describe an error if one occurs,
                        including the case where the path does not refer
                        to a valid file or directory.

.. iterfunction:: iter listdir(path: string = ".", hidden: bool = false, dirs: bool = true, files: bool = true, listlinks: bool = true): string

   Lists the contents of a directory.  May be invoked in serial
   contexts only.
   
   :arg path: The directory whose contents should be listed
              (defaults to ``"."``)
   :type path: `string`
   
   :arg hidden: Indicates whether hidden files/directory should be listed
                (defaults to `false`)
   :type hidden: `bool`
   
   :arg dirs: Indicates whether directories should be listed
              (defaults to `true`)
   :type dirs: `bool`
   
   :arg files: Indicates whether files should be listed (defaults to `true`)
   :type files: `bool`
   
   :arg listlinks: Indicates whether symbolic links should be listed
                   (defaults to `true`)
   :type listlinks: `bool`
   
   :yield: The names of the specified directory's contents, as strings

.. function:: proc mkdir(name: string, mode: int = 511, parents: bool = false) throws

   Attempt to create a directory with the given path, `name`.  If `parents`
   is `true`, will attempt to create any directory in the path that did not
   previously exist.
   
   .. warning::
   
      In the case where `parents` is `true`, there is a potential security
      vulnerability.  Checking whether parent directories exist and creating
      them are separate events.  This is called a Time of Check, Time of Use
      vulnerability (TOCTOU), and in the case of files or directories that did
      not previously exist, there is no known guard against it.  So even if
      `parents == true` and a parent directory didn't exist before this
      function was called but does exist afterward, it's not necessarily true
      that this function created that parent. Some other concurrent operation
      could have done so, either intentionally or unintentionally, maliciously
      or ignorantly.  This option is provided as a convenience only, and any
      attempt to perform the same functionality will run a similar risk.
   
   :arg name: The name of the directory to be created, fully specified.
   :type name: `string`
   :arg mode: The permissions desired for the directory to create.  Takes the
              current :proc:`~locale.umask` into account.  See description of
              :const:`S_IRUSR`, for instance, for potential values.
   :type mode: `int`
   :arg parents: Indicates whether parent directories should be created.  If
                 set to `false`, any nonexistent parent will cause an error
                 to occur.
   :type parents: `bool`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. function:: proc moveDir(src: string, dest: string) throws

   Recursively moves the directory indicated by `src` and its contents to the
   destination denoted by `dest`.  If `dest` is a directory, `src` is moved
   inside of it.
   
   .. note::
   
      We do not currently support the case where the dest argument already
      exists and is a directory.  When the :mod:`Path` module has been
      expanded further, this support can be enabled.
   
   :arg src: the location of the directory to be moved
   :type src: `string`
   :arg dest: the location to move it to.
   :type dest: `string`
   
   :throws IsADirectoryError: when `dest` exists and is a directory.
   :throws NotADirectoryError: when `dest` exists and is not a directory.
   :throws IllegalArgumentError: when `src` and `dest` is the same directory.
   :throws SystemError: thrown to describe another error if it occurs.

.. function:: proc rename(oldname: string, newname: string) throws

   Renames the file specified by `oldname` to `newname`.  The file is not
   opened during this operation.
   
   :arg oldname: Current name of the file
   :type oldname: `string`
   :arg newname: Name which should be used to refer to the file in the future.
   :type newname: `string`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. function:: proc remove(name: string) throws

   Removes the file or directory specified by `name`
   
   :arg name: The file/directory to remove
   :type name: `string`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. function:: proc rmTree(root: string) throws

   Delete the entire directory tree specified by root.
   
   :arg root: path name representing a directory that should be deleted along
              with its entire contents.
   :type root: `string`
   
   :throws FileNotFoundError: when `root` does not exist.
   :throws NotADirectoryError: when `root` is not a directory.
   :throws SystemError: thrown to describe another error if it occurs.

.. function:: proc sameFile(file1: string, file2: string): bool throws

   Determines if both pathnames refer to the same file or directory (utilizing
   operating system operations rather than string ones, due to the possibility
   of symbolic links, :data:`~Path.curDir`, or :data:`~Path.parentDir` appearing
   in the path) and returns the result of that check
   
   :arg file1: The first path to be compared.
   :type file1: `string`
   :arg file2: The second path to be compared.
   :type file2: `string`
   
   :return: `true` if the two paths refer to the same file or directory,
            `false` otherwise.
   :rtype: `bool`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. function:: proc sameFile(file1: file, file2: file): bool throws

   Determines if both :type:`~IO.file` records refer to the same file
   (utilizing operating system operations rather than string ones, due to the
   possibility of symbolic links, :data:`~Path.curDir`, or
   :data:`~Path.parentDir` appearing in the path) and returns the result of that
   check
   
   :arg file1: The first file to be compared.
   :type file1: `file`
   :arg file2: The second file to be compared.
   :type file2: `file`
   
   :return: `true` if the two records refer to the same file, `false`
            otherwise.
   :rtype: `bool`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. function:: proc symlink(oldName: string, newName: string) throws

   Create a symbolic link pointing to `oldName` with the path `newName`.
   
   :arg oldName: The source file to be linked
   :type oldName: `string`
   :arg newName: The location where the symbolic link should live
   :type newName: `string`
   
   :throws SystemError: Thrown to describe an error if one occurs.

.. method:: proc locale.umask(mask: int): int

   Sets the file creation mask of the current locale to `mask`, and returns
   the previous value of the file creation mask for that locale.  See
   description of :const:`S_IRUSR`, for instance, for potential values.
   
   .. warning::
   
      This is not safe within a parallel context.  A umask call in one task
      will affect the umask of all tasks for that locale.
   
   
   :arg mask: The file creation mask to use now.
   :type mask: `int`
   
   :return: The previous file creation mask
   :rtype: `int`

.. iterfunction:: iter walkdirs(path: string = ".", topdown: bool = true, depth: int = max(int), hidden: bool = false, followlinks: bool = false, sort: bool = false): string

   Recursively walk a directory structure, yielding directory names.
   May be invoked in serial or non-zippered parallel contexts.
   
   .. note::
            The current parallel version is not very adaptive/dynamic
            in its application of parallelism to the list of
            subdirectories at any given level of the traversal, and
            could be improved in this regard.
   
   :arg path: The directory from which to start the walk (defaults to ``"."``)
   :type path: `string`
   
   :arg topdown: Indicates whether to yield a directory before or after descending into its children (defaults to `true`)
   :type topdown: `bool`
   
   :arg depth: Indicates the maximum recursion depth to use (defaults to `max(int)`)
   :type depth: `int`
   
   :arg hidden: Indicates whether to descend into hidden directories (defaults to `false`)
   :type hidden: `bool`
   
   :arg followlinks: Indicates whether to follow symbolic links (defaults to `false`)
   :type followlinks: `bool`
   
   :arg sort: Indicates whether or not to consider subdirectories in sorted order (defaults to `false`).  Note that requesting sorting has no effect in parallel invocations.
   :type sort: `bool`
   
   :yield: The directory names encountered, relative to `path`, as strings

