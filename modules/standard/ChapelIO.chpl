/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*

Basic types and utilities in support of I/O operation.

Most of Chapel's I/O support is within the :mod:`IO` module.  This section
describes automatically included basic types and routines that support the
:mod:`IO` module.

Writing
~~~~~~~~~~~~~~~~~~~

The :proc:`writeln` function allows for a simple implementation
of a Hello World program:

.. code-block:: chapel

 writeln("Hello, World!");
 // outputs
 // Hello, World!

.. _serialize-deserialize:

The 'serialize' and 'deserialize' Methods
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A Chapel program can implement ``serialize`` and ``deserialize`` methods
on a user-defined data type to define how that type is deserialized from a
``fileReader`` or serialized to a ``fileWriter``. The method signatures for
non-class types are:

.. code-block:: chapel

   proc T.serialize(writer: fileWriter(locking=false, ?),
                    ref serializer: ?st) throws

   proc ref T.deserialize(reader: fileReader(locking=false, ?),
                          ref deserializer: ?dt) throws

The signatures for classes are slightly different:

.. code-block:: chapel

   override proc T.serialize(writer: fileWriter(locking=false, ?),
                             ref serializer: ?st) throws

   override proc T.deserialize(reader: fileReader(locking=false, ?),
                               ref deserializer: ?dt) throws

The ``serializer`` and ``deserializer`` arguments must satisfy the
:ref:`Serializer API<io-serializer-API>` and the
:ref:`Deserializer API<io-deserializer-API>`, respectively.

Basic Usage
-----------

Implementations of ``serialize`` and ``deserialize`` methods are not
necessarily required to utilize their ``serializer`` and ``deserializer``
arguments, and can instead trivially read and write from their ``fileReader``
and ``fileWriter`` arguments. For example:

.. code-block:: chapel

  // A record 'R' that serializes as an integer
  record R : writeSerializable {
    var x : int;

    proc serialize(writer: fileWriter(locking=false, ?),
                   ref serializer: ?st) {
      writer.write(x);
    }
  }

  var val = new R(5);
  writeln(val); // prints '5'

Using Serializers and Deserializers
-----------------------------------

:ref:`Serializers<io-serializer-user-API>` and
:ref:`Deserializers<io-deserializer-user-API>` support a variety of methods
to support serializing various kinds of types. These methods can be used
to serialize or deserialize a type in a format-agnostic way. For example,
consider a simple 'point' type:

.. code-block:: chapel

  record point : writeSerializable {
    var x : int;
    var y : int;
  }

The default implementation of ``point``'s ``serialize`` method will naturally
serialize ``point`` as a record. In the default serialization format, this
would look something like ``(x = 2, y = 4)``. In the JSON serialization format,
the output would instead be ``{"x":4, "y":2}``. While this may be perfectly
acceptable, what if the author of ``point`` wished to always serialize a
``point`` as a tuple?

Serializers and Deserializers have "start" methods that begin serialization
or deserialization of a type, and then return a helper object that implements
methods to continue the process. To begin serializing ``point`` as a tuple,
a user may invoke the ``startTuple`` method on the ``serializer``, passing in
the ``fileWriter`` to use when writing serialized output and the number of
elements in the tuple. The returned value from ``startTuple`` is a helper
object that implements ``writeElement`` and ``endTuple`` methods:

.. code-block:: chapel

    proc point.serialize(writer: fileWriter(locking=false, ?),
                         ref serializer: ?st) {
      // Start serializing and get the helper object
      // '2' represents the number of tuple elements to be serialized
      var ser = serializer.startTuple(writer, 2);

      ser.writeElement(x); // serialize 'x' as a tuple element
      ser.writeElement(y); // serialize 'y' as a tuple element

      // End serialization of the tuple
      ser.endTuple();
    }

Now, when using different Serializers like the :type:`~IO.defaultSerializer` or
the :type:`~JSON.jsonSerializer`, the ``point`` type can be serialized without
introducing special cases for each format:

.. code-block:: chapel

  use IO, JSON;

  var p = new point(4, 2);

  // Prints '(4, 2)' in the default serialization format
  stdout.writeln(p);

  // Prints '[4, 2]' in the JSON serialization format
  var jsonWriter = stdout.withSerializer(jsonSerializer);
  jsonWriter.writeln(p);

A similar API exists for deserialization that would allow for deserializing a
``point`` as a tuple. Please refer to the
:ref:`IO Serializers technote<ioSerializers>` for more detail on the various
kinds of types that can be serialized and deserialized. As of Chapel 1.32 the
supported type-kinds are Classes, Records, Tuples, Arrays, Lists, and Maps.

