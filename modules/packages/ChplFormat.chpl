/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

/* Provides serialization in a format similar to Chapel's syntax.

Intended for use with :ref:`IO support for serializers and deserializers <serialize-deserialize>`.
 */
@unstable("ChplFormat module is considered unstable pending naming changes")
module ChplFormat {
  private use IO;
  private use CTypes;
  private use Map;
  private use List;

  // TODO: out of order reading

  @chpldoc.nodoc
  type _writeType = fileWriter(serializerType=chplSerializer, ?);
  @chpldoc.nodoc
  type _readerType = fileReader(deserializerType=chplDeserializer, ?);

  record chplSerializer {

    // TODO: rewrite in terms of writef, or something
    @chpldoc.nodoc
    proc _oldWrite(ch: _writeType, const val:?t) throws {
      var _def = new defaultSerializer();
      var dc = ch.withSerializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }
      st.realfmt = 2;
      st.string_format = iostringformatInternal.chpl:uint(8);
      st.aggregate_style = QIO_AGGREGATE_FORMAT_CHPL:uint(8);
      st.array_style = QIO_ARRAY_FORMAT_CHPL:uint(8);
      st.tuple_style = QIO_TUPLE_FORMAT_CHPL:uint(8);
      st.pad_char = 0x20;
      dc._set_styleInternal(st);
      dc._writeOne(_iokind.dynamic, val, here);
    }

    proc ref serializeValue(writer: _writeType, const val:?t) throws {
      if t == string  || isEnumType(t) || t == bytes {
        _oldWrite(writer, val);
      } else if isNumericType(t) || isBoolType(t) {
        _oldWrite(writer, val);
      } else if (isDomainType(t) && val.isRectangular()) || isRangeType(t) {
        writer.withSerializer(defaultSerializer).write(val);
      } else if isClassType(t) {
        if val == nil {
          writer.writeLiteral("nil");
        } else {
          val!.serialize(writer=writer, serializer=this);
        }
      } else {
        if isArray(val) && val.rank > 1 then
          throw new IllegalArgumentError("chplSerializer does not support multidimensional arrays");
        val.serialize(writer=writer, serializer=this);
      }
    }

    record AggregateSerializer {
      var writer;
      var _parent = false;
      var _first = true;
      var _firstPtr : c_ptr(bool) = nil;

      @chpldoc.nodoc
      proc ref writeField(name: string, const field: ?T) throws {
        if !_first then writer.writeLiteral(", ");
        else _first = false;

        writer.writeLiteral(name);
        writer.writeLiteral(" = ");

        writer.write(field);
      }

      proc ref startClass(writer: _writeType, name: string, size: int) throws {
        return new AggregateSerializer(this.writer, _parent=true,
                                       _firstPtr=c_addrOf(_first));
      }

      @chpldoc.nodoc
      proc endClass() throws {
        if !_parent then
          writer.writeLiteral(")");
        else if _firstPtr != nil then
          _firstPtr.deref() = _first;
      }

      proc endRecord() throws {
        writer.writeLiteral(")");
      }
    }

    // TODO: How should generic types be printed?
    // - 'new G(int,real)(5, 42.0)' vs 'new G(int, real, 5, 42.0)'
    // - is the latter even possible?
    @chpldoc.nodoc
    proc startClass(writer: _writeType, name: string, size: int) throws {
      writer.writeLiteral("new ");
      writer.writeLiteral(name);
      writer.writeLiteral("(");

      return new AggregateSerializer(writer);
    }

    @chpldoc.nodoc
    proc startRecord(writer: _writeType, name: string, size: int) throws {
      writer.writeLiteral("new ");
      writer.writeLiteral(name);
      writer.writeLiteral("(");

      return new AggregateSerializer(writer);
    }

    record TupleSerializer {
      var writer;
      const size : int;
      var _first : bool = true;

      proc ref writeElement(const element: ?) throws {
        if !_first then writer.writeLiteral(", ");
        else _first = false;

        writer.write(element);
      }

      proc endTuple() throws {
        if size == 1 then
          writer.writeLiteral(",)");
        else
          writer.writeLiteral(")");
      }
    }

    @chpldoc.nodoc
    proc startTuple(writer: _writeType, size: int) throws {
      writer.writeLiteral("(");
      return new TupleSerializer(writer, size);
    }

