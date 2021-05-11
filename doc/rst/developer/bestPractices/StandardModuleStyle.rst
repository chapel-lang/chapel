Standard Module Style
=====================

This document describes general guidance for style when writing a
standard module. Of course there will be exceptions to this guidance when
there is a good reason.

Background
----------

Chapel does not use a separate namespace for types vs. module names vs
local variables. As a result, it's generally a goal to avoid using the
same name for, say, a type and a module.

CamelCase
---------

Pefer CamelCase or camelCase to separating_with_underscores. The rest of
this document will use CamelCase or camelCase to signify camel-case
starting with an upper case or lower case letter, respectively.

Modules
-------

Module names should be CamelCase.

Classes
-------

Class type names should be CamelCase. The idea is that starting with an
uppercase letter is the convention for by-reference types.

Records
-------

Record type names should be camelCase. The idea is that starting with a
lowercase letter is the convention for by-value types.

Enums
-----

Enum type names should be camelCase. The enum values should also be
camelCase.

Methods
-------

Method names should be camelCase.

Use parentheses-less methods only for simple properties that could be
reasonably implemented as fields.

Try to make methods that take some notable action be verbs. In particular,
a method that modifies an argument in-place should be a verb.

Functions
---------

Non-method functions should be camelCase.

Generally speaking it's desireable use methods when there is a clear type
responsible for the operation.

Global Variable Names
---------------------

Variables should be camelCase or CamelCase.