Compiler-Generated Default Methods
----------------------------------

Default ``serialize`` methods are created for all types for which a
user-defined ``serialize`` method is not provided.

Classes will be serialized as a 'Class' type-kind using the Serializer API,
and will invoke their parent ``serialize`` method before serializing their
own fields.

Records will be serialized as a 'Record' type-kind using the Serializer API,
and will serialize each field in the record.

Default ``deserialize`` methods are created for all types for which a
user-defined ``deserialize`` method is not provided.  The default
``deserialize`` methods will mirror the relevant API calls in the default
``serialize`` methods.

For more information on the default serialization format, please refer to the
:type:`~IO.defaultSerializer` and :type:`~IO.defaultDeserializer` types.

If the compiler sees a user-defined implementation of the ``serialize`` method,
the ``deserialize`` method, or the deserializing initializer, then the compiler
may choose to not automatically generate any of the other unimplemented
methods. This is out of concern that the user has intentionally deviated from
the default implementation of serialization and deserialization.

Types with compiler-generated versions of these methods do not need to
explicitly indicate that they satisfy any of the relevant serialization
interfaces (such as ``writeSerializable``).

.. note::

  Note that it is not currently possible to read and write circular
  data structures with these mechanisms.

.. _readThis-writeThis:

The readThis() and writeThis() Methods
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. warning::

  ``readThis`` and ``writeThis`` methods are deprecated. Please use
  :ref:`serialize and deserialize<serialize-deserialize>` methods instead.
  Until ``readThis`` and ``writeThis`` methods are removed, any
  compiler-generated implementations of the 'serialize' and 'deserialize'
  methods will attempt to invoke ``readThis`` and ``writeThis`` methods for
  the sake of compatibility.

A Chapel program can implement ``readThis`` and ``writeThis`` methods on a
custom data type to define how that type is read from a fileReader or written to
a fileWriter.  ``readThis`` accepts a fileReader as its only argument and the
file must be readable.  ``writeThis`` accepts a fileWriter as its only argument
and the file must be writable. If neither of these methods is defined, a default
version of ``readThis`` and ``writeThis`` will be generated by the compiler.

Note that arguments to ``readThis`` and ``writeThis`` may be locked; as a
result, calling methods on the fileReader or fileWriter in parallel from within
a ``readThis`` or ``writeThis`` may cause undefined behavior.  Additionally,
performing I/O on a global fileReader or fileWriter that is the same as the one
``readThis`` or ``writeThis`` is operating on can result in a deadlock. In
particular, these methods should not refer to :var:`~IO.stdin`,
:var:`~IO.stdout`, or :var:`~IO.stderr` explicitly or implicitly (such as by
calling the global :proc:`writeln` function).  Instead, these methods should
only perform I/O on the fileReader or fileWriter passed as an argument.

Note that the procedures :proc:`~IO.fileReader.readLiteral` and
:proc:`~IO.fileWriter.writeLiteral` may be useful when implementing ``readThis``
and ``writeThis`` methods. These methods are not included by default.

This example defines a writeThis method - so that there will be a function
resolution error if the record NoRead is read.

.. code-block:: chapel

  record NoRead {
    var x: int;
    var y: int;
    proc writeThis(f) throws {
      f.write("hello");
    }
    // Note that no readThis function will be generated.
  }
  var nr = new NoRead();
  write(nr);
  // prints out
  // hello

  // Note that read(nr) will generate a compiler error.

.. _default-readThis-writeThis:

Default writeThis and readThis Methods
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Default ``writeThis`` methods are created for all types for which a
user-defined ``writeThis`` method is not provided.  They have the following
semantics:

* for a class: outputs the values within the fields of the class prefixed by
  the name of the field and the character ``=``.  Each field is separated by a
  comma.  The output is delimited by ``{`` and ``}``.
* for a record: outputs the values within the fields of the class prefixed by
  the name of the field and the character ``=``.  Each field is separated by a
  comma.  The output is delimited by ``(`` and ``)``.

Default ``readThis`` methods are created for all types for which a user-defined
``readThis`` method is not provided.  The default ``readThis`` methods are
defined to read in the output of the default ``writeThis`` method.

Additionally, the Chapel implementation includes ``writeThis`` methods for
built-in types as follows:

* for an array: outputs the elements of the array in row-major order
  where rows are separated by line-feeds and blank lines are used to separate
  other dimensions.
* for a domain: outputs the dimensions of the domain enclosed by
  ``{`` and ``}``.