    record ListSerializer {
      var writer;
      var _first : bool = true;

      proc ref writeElement(const element: ?) throws {
        if !_first then writer.writeLiteral(", ");
        else _first = false;

        writer.write(element);
      }

      @chpldoc.nodoc
      proc endList() throws {
        writer.writeLiteral("]");
      }
    }
    @chpldoc.nodoc
    proc startList(writer: fileWriter, size: int) throws {
      writer.writeLiteral("[");
      return new ListSerializer(writer);
    }

    record ArraySerializer {
      var writer;
      var _first = true;

      proc startDim(size: int) throws {
      }
      proc endDim() throws {
      }

      proc ref writeElement(const element: ?) throws {
        if !_first then writer.writeLiteral(", ");
        else _first = false;

        writer.write(element);
      }
      proc endArray() throws {
        writer.writeLiteral("]");
      }
    }

    @chpldoc.nodoc
    proc startArray(writer: _writeType, size: int) throws {
      writer.writeLiteral("[");
      return new ArraySerializer(writer);
    }

    record MapSerializer {
      var writer;
      var _first = true;

      @chpldoc.nodoc
      proc ref writeKey(const key: ?) throws {
        if !_first then writer.writeLiteral(", ");
        else _first = false;

        writer.write(key);
      }

      @chpldoc.nodoc
      proc writeValue(const val: ?) throws {
        writer.writeLiteral(" => ");
        writer.write(val);
      }

      @chpldoc.nodoc
      proc endMap() throws {
        writer.writeLiteral("]");
      }
    }

