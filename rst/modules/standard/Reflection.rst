.. default-domain:: chpl

.. module:: Reflection
   :synopsis: Functions for finding out about fields, functions, and methods.

Reflection
==========
**Usage**

.. code-block:: chapel

   use Reflection;



Functions for finding out about fields, functions, and methods.

.. note ::

  There are several ways in which this module could be improved upon:
  
    * the methods here might be better as type methods,
      so you could use `R.numFields()` instead of `numFields(R)`.
    * :proc:`getField` does not yet return a mutable value.

.. function:: proc numFields(type t) param: int

   Return the number of fields in a class or record as a param.
   The count of fields includes types and param fields.
   

.. function:: proc getFieldName(type t, param i: int) param: string

   Get the name of the ith field in a class or record.
   Causes a compilation error if `i` is not in 1..numFields(t).
   
   :arg t: a class or record type
   :arg i: which field to get the name of
   :returns: the name of the field, as a param string
   

.. function:: proc getField(const ref x: ?t, param i: int) param

   Get the ith field in a class or record. When the ith field is
   a `param` this overload will be chosen to return a `param`.
   Causes a compilation error if `i` is not in 1..numFields(t).
   
   :arg x: a class or record
   :arg i: which field to get
   :returns: the `param` that field represents

.. function:: proc getField(const ref x: ?t, param i: int) type

   Get the ith field in a class or record. When the ith field is
   a `type` variable this overload will be chosen to return a type.
   Causes a compilation error if `i` is not in 1..numFields(t).
   
   :arg x: a class or record
   :arg i: which field to get
   :returns: the type that field represents

.. function:: proc getField(const ref x: ?t, param i: int) const ref

   Get the ith field in a class or record.
   Causes a compilation error if `i` is not in 1..numFields(t).
   
   :arg x: a class or record
   :arg i: which field to get
   :returns: an rvalue referring to that field.
   

.. function:: proc getField(const ref x: ?t, param s: string) param

   Get a field in a class or record by name. When the named
   field is a `param` this overload will be chosen to return a
   `param`. Will generate a compilation error if a field with
   that name is not found.
   
   :arg x: a class or record
   :arg s: the name of a field
   :returns: the `param` that field represents
   

.. function:: proc getField(const ref x: ?t, param s: string) type

   Get a field in a class or record by name. When the named
   field is a `type` variable this overload will be chosen to
   return a type. Will generate a compilation error if a field
   with that name is not found.
   
   :arg x: a class or record
   :arg s: the name of a field
   :returns: the type that field represents
   

.. function:: proc getField(const ref x: ?t, param s: string) const ref

   Get a field in a class or record by name.
   Will generate a compilation error if a field with that name
   is not found.
   
   :arg x: a class or record
   :arg s: the name of a field
   :returns: an rvalue referring to that field.
   

.. function:: proc getFieldRef(ref x: ?t, param i: int) ref

   Get a mutable ref to the ith field in a class or record.
   Causes a compilation error if `i` is not in 1..numFields(t)
   or if the argument is not mutable.
   
   :arg x: a class or record
   :arg i: which field to get
   :returns: an rvalue referring to that field.
   

.. function:: proc getFieldRef(ref x: ?t, param s: string) ref

   Get a mutable ref to a field in a class or record by name.
   Will generate a compilation error if a field with that name
   is not found or if the class or record is not mutable.
   
   :arg x: a class or record
   :arg s: the name of a field
   :returns: an rvalue referring to that field.
   

.. function:: proc getFieldIndex(type t, param s: string) param: int

   Get a field index in a class or record, or 0 if
   the field is not found.
   
   :arg t: a class or record type
   :arg s: the name of a field
   :returns: an index `i` usable in getField, or 0 if the field was not found.
   

.. function:: proc hasField(type t, param s: string) param: bool

   Returns `true` if a class or record has a field named `s`,
   or `false` otherwise.
   
   :arg t: a class or record type
   :arg s: the name of a field
   :returns: `true` if the field is present.
   

.. function:: proc isFieldBound(type t, param i: int) param: bool

   Returns `true` if the given class or record's ith field
   has been instantiated.
   
   :arg t: a class or record type
   :arg i: which field to query
   :returns: `true` if the field is instantiated

.. function:: proc isFieldBound(type t, param s: string) param: bool

   Returns `true` if the given class or record's field named `s`
   has been instantiated.
   
   :arg t: a class or record type
   :arg s: the name of a field
   :returns: `true` if the field is instantiated

.. function:: proc canResolve(param fname: string) param: bool

   Returns true if a function named `fname` taking no arguments
   could be called in the current scope.
   

.. function:: proc canResolve(param fname: string, args ...) param: bool

   Returns true if a function named `fname` taking the arguments in `args`
   could be called in the current scope.
   

.. function:: proc canResolveMethod(obj, param fname: string) param: bool

   Returns true if a method named `fname` taking no arguments
   could be called on `obj` in the current scope.
   

.. function:: proc canResolveMethod(obj, param fname: string, args ...) param: bool

   Returns true if a method named `fname` taking the arguments in `args`
   could be called on `obj` in the current scope.
   

.. function:: proc canResolveTypeMethod(type t, param fname: string) param: bool

   Returns true if a type method named `fname` taking no
   arguments could be called on type `t` in the current scope.
   

.. function:: proc canResolveTypeMethod(type t, param fname: string, args ...) param: bool

   Returns true if a type method named `fname` taking the
   arguments in `args` could be called on type `t` in the current scope.
   

.. function:: proc getLineNumber() param: int

   Returns the line number of the call to this function. 

.. function:: proc getFileName() param: string

   Returns the file name this function was called from. 

.. function:: proc getRoutineName() param: string

   Returns the name of the function this function was called from. 

.. function:: proc getModuleName() param: string

   Returns the name of the module this function was called from. 