* for a range: output the lower bound of the range, output ``..``,
  then output the upper bound of the range.  If the stride of the range
  is not ``1``, output the word ``by`` and then the stride of the range.
  If the range has special alignment, output the word ``align`` and then the
  alignment.
* for tuples, outputs the components of the tuple in order delimited by ``(``
  and ``)``, and separated by commas.

These types also include ``readThis`` methods to read the corresponding format.
Note that when reading an array, the domain of the array must be set up
appropriately before the elements can be read.

.. note::

  Note that it is not currently possible to read and write circular
  data structures with these mechanisms.


 */
pragma "module included by default"
@unstable("The module name 'ChapelIO' is unstable.  If you want to use qualified naming on the symbols within it, please 'use' or 'import' the :mod:`IO` module")
module ChapelIO {
  use ChapelBase; // for uint().
  use ChapelLocale;

  // TODO -- this should probably be private
  @chpldoc.nodoc
  proc _isNilObject(val) {
    proc helper(o: borrowed RootClass) do return o == nil;
    proc helper(o) do                  return false;
    return helper(val);
  }

  use IO;
  import CTypes.{c_int};

  /*
   Local copies of IO.{EEOF,ESHORT,EFORMAT} as these are being phased out
   and are now private in IO
   */
  private extern proc chpl_macro_int_EEOF():c_int;
  private extern proc chpl_macro_int_ESHORT():c_int;
  private extern proc chpl_macro_int_EFORMAT():c_int;
  @chpldoc.nodoc
  private inline proc EEOF do return chpl_macro_int_EEOF():c_int;
  @chpldoc.nodoc
  private inline proc ESHORT do return chpl_macro_int_ESHORT():c_int;
  @chpldoc.nodoc
  private inline proc EFORMAT do return chpl_macro_int_EFORMAT():c_int;

    private
    proc isIoField(x, param i) param {
      if isType(__primitive("field by num", x, i)) ||
         isParam(__primitive("field by num", x, i)) ||
         __primitive("field by num", x, i).type == nothing {
        // I/O should ignore type or param fields
        return false;
      } else {
        return true;
      }
    }

    // ch is the fileReader or fileWriter
    // x is the record/class/union
    // i is the field number of interest
    private
    proc ioFieldNameEqLiteral(ch, type t, param i) {
      const st = ch.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
      if st == QIO_AGGREGATE_FORMAT_JSON {
        return '"' + __primitive("field num to name", t, i) + '":';
      } else {
        return __primitive("field num to name", t, i) + " = ";
      }
    }

    private
    proc ioFieldNameLiteral(ch, type t, param i) {
      const st = ch.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
      if st == QIO_AGGREGATE_FORMAT_JSON {
        return '"' + __primitive("field num to name", t, i) + '"';
      } else {
        return __primitive("field num to name", t, i);
      }
    }

    @chpldoc.nodoc
    proc writeThisFieldsDefaultImpl(writer, x:?t, inout first:bool) throws {
      param num_fields = __primitive("num fields", t);
      var isBinary = writer._binary();

      if (isClassType(t)) {
        if _to_borrowed(t) != borrowed RootClass {
          // only write parent fields for subclasses of RootClass
          // since RootClass has no .super field.
          writeThisFieldsDefaultImpl(writer, x.super, first);
        }
      }

      if isExternUnionType(t) {
        compilerError("Cannot write extern union");

      } else if !isUnionType(t) {
        // print out all fields for classes and records
        for param i in 1..num_fields {
          if isIoField(x, i) {
            if !isBinary {
              if !first then writer.writeLiteral(", ");

              const eq = ioFieldNameEqLiteral(writer, t, i);
              writer.writeLiteral(eq);
            }

            writer.write(__primitive("field by num", x, i));

            first = false;
          }
        }
      } else {
        // Handle unions.
        // print out just the set field for a union.
        var id = __primitive("get_union_id", x);
        for param i in 1..num_fields {
          if isIoField(x, i) && i == id {
            if isBinary {
              // store the union ID
              write(id);
            } else {
              const eq = ioFieldNameEqLiteral(writer, t, i);
              writer.writeLiteral(eq);
            }
            writer.write(__primitive("field by num", x, i));
          }
        }
      }
    }
    // Note; this is not a multi-method and so must be called
    // with the appropriate *concrete* type of x; that's what
    // happens now with buildDefaultWriteFunction
    // since it has the concrete type and then calls this method.