    @chpldoc.nodoc
    proc startMap(writer: _writeType, size: int) throws {
      writer.writeLiteral("[");
      return new MapSerializer(writer);
    }
  }

  record chplDeserializer {

    // TODO: rewrite in terms of writef, or something
    @chpldoc.nodoc
    proc _oldRead(ch: _readerType, ref val:?t) throws {
      var _def = new defaultDeserializer();
      var dc = ch.withDeserializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }
      st.realfmt = 2;
      st.string_format = iostringformatInternal.chpl:uint(8);
      st.aggregate_style = QIO_AGGREGATE_FORMAT_CHPL:uint(8);
      st.array_style = QIO_ARRAY_FORMAT_CHPL:uint(8);
      st.tuple_style = QIO_TUPLE_FORMAT_CHPL:uint(8);
      st.pad_char = 0x20;
      dc._set_styleInternal(st);
      dc._readOne(_iokind.dynamic, val, here);
    }

    proc ref deserializeType(reader:_readerType, type readType) : readType throws {
      if isNilableClassType(readType) && reader.matchLiteral("nil") {
        return nil:readType;
      }

      if isNumericType(readType) || isBoolType(readType) {
        var x : readType;
        reader._readOne(_iokind.dynamic, x, here);
        return x;
      } else if isStringType(readType) || isBytesType(readType) {
        var tmp : readType;
        _oldRead(reader, tmp);
        return tmp;
      } else if isEnumType(readType) {
        var ret = reader.withDeserializer(defaultDeserializer).read(readType);
        return ret;
      } else if isDomainType(readType) || isRangeType(readType) {
        return reader.withDeserializer(defaultDeserializer).read(readType);
      } else if canResolveTypeMethod(readType, "deserializeFrom", reader, this) ||
                isArrayType(readType) {
        if isArrayType(readType) && chpl__domainFromArrayRuntimeType(readType).rank > 1 then
          throw new IllegalArgumentError("chplSerializer does not support multidimensional arrays");
        return readType.deserializeFrom(reader=reader, deserializer=this);
      } else {
        return new readType(reader=reader, deserializer=this);
      }
    }

    proc ref deserializeValue(reader: _readerType, ref val: ?readType) : void throws {
      // Shortcut for domains/ranges to just use the default format
      if isDomainType(readType) || isRangeType(readType) {
        reader.withDeserializer(defaultDeserializer).read(val);
        return;
      }

      if canResolveMethod(val, "deserialize", reader, this) {
        if isArrayType(readType) && val.rank > 1 then
          throw new IllegalArgumentError("chplSerializer does not support multidimensional arrays");
        val.deserialize(reader=reader, deserializer=this);
      } else {
        val = deserializeType(reader, readType);
      }
    }

    record AggregateDeserializer {
      var reader;
      var _first = true;
      var _parent = false;

      @chpldoc.nodoc
      proc readField(name: string, type fieldType) : fieldType throws {
        reader.readLiteral(name);
        reader.readLiteral("=");
        var ret = reader.read(fieldType);
        reader.matchLiteral(",");

        return ret;
      }

      @chpldoc.nodoc
      proc readField(name: string, ref field) throws {
        reader.readLiteral(name);
        reader.readLiteral("=");
        reader.read(field);
        reader.matchLiteral(",");
      }

      proc startClass(reader:_readerType, name: string) throws {
        return new AggregateDeserializer(reader, _parent=true);
      }

      @chpldoc.nodoc
      proc endClass() throws {
        if !_parent then
          reader.readLiteral(")");
      }

      proc endRecord() throws {
        endClass();
      }
    }

    @chpldoc.nodoc
    proc startClass(reader: _readerType, name: string) throws {
      reader.readLiteral("new " + name);
      reader.readLiteral("(");

      return new AggregateDeserializer(reader);
    }

    @chpldoc.nodoc
    proc startRecord(reader: _readerType, name: string) throws {
      reader.readLiteral("new " + name);
      reader.readLiteral("(");

      return new AggregateDeserializer(reader);
    }

    record TupleDeserializer {
      var reader;

      proc readElement(type eltType) : eltType throws {
        var ret = reader.read(eltType);
        reader.matchLiteral(",");
        return ret;
      }

      proc readElement(ref element) throws {
        reader.read(element);
        reader.matchLiteral(",");
      }

      proc endTuple() throws {
        reader.readLiteral(")");
      }
    }

    @chpldoc.nodoc
    proc startTuple(reader: _readerType) throws {
      reader.readLiteral("(");
      return new TupleDeserializer(reader);
    }

    record ListDeserializer {
      var reader;
      var _first = true;

      proc ref readElement(type eltType) : eltType throws {
        if !_first then reader.readLiteral(", ");
        else _first = false;

        return reader.read(eltType);
      }

      proc ref readElement(ref element) throws {
        if !_first then reader.readLiteral(", ");
        else _first = false;

        reader.read(element);
      }

      proc endList() throws {
        reader.readLiteral("]");
      }

      proc hasMore() : bool throws {
        reader.mark();
        defer reader.revert();
        return !reader.matchLiteral("]");
      }
    }

    @chpldoc.nodoc
    proc startList(reader: _readerType) throws {
      reader.readLiteral("[");
      return new ListDeserializer(reader);
    }

    record ArrayDeserializer {
      var reader;
      forwarding var _listHelper = new ListDeserializer(reader);

      proc startDim() throws {
      }
      proc endDim() throws {
      }
      proc endArray() throws {
        _listHelper.endList();
      }
    }

    @chpldoc.nodoc
    proc startArray(reader: _readerType) throws {
      reader.readLiteral("[");
      return new ArrayDeserializer(reader);
    }

    record MapDeserializer {
      var reader;
      var _first = true;

      @chpldoc.nodoc
      proc ref readKey(type keyType) : keyType throws {
        if !_first then reader.readLiteral(",");
        else _first = false;

        return reader.read(keyType);
      }

      @chpldoc.nodoc
      proc ref readKey(ref key) throws {
        if !_first then reader.readLiteral(",");
        else _first = false;

        reader.read(key);
      }

      @chpldoc.nodoc
      proc readValue(type valType) : valType throws {
        reader.readLiteral("=>");
        return reader.read(valType);
      }

      @chpldoc.nodoc
      proc readValue(ref value) throws {
        reader.readLiteral("=>");
        reader.read(value);
      }

      @chpldoc.nodoc
      proc endMap() throws {
        reader.readLiteral("]");
      }

      proc hasMore() : bool throws {
        reader.mark();
        defer reader.revert();
        return !reader.matchLiteral("]");
      }
    }

    @chpldoc.nodoc
    proc startMap(reader: _readerType) throws {
      reader.readLiteral("[");
      return new MapDeserializer(reader);
    }
  }
}
