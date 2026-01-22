chpldoc
=======

.. confchpldoc.rst

SYNOPSIS
--------

| **chpldoc** [**-o** *directory*]
|             [**\--author** *authortext*]
|             [**\--save-sphinx** *directory*]
|             [options] source-files...
|

DESCRIPTION
-----------

**chpldoc** is a tool for generating HTML based documentation from
Chapel source code and its comments. It does this by parsing Chapel
source files and associating comments with symbols, then generating
reStructuredText, and finally building the output documentation with
Sphinx. Most users will not need to be aware of the use of
reStructuredText as an intermediate format.

*Documentation Options*

.. _man-chpldoc-output-dir:

**-o, \--output-dir <dirname>**

    Specify the *directory* name into which documentation should be saved
    (defaults to 'docs' if unspecified).

.. _man-chpldoc-author:

**\--author <**\ *authortext*\ **>**

    Documentation author string. *authortext* becomes the
    author in the output documentation.

.. _man-chpldoc-comment-style:

**\--comment-style <string>**

    Specify the opening comment character sequence used to distinguish a
    documentation comment from a normal one (defaults to '/\*' if
    unspecified).

.. _man-chpldoc-module-dir:

**-M, \--module-dir <**\ *directory*\ **>**

    Add the specified *directory* to the module search path. The module
    search path is used to satisfy module 'use' and 'import' statements.
    See the :ref:`man page for chpl <man-module-dir>` for further details.

.. _man-chpldoc-process-used-modules:

**\--process-used-modules**

    By default, **chpldoc** only generates documentation for the source
    **file**\ (s) named on the command-line. When this flag is thrown,
    modules that it 'use's are also parsed and processed.

.. _man-chpldoc-save-sphinx:

**\--save-sphinx <**\ *directory*\ **>**

    Save generated Sphinx project in *directory*.

.. _man-chpldoc-text-only:

**\--text-only**

    Generate text-based documentation instead of HTML. Takes precedence over
    \--[no-]html

.. _man-chpldoc-html:

**\--[no-]html**

    [Don't] generate HTML-based documentation (on by default)

.. _man-chpldoc-index:

**\--index**

    Specify a path to a custom index.rst file to be used as the main page of
    the generated documentation. If this option is not provided, a default
    index.rst file will be generated.

.. _man-chpldoc-project-name:

**\--project-name <**\ *projectname*\ **>**

    Sets the documentation project name to *projectname*
    (documentation project name defaults to 'Project Name' if unspecified).

.. _man-chpldoc-project-description:

**\--project-description <**\ *projectdescription*\ **>**

    Sets the documentation project description to *projectdescription*
    (documentation project description defaults to an empty string if
    unspecified).

.. _man-chpldoc-project-version:

**\--project-version <**\ *projectversion*\ **>**

    Sets the documentation version to *projectversion*
    (documentation version defaults to '0.0.1' if unspecified).

.. _man-chpldoc-project-copyright-year:

**\--project-copyright-year <**\ *projectcopyrightyear*\ **>**

    Sets the documentation copyright to *projectcopyrightyear*
    (documentation copyright defaults to the current year if unspecified).

.. _man-chpldoc-print-commands:

**\--[no-]print-commands**

    Prints the system commands that **chpldoc** executes in order to create
    the documentation.

.. _man-chpldoc-warn-unknown-attribute-toolname:

**\--[no-]warn-unknown-attribute-toolname**

    [Don't] warn about attribute tool names that aren't recognized. Without this
    warning, attributes belonging to unknown tools will be silently ignored.

.. _man-chpldoc-using-attribute-toolname:

**\--using-attribute-toolname <**\ *toolname*\ **>**

    Provide a tool name whose use in an attribute will not trigger an
    "unknown tool name" warning. To provide multiple tool names, use one
    **\--using-attribute-toolname** flag per name.

*Information Options*

.. _man-chpldoc-help:

**-h, \--help**

    Print a list of the command line options, indicating the arguments that
    they expect and a brief summary of their purpose.

.. _man-chpldoc-help-env:

**\--help-env**

    Print the command line option help message, listing the environment
    variable equivalent for each flag, if applicable (see ENVIRONMENT), and
    its current value.

.. _man-chpldoc-help-settings:

**\--help-settings**

    Print the command line option help message, listing the current setting
    of each option as specified by environment variables and other flags on
    the command line.

.. _man-chpldoc-version:

**\--version**

    Print **chpldoc**\ 's version number.

.. _man-chpldoc-copyright:

**\--copyright**

    Print **chpldoc**\ 's copyright information.

ENVIRONMENT
-----------

Some command-line options have an environment variable that can be used
to specify a default value. Use the **\--help-env** option to list the
environment variable equivalent for each option. Command-line options
will always override environment variable settings in the event of a
conflict.

If the environment variable equivalent is set to empty, it is considered
unset. This does not apply to options expecting a string or a path.

For options that can be used with or without the leading **\--no** (they
are shown with "[no-]" in the help text), the environment variable
equivalent, when set to a non-empty string, has the following effect.
When the first character of the string is one of:

|
    Y y T t 1 - same as passing the option without \--no,

    N n F f 0 - same as passing the option with \--no,

    anything else - generates an error.

For the other options that enable, disable or toggle some feature, any
non-empty value of the environment variable equivalent has the same
effect as passing that option once.

BUGS
----

See $CHPL\_HOME/doc/rst/usingchapel/bugs.rst for instructions on reporting bugs.

SEE ALSO
--------

$CHPL\_HOME/doc/rst/usingchapel/QUICKSTART.rst for more information on how to
get started with Chapel.

AUTHORS
-------

See $CHPL\_HOME/CONTRIBUTORS.md for a list of contributors to Chapel.

COPYRIGHT
---------

| Copyright 2020-2026 Hewlett Packard Enterprise Development LP
| Copyright 2004-2019 Cray Inc.