    // MPF: We would like to entirely write the default writeThis
    // method in Chapel, but that seems to be a bit of a challenge
    // right now and I'm having trouble with scoping/modules.
    // So I'll go back to writeThis being generated by the
    // compiler.... the writeThis generated by the compiler
    // calls writeThisDefaultImpl.
    @chpldoc.nodoc
    proc writeThisDefaultImpl(writer, x:?t) throws {
      const st = writer.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
      const isJson = st == QIO_AGGREGATE_FORMAT_JSON;

      if !writer._binary() {
        const start = if isJson then "{"
                      else if st == QIO_AGGREGATE_FORMAT_CHPL
                      then "new " + t:string + "("
                      else if isClassType(t) then "{"
                      else "(";
        writer.writeLiteral(start);
      }

      var first = true;

      writeThisFieldsDefaultImpl(writer, x, first);

      if !writer._binary() {
        const end = if isJson then "}"
                    else if st == QIO_AGGREGATE_FORMAT_CHPL then ")"
                    else if isClassType(t) then "}"
                    else ")";
        writer.writeLiteral(end);
      }
    }

    private proc __numIOFields(type t) : int {
      param n = __primitive("num fields", t);
      var ret = 0;
      pragma "no init"
      var dummy : t;
      for param i in 1..n {
        if isIoField(dummy, i) then
          ret += 1;
      }
      return ret;
    }

    //
    // Called by the compiler to implement the default behavior for
    // the compiler-generated 'serialize' method.
    //
    // TODO: would any formats want to print type or param fields?
    // - more useful for param fields, e.g., an enum
    //
    @chpldoc.nodoc
    proc serializeDefaultImpl(writer:fileWriter, ref serializer,
                              const x:?t) throws {
      const name = __primitive("simple type name", x);
      const numIO = __numIOFields(t);
      var ser = if isClassType(t) then
        serializer.startClass(writer, name, numIO)
      else
        serializer.startRecord(writer, name, numIO);

      if isClassType(t) && _to_borrowed(t) != borrowed RootClass {
        if x.super.type != borrowed RootClass then
          x.super.serialize(writer, ser);
      }

      param num_fields = __primitive("num fields", t);
      for param i in 1..num_fields {
        if isIoField(x, i) {
          param name : string = __primitive("field num to name", x, i);
          ser.writeField(name,
                         __primitive("field by num", x, i));
        }
      }

      if isClassType(t) then
        ser.endClass();
      else
        ser.endRecord();
    }

    @chpldoc.nodoc
    proc deserializeDefaultImpl(reader: fileReader, ref deserializer,
                                ref x:?t) throws {
      const name = __primitive("simple type name", x):string;
      var des = if isClassType(t) then
        deserializer.startClass(reader, name)
      else
        deserializer.startRecord(reader, name);

      if isClassType(t) && _to_borrowed(t) != borrowed RootClass {
        if x.super.type != borrowed RootClass then
          x.super.deserialize(reader, des);
      }

      param num_fields = __primitive("num fields", t);
      for param i in 1..num_fields {
        if isIoField(x, i) {
          param name : string = __primitive("field num to name", x, i);
          ref field = __primitive("field by num", x, i);
          des.readField(name, field);
        }
      }

      if isClassType(t) then
        des.endClass();
      else
        des.endRecord();
    }

    //
    // Used by the compiler to support the compiler-generated initializers that
    // accept a 'fileReader'. The type 'fileReader' may not be readily
    // available, but the ChapelIO module generally is available and so
    // we place the check here. For example:
    //
    //   proc R.init(r) where chpl__isFileReader(r.type) { ... }
    //
    proc chpl__isFileReader(type T) param : bool {
      return isSubtype(T, fileReader(?));
    }

    private
    proc skipFieldsAtEnd(reader, inout needsComma:bool) throws {
      const qioFmt = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
      const isJson = qioFmt == QIO_AGGREGATE_FORMAT_JSON;
      const qioSkipUnknown = QIO_STYLE_ELEMENT_SKIP_UNKNOWN_FIELDS;
      const isSkipUnknown = reader.styleElement(qioSkipUnknown) != 0;

      if !isSkipUnknown || !isJson then return;

      while true {
        if needsComma {

          // Try reading a comma. If we don't, break out of the loop.
          try {
            reader.readLiteral(",", true);
            needsComma = false;
          } catch err: BadFormatError {
            break;
          }
        }

        // Skip an unknown JSON field.


        try reader._skipField();
        needsComma = true;
      }
    }

