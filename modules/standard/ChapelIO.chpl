/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

Automatically included IO symbols

All Chapel programs include :proc:`~IO.write`, :proc:`~IO.writeln` and
:proc:`~IO.writef` by default. This allows for a simple implementation of a
Hello World program:

.. literalinclude:: ../../../../test/library/standard/ChapelIO/doc-examples/ChapelIOExamples.chpl
   :language: chapel
   :start-after: START_EXAMPLE_0
   :end-before: STOP_EXAMPLE_0

.. begin ChapelIO docs
 */
pragma "module included by default"
@unstable("The module name 'ChapelIO' is unstable.  If you want to use qualified naming on the symbols within it, please 'use' or 'import' the :mod:`IO` module")
module ChapelIO {
  use ChapelBase; // for uint().
  use ChapelLocale;

  use IO;
  import CTypes.{c_int};

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

    pragma 'fn exempt instantiation limit'
    @chpldoc.nodoc
    proc serializeDefaultImpl(writer:fileWriter, ref serializer,
                              const x:?t) throws where isUnionType(t) {
      // Handle unions.
      // print out just the set field for a union.
      writer.writeLiteral("(");
      param num_fields = __primitive("num fields", t);
      var id = __primitive("get_union_id", x);
      for param i in 1..num_fields {
        if isIoField(x, i) && i == id {
          const eq = __primitive("field num to name", t, i) + " = ";
          writer.writeLiteral(eq);
          writer.write(__primitive("field by num", x, i));
        }
      }
      writer.writeLiteral(")");
    }

    //
    // Called by the compiler to implement the default behavior for
    // the compiler-generated 'serialize' method.
    //
    // TODO: would any formats want to print type or param fields?
    // - more useful for param fields, e.g., an enum
    //
    pragma 'fn exempt instantiation limit'
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

  @chpldoc.nodoc
  proc locale.serialize(writer, ref serializer) throws {
    // FIXME this doesn't resolve without `this`
    writer.write(this._instance);
  }
  locale implements writeSerializable;

  @chpldoc.nodoc
  proc _ddata.serialize(writer, ref serializer) throws {
    compilerWarning("printing _ddata class");
    writer.write("<_ddata class cannot be printed>");
  }
  implements writeSerializable(_ddata);

  proc chpl_taskID_t.serialize(writer, ref serializer) throws {
    writer.write(this : uint(64));
  }

  proc ref chpl_taskID_t.deserialize(reader, ref deserializer) throws {
    this = reader.read(uint(64)) : chpl_taskID_t;
  }

  proc type chpl_taskID_t.deserializeFrom(reader, ref deserializer) throws {
    var ret : chpl_taskID_t;
    ret.deserialize(reader, deserializer);
    return ret;
  }

  @chpldoc.nodoc
  proc nothing.serialize(writer, ref serializer) {}

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
  proc _iteratorRecord._defaultWriteHelper(f) throws {
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
    if isDefaultSerializerType(serializer.type) {
      _defaultWriteHelper(writer);
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
  proc range._defaultWriteHelper(f) throws
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
    if isDefaultSerializerType(serializer.type) {
      _defaultWriteHelper(writer);
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
  proc ref range._defaultReadHelper(f) throws {
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
      _defaultReadHelper(reader);
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
        _defaultReadHelper(f.reader(locking=false));
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
  override proc LocaleModel.serialize(writer, ref serializer) throws {
    writer.writeLiteral("LOCALE");
    writer.write(chpl_id());
  }
  LocaleModel implements writeSerializable;

  /* Errors can be printed out. In that event, they will
     show information about the error including the result
     of calling :proc:`Error.message`.
  */
  @chpldoc.nodoc
  override proc Error.serialize(writer, ref serializer) throws {
    writer.write(chpl_describe_error(this));
  }
  Error implements writeSerializable;

  /* Equivalent to ``try! stdout.write``. See :proc:`IO.fileWriter.write` */
  pragma "last resort"
  proc write(const args ...?n, sep: string = "") {
    if chpl_warnUnstable then
      compilerWarning("specifying 'sep' is an unstable feature");
    try! stdout.writeHelper(none, sep, (...args));
  }
  @chpldoc.nodoc
  proc write(const args ...?n) {
    try! stdout.write((...args));
  }
  /* Equivalent to ``try! stdout.writeln``. See :proc:`IO.fileWriter.writeln` */
  pragma "last resort"
  proc writeln(const args ...?n, sep: string = "") {
    if chpl_warnUnstable then
      compilerWarning("specifying 'sep' is an unstable feature");
    try! stdout.writeHelper(new chpl_ioNewline(), sep, (...args));
  }
  @chpldoc.nodoc
  proc writeln(const args ...?n) {
    try! stdout.writeHelper(new chpl_ioNewline(), none, (...args));
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
    import IO.{chpl_stringify};
    return chpl_stringify((...args));
  }

  // NOTE: Moved here to avoid circular dependencies in 'StringCasts.chpl'.
  //
  // Cast a procedure value to a string. This path handles both the legacy
  // class types and the newer procedure pointer types.
  //
  @chpldoc.nodoc
  operator :(x: ?t1, type t2: string) where isProcedureType(t1) {
    return chpl_stringify_wrapper(x);
  }
}