    @chpldoc.nodoc
    proc readThisFieldsDefaultImpl(reader, type t, ref x,
                                   inout needsComma: bool) throws
        where !isUnionType(t) {

      param numFields = __primitive("num fields", t);
      var isBinary = reader._binary();

      if isClassType(t) && _to_borrowed(t) != borrowed RootClass {

        //
        // Only write parent fields for subclasses of RootClass since
        // RootClass has no .super field.
        //
        type superType = x.super.type;

        // Copy the pointer to pass it by ref.
        var castTmp: superType = x;

        try {
          // Read superclass fields.
          readThisFieldsDefaultImpl(reader, superType, castTmp,
                                    needsComma);
        } catch err {

          // TODO: Hold superclass errors or just throw immediately?
          throw err;
        }
      }

      if isBinary {

        // Binary is simple, just read all fields in order.
        for param i in 1..numFields do
          if isIoField(x, i) then
            try reader.readIt(__primitive("field by num", x, i));
      } else if numFields > 0 {

        // This tuple helps us not read the same field twice.
        var readField: (numFields) * bool;

        // These two help us know if we've read all the fields.
        var numToRead = 0;
        var numRead = 0;

        for param i in 1..numFields do
          if isIoField(x, i) then
            numToRead += 1;

        // The order should not matter.
        while numRead < numToRead {

          // Try reading a comma. If we don't, then break.
          if needsComma then
            try {
              reader.readLiteral(",", true);
              needsComma = false;
            } catch err: BadFormatError {
              // Break out of the loop if we didn't read a comma.
              break;
            }

          //
          // Find a field name that matches.
          //
          // TODO: this is not particularly efficient. If we have a lot of
          // fields, this is O(n**2), and there are other potential problems
          // with string reallocation.
          // We could do better if we put the field names to scan for into
          // a regular expression, possibly with | and ( ) for capture
          // groups so we can know which field was read.
          //

          var st = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
          const qioSkipUnknown = QIO_STYLE_ELEMENT_SKIP_UNKNOWN_FIELDS;
          var isSkipUnknown = reader.styleElement(qioSkipUnknown) != 0;

          var hasReadFieldName = false;
          const isJson = st == QIO_AGGREGATE_FORMAT_JSON;

          for param i in 1..numFields {
            if !isIoField(x, i) || hasReadFieldName || readField[i-1] then
              continue;

            try {
              const fieldName = ioFieldNameLiteral(reader, t, i);
              reader.readLiteral(fieldName);
            } catch e : BadFormatError {
              // Try reading again with a different union element.
              continue;
            } catch e : EofError {
              continue;
            }

            hasReadFieldName = true;
            needsComma = true;

            const equalSign = if isJson then ":"
                              else "=";

            try reader.readLiteral(equalSign, true);

            try reader.readIt(__primitive("field by num", x, i));
            readField[i-1] = true;
            numRead += 1;
          }


          // Try skipping fields if we're JSON and allowed to do so.
          if !hasReadFieldName then
            if isSkipUnknown && isJson {
              try reader._skipField();
              needsComma = true;
            } else {
              throw new owned
                BadFormatError("Failed to read field, could not skip");
            }
        }

        // Check that we've read all fields, return error if not.
        if numRead == numToRead {
          // TODO: Do we throw superclass error here?
        } else {
          param tag = if isClassType(t) then "class" else "record";
          const msg = "Read only " + numRead:string + " out of "
              + numToRead:string + " fields of " + tag + " " + t:string;
          throw new owned
            BadFormatError(msg);
        }
      }
    }

    @chpldoc.nodoc
    proc readThisFieldsDefaultImpl(reader, type t, ref x,
                                   inout needsComma: bool) throws
        where isUnionType(t) && !isExternUnionType(t) {

      param numFields = __primitive("num fields", t);
      var isBinary = reader._binary();


      if isBinary {
        var id = __primitive("get_union_id", x);

        // Read the ID.
        try reader.readIt(id);
        for param i in 1..numFields do
          if isIoField(x, i) && i == id then
            try reader.readIt(__primitive("field by num", x, i));
      } else {

        // Read the field name = part until we get one that worked.
        var hasFoundAtLeastOneField = false;

        for param i in 1..numFields {
          if !isIoField(x, i) then continue;

          try {
            const fieldName = ioFieldNameLiteral(reader, t, i);
            reader.readLiteral(fieldName);
          } catch e : BadFormatError {
            // Try reading again with a different union element.
            continue;
          } catch e : EofError {
            continue;
          }

          hasFoundAtLeastOneField = true;

          const st = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
          const isJson = st == QIO_AGGREGATE_FORMAT_JSON;
          const eq = if isJson then ":"
                     else "=";

          try reader.readLiteral(eq, true);

          // We read the 'name = ', so now read the value!
          __primitive("set_union_id", x, i);
          try reader.readIt(__primitive("field by num", x, i));
        }

        if !hasFoundAtLeastOneField then
          throw new owned
            BadFormatError("Failed to find any union fields");
      }
    }

    // Note; this is not a multi-method and so must be called
    // with the appropriate *concrete* type of x; that's what
    // happens now with buildDefaultWriteFunction
    // since it has the concrete type and then calls this method.
    @chpldoc.nodoc
    proc readThisDefaultImpl(reader, x:?t) throws where isClassType(t) {
      const st = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);

      if !reader._binary() {
        const start = if st == QIO_AGGREGATE_FORMAT_CHPL
                      then "new " + t:string + "("
                      else "{";

        try reader.readLiteral(start);
      }

      var needsComma = false;

      // Make a copy of the reference that we can modify.
      var obj = x;

      try readThisFieldsDefaultImpl(reader, t, obj, needsComma);
      try skipFieldsAtEnd(reader, needsComma);

      if !reader._binary() {
        const end = if st == QIO_AGGREGATE_FORMAT_CHPL then ")"
                    else "}";

        try reader.readLiteral(end);
      }
    }

    @chpldoc.nodoc
    proc readThisDefaultImpl(reader, ref x:?t) throws where !isClassType(t) {
      const st = reader.styleElement(QIO_STYLE_ELEMENT_AGGREGATE);
      const isJson = st ==  QIO_AGGREGATE_FORMAT_JSON;

      if !reader._binary() {
        const start = if st ==  QIO_AGGREGATE_FORMAT_CHPL
                      then "new " + t:string + "("
                      else if isJson then "{"
                      else "(";

        try reader.readLiteral(start);
      }

      var needsComma = false;

      try readThisFieldsDefaultImpl(reader, t, x, needsComma);
      try skipFieldsAtEnd(reader, needsComma);

      if !reader._binary() {
        const end = if isJson then "}"
                    else ")";

        try reader.readLiteral(end);
      }
    }

  @chpldoc.nodoc
  proc locale.writeThis(f) throws {
    // FIXME this doesn't resolve without `this`
    f.write(this._instance);
  }

  @chpldoc.nodoc
  proc locale.serialize(writer, ref serializer) throws {
    writer.write(this._instance);
  }
  locale implements writeSerializable;

  @chpldoc.nodoc
  proc _ddata.writeThis(f) throws {
    compilerWarning("printing _ddata class");
    f.write("<_ddata class cannot be printed>");
  }

  @chpldoc.nodoc
  proc _ddata.serialize(writer, ref serializer) throws {
    compilerWarning("printing _ddata class");
    writer.write("<_ddata class cannot be printed>");
  }
  implements writeSerializable(_ddata);

  proc chpl_taskID_t.writeThis(f) throws {
    f.write(this : uint(64));
  }
  proc chpl_taskID_t.serialize(writer, ref serializer) throws { writeThis(writer); }

  proc chpl_taskID_t.readThis(f) throws {
    this = f.read(uint(64)) : chpl_taskID_t;
  }

  proc type chpl_taskID_t.deserializeFrom(reader, ref deserializer) throws {
    var ret : chpl_taskID_t;
    ret.readThis(reader);
    return ret;
  }

  @chpldoc.nodoc
  proc nothing.writeThis(f) {}
  @chpldoc.nodoc
  proc nothing.serialize(writer, ref serializer) {}

  @chpldoc.nodoc
  proc ref _tuple.readThis(f) throws {
    _readWriteHelper(f);
  }

  @chpldoc.nodoc
  proc ref _tuple.writeThis(f) throws {
    _readWriteHelper(f);
  }

  // Moved here to avoid circular dependencies in ChapelTuple.
  @chpldoc.nodoc
  proc ref _tuple._readWriteHelper(f) throws {
    const st = f.styleElement(QIO_STYLE_ELEMENT_TUPLE);
    const isJson = st == QIO_TUPLE_FORMAT_JSON;
    const binary = f._binary();

    // Returns a 4-tuple containing strings representing:
    // - start of a tuple
    // - the comma/separator between elements
    // - a comma/separator for 1-tuples
    // - end of a tuple
    proc getLiterals() : 4*string {
      if st == QIO_TUPLE_FORMAT_SPACE {
        return ("", " ", "", "");
      } else if isJson {
        return ("[", ", ", "", "]");
      } else {
        return ("(", ", ", ",", ")");
      }
    }

    const (start, comma, comma1tup, end) = getLiterals();

    proc helper(const ref arg) throws where f._writing { f.write(arg); }
    proc helper(ref arg) throws where !f._writing { arg = f.read(arg.type); }

    proc rwLiteral(lit:string) throws {
      if f._writing then f.writeLiteral(lit); else f.readLiteral(lit);
    }

    if !binary {
      rwLiteral(start);
    }
    if size > 1 {
      helper(this(0));
      for param i in 1..size-1 {
        if !binary {
          rwLiteral(comma);
        }
        helper(this(i));
      }
    } else if size == 1 {
      helper(this(0));
      if !binary then
        rwLiteral(comma1tup);
    } else {
      // size < 1, print nothing
    }
    if !binary {
      rwLiteral(end);
    }
  }

  @chpldoc.nodoc
  proc type _tuple.deserializeFrom(reader, ref deserializer) throws {
    pragma "no init"
    var ret : this;
    ret.deserialize(reader, deserializer);
    return ret;
  }

  @chpldoc.nodoc
  proc ref _tuple.deserialize(reader, ref deserializer) throws {
    var des = deserializer.startTuple(reader);
    for param i in 0..<this.size {
      pragma "no auto destroy"
      var elt = des.readElement(this(i).type);
      __primitive("=", this(i), elt);
    }
    des.endTuple();
  }
  implements readDeserializable(_tuple);

  @chpldoc.nodoc
  proc const _tuple.serialize(writer, ref serializer) throws {
    var ser = serializer.startTuple(writer, this.size);
    for param i in 0..<size {
      const ref elt = this(i);
      ser.writeElement(elt);
    }
    ser.endTuple();
  }
  implements writeSerializable(_tuple);

  @chpldoc.nodoc
  proc _iteratorRecord.writeThis(f) throws {
    var first: bool = true;
    for e in this {
      if !first then
        f.write(" ");
      else
        first = false;
      f.write(e);
    }
  }

  @chpldoc.nodoc
  proc _iteratorRecord.serialize(writer, ref serializer) throws {
    if serializer.type == IO.defaultSerializer {
      writeThis(writer);
    } else {
      if chpl_warnUnstable then
        compilerWarning("Serialization of iterators with non-default Serializer is unstable, and may change in the future");
      var ser = serializer.startList(writer, -1);
      for e in this do ser.writeElement(e);
      ser.endList();
    }
  }

  // Moved here to avoid circular dependencies in ChapelRange
  // Write implementation for ranges
  // Follows operator :(range, string)
  @chpldoc.nodoc
  proc range.writeThis(f) throws
  {
    if hasLowBound() then
      f.write(lowBound);

    f.writeLiteral("..");

    if hasHighBound() {
      if (chpl__singleValIdxType(this.idxType) && this._low != this._high) {
        f.writeLiteral("<");
        f.write(lowBound);
      } else {
        f.write(highBound);
      }
    }

    if stride != 1 {
      f.writeLiteral(" by ");
      f.write(stride);

      if stride != -1 && isAligned() && ! chpl_isNaturallyAligned() {
    // Write out the alignment only if it differs from natural alignment.
    // We take alignment modulo the stride for consistency.
      f.writeLiteral(" align ");
      f.write(alignment);
      }
    }
  }

  @chpldoc.nodoc
  proc range.serialize(writer, ref serializer) throws {
    if serializer.type == defaultSerializer {
      writeThis(writer);
    } else {
      if chpl_warnUnstable then
        compilerWarning("Serialization of ranges with non-default Serializer is unstable, and may change in the future");
      if serializer.type == binarySerializer {
        writer.write((this.low, this.high, this.stride, this.alignment));
      } else {
        writer.write(this:string);
      }
    }
  }
  implements writeSerializable(range);

  @chpldoc.nodoc
  proc ref range.readThis(f) throws {
    if hasLowBound() then _low = f.read(_low.type);

    f.readLiteral("..");

    if hasHighBound() then _high = f.read(_high.type);

    if f.matchLiteral(" by ") {
      const strideVal = f.read(strType);
      var expectedStride = "";
      use strideKind;
      select strides {
        when one      do if strideVal != 1 then expectedStride = "stride 1";
        when negOne   do if strideVal != -1 then expectedStride = "stride -1";
        when positive do if strideVal < 0 then expectedStride = "a positive";
        when negative do if strideVal > 0 then expectedStride = "a negative";
        when any      do;
      }
      if expectedStride != "" then throw new owned BadFormatError(
        "for a range with strides=" + strides:string + ", expected " +
        (if expectedStride.size > 2 then expectedStride + " stride"
         else expectedStride) + ", got stride ", strideVal:string);

      if ! hasParamStride() then
        this = (this by strideVal):this.type;
    }

    if f.matchLiteral(" align ") {
      const alignVal = f.read(chpl_integralIdxType);
      if hasParamStrideAltvalAld() {
        // It is valid to align any range. In this case we do not store
        // the alignment at runtime because it always normalizes to 0.
      } else {
        this = (this align alignVal):this.type;
      }
    }
  }

  @chpldoc.nodoc
  proc ref range.deserialize(reader, ref deserializer) throws {
    if deserializer.type == IO.defaultDeserializer {
      readThis(reader);
    } else {
      if chpl_warnUnstable then
        compilerWarning("Deserialization of ranges with non-default Deserializer is unstable, and may change in the future");
      if deserializer.type == binaryDeserializer {
        var (lo, hi, str, al) = reader.read((idxType, idxType, strType, idxType));
        this = (lo..hi by str align al):this.type;
      } else {
        const data = reader.read(string);
        var f = openMemFile();
        f.writer(locking=false).write(data);
        readThis(f.reader(locking=false));
      }
    }
  }
  implements readDeserializable(range);

  @chpldoc.nodoc
  proc range.init(type idxType = int,
                  param bounds : boundKind = boundKind.both,
                  param strides : strideKind = strideKind.one,
                  reader: fileReader(?),
                  ref deserializer) throws {
    this.init(idxType, bounds, strides);
    this.deserialize(reader, deserializer);
  }
  implements initDeserializable(range);

  @chpldoc.nodoc
  override proc LocaleModel.writeThis(f) throws {
    f.writeLiteral("LOCALE");
    f.write(chpl_id());
  }

  @chpldoc.nodoc
  override proc LocaleModel.serialize(writer, ref serializer) throws {
    writeThis(writer);
  }
  LocaleModel implements writeSerializable;

  /* Errors can be printed out. In that event, they will
     show information about the error including the result
     of calling :proc:`Error.message`.
  */
  @chpldoc.nodoc
  override proc Error.writeThis(f) throws {
    f.write(chpl_describe_error(this));
  }

  @chpldoc.nodoc
  override proc Error.serialize(writer, ref serializer) throws {
    writer.write(chpl_describe_error(this));
  }
  Error implements writeSerializable;

  /* Equivalent to ``try! stdout.write``. See :proc:`IO.fileWriter.write` */
  proc write(const args ...?n) {
    try! stdout.write((...args));
  }
  /* Equivalent to ``try! stdout.writeln``. See :proc:`IO.fileWriter.writeln` */
  proc writeln(const args ...?n) {
    try! stdout.writeln((...args));
  }

  // documented in the arguments version.
  @chpldoc.nodoc
  proc writeln() {
    try! stdout.writeln();
  }

 /* Equivalent to ``try! stdout.writef``. See
     :proc:`FormattedIO.fileWriter.writef`. */
  proc writef(fmt:?t, const args ...?k)
      where isStringType(t) || isBytesType(t)
  {
    try! { stdout.writef(fmt, (...args)); }
  }
  // documented in string version
  @chpldoc.nodoc
  proc writef(fmt:?t)
      where isStringType(t) || isBytesType(t)
  {
    try! { stdout.writef(fmt); }
  }

  @chpldoc.nodoc
  proc chpl_stringify_wrapper(const args ...):string {
    use IO only chpl_stringify;
    return chpl_stringify((...args));
  }

  //
  // handle casting FCF types to string
  //
  @chpldoc.nodoc
  proc isFcfType(type t) param do
    return __primitive("is fcf type", t);

  @chpldoc.nodoc
  operator :(x, type t:string) where isFcfType(x.type) do
    return chpl_stringify_wrapper(x);

  //
  // Catch all
  //
  // Convert 'x' to a string just the way it would be written out.
  //
  // This is marked as last resort so it doesn't take precedence over
  // generated casts for types like enums
  //
  // This version only applies to non-primitive types
  // (primitive types should support :string directly)
  pragma "last resort"
  @chpldoc.nodoc
  operator :(x, type t:string) where !isPrimitiveType(x.type) {
    compilerWarning(
      "universal 'x:string' is deprecated; please define a cast-to-string operator on the type '" +
      x.type:string +
      "', or use 'try! \"%?\".format(x)' from IO.FormattedIO instead"
    );
    return chpl_stringify_wrapper(x);
  }
}
