/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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
 The ChplFormat module provides a ChplSerializer and ChplDeserializer that
 aim to read and write data in a format similar to that of Chapel's syntax.
 */
@unstable("ChplFormat module is considered unstable pending naming changes")
module ChplFormat {
  private use IO;
  private use CTypes;
  private use Map;
  private use List;

  // TODO: out of order reading

  @chpldoc.nodoc
  type _writeType = fileWriter(serializerType=ChplSerializer, ?);
  @chpldoc.nodoc
  type _readerType = fileReader(deserializerType=ChplDeserializer, ?);

  record ChplSerializer {
    @chpldoc.nodoc
    var _firstThing = true;
    @chpldoc.nodoc
    var _inheritLevel = 0;
    @chpldoc.nodoc
    var _typename : string;

    // TODO: rewrite in terms of writef, or something
    @chpldoc.nodoc
    proc _oldWrite(ch: _writeType, const val:?t) throws {
      var _def = new DefaultSerializer();
      var dc = ch.withSerializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }
      st.realfmt = 2;
      st.string_format = iostringformat.chpl:uint(8);
      st.aggregate_style = QIO_AGGREGATE_FORMAT_CHPL:uint(8);
      st.array_style = QIO_ARRAY_FORMAT_CHPL:uint(8);
      st.tuple_style = QIO_TUPLE_FORMAT_CHPL:uint(8);
      st.pad_char = 0x20;
      dc._set_styleInternal(st);
      dc._writeOne(dc.kind, val, here);
    }

    proc serializeValue(writer: _writeType, const val:?t) throws {
      if t == string  || isEnumType(t) || t == bytes {
        _oldWrite(writer, val);
      } else if isNumericType(t) || isBoolType(t) {
        _oldWrite(writer, val);
      } else if isClassType(t) {
        if val == nil {
          writer._writeLiteral("nil");
        } else {
          var alias = writer.withSerializer(new ChplSerializer(_typename=t:string));
          val!.serialize(writer=alias, serializer=alias.serializer);
        }
      } else {
        if isArray(val) && val.rank > 1 then
          throw new IllegalArgumentError("ChplSerializer does not support multidimensional arrays");
        var alias = writer.withSerializer(new ChplSerializer(_typename=t:string));
        val.serialize(writer=alias, serializer=alias.serializer);
      }
    }

    @chpldoc.nodoc
    proc serializeField(writer: _writeType, name: string, const val: ?T) throws {
      if !_firstThing then writer.writeLiteral(", ");
      else _firstThing = false;

      if !name.isEmpty() {
        writer.writeLiteral(name);
        writer.writeLiteral(" = ");
      }

      writer.write(val);
    }

    // Class helpers
    //
    // TODO: How should generic types be printed?
    // - 'new G(int,real)(5, 42.0)' vs 'new G(int, real, 5, 42.0)'
    // - is the latter even possible?
    @chpldoc.nodoc
    proc startClass(writer: _writeType, name: string, size: int) throws {
      if _inheritLevel == 0 {
        writer._writeLiteral("new ");
        writer._writeLiteral(_typename);
        writer.writeLiteral("(");
      }

      _inheritLevel += 1;
    }
    @chpldoc.nodoc
    proc endClass(writer: _writeType) throws {
      if _inheritLevel == 1 {
        writer.writeLiteral(")");
      }

      _inheritLevel -= 1;
    }

    // Record helpers
    @chpldoc.nodoc
    proc startRecord(writer: _writeType, name: string, size: int) throws {
      writer._writeLiteral("new ");
      writer._writeLiteral(_typename);
      writer.writeLiteral("(");
    }
    @chpldoc.nodoc
    proc endRecord(writer: _writeType) throws {
      writer.writeLiteral(")");
    }

    // Tuple helpers
    @chpldoc.nodoc
    proc startTuple(writer: _writeType, size: int) throws {
      writer.writeLiteral("(");
    }
    @chpldoc.nodoc
    proc endTuple(writer: _writeType) throws {
      writer.writeLiteral(")");
    }

    // List helpers
    @chpldoc.nodoc
    proc startList(writer: _writeType, size: uint) throws {
      writer._writeLiteral("[");
    }
    @chpldoc.nodoc
    proc writeListElement(writer: _writeType, const val: ?) throws {
      if !_firstThing then writer._writeLiteral(", ");
      else _firstThing = false;

      writer.write(val);
    }
    @chpldoc.nodoc
    proc endList(writer: _writeType) throws {
      writer._writeLiteral("]");
    }

    // Array helpers
    @chpldoc.nodoc
    proc startArray(writer: _writeType, size: uint) throws {
      startList(writer, size);
    }
    @chpldoc.nodoc
    proc startArrayDim(writer: _writeType, size: uint) throws {
    }
    @chpldoc.nodoc
    proc endArrayDim(writer: _writeType) throws {
    }

    @chpldoc.nodoc
    proc writeArrayElement(writer: _writeType, const val: ?) throws {
      writeListElement(writer, val);
    }

    @chpldoc.nodoc
    proc endArray(writer: _writeType) throws {
      endList(writer);
    }

    // Map helpers
    @chpldoc.nodoc
    proc startMap(writer: _writeType, size: uint) throws {
      writer._writeLiteral("[");
    }

    @chpldoc.nodoc
    proc writeKey(writer: _writeType, const key: ?) throws {
      if !_firstThing then writer._writeLiteral(", ");
      else _firstThing = false;

      writer.write(key);
    }

    @chpldoc.nodoc
    proc writeValue(writer: _writeType, const val: ?) throws {
      writer._writeLiteral(" => ");
      writer.write(val);
    }

    @chpldoc.nodoc
    proc endMap(writer: _writeType) throws {
      writer._writeLiteral("]");
    }
  }

  record ChplDeserializer {
    @chpldoc.nodoc
    var _firstThing = true;
    @chpldoc.nodoc
    var _inheritLevel = 0;
    @chpldoc.nodoc
    var _typename : string;

    // TODO: rewrite in terms of writef, or something
    @chpldoc.nodoc
    proc _oldRead(ch: _readerType, ref val:?t) throws {
      var _def = new DefaultDeserializer();
      var dc = ch.withDeserializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }
      st.realfmt = 2;
      st.string_format = iostringformat.chpl:uint(8);
      st.aggregate_style = QIO_AGGREGATE_FORMAT_CHPL:uint(8);
      st.array_style = QIO_ARRAY_FORMAT_CHPL:uint(8);
      st.tuple_style = QIO_TUPLE_FORMAT_CHPL:uint(8);
      st.pad_char = 0x20;
      dc._set_styleInternal(st);
      dc._readOne(dc.kind, val, here);
    }

    proc deserializeType(reader:_readerType, type readType) : readType throws {
      if isNilableClassType(readType) && reader.matchLiteral("nil") {
        return nil:readType;
      }

      if isNumericType(readType) || isBoolType(readType) {
        var x : readType;
        reader._readOne(reader.kind, x, here);
        return x;
      } else if isStringType(readType) || isBytesType(readType) {
        var tmp : readType;
        _oldRead(reader, tmp);
        return tmp;
      } else if isEnumType(readType) {
        var ret = reader.withDeserializer(DefaultDeserializer).read(readType);
        return ret;
      } else if canResolveTypeMethod(readType, "deserializeFrom", reader, this) ||
                isArrayType(readType) {
        if isArrayType(readType) && chpl__domainFromArrayRuntimeType(readType).rank > 1 then
          throw new IllegalArgumentError("ChplSerializer does not support multidimensional arrays");
        var alias = reader.withDeserializer(new ChplDeserializer(_typename=readType:string));
        return readType.deserializeFrom(reader=alias, deserializer=alias.deserializer);
      } else {
        var alias = reader.withDeserializer(new ChplDeserializer(_typename=readType:string));
        return new readType(reader=alias, deserializer=alias.deserializer);
      }
    }

    proc deserializeValue(reader: _readerType, ref val: ?readType) : void throws {
      if canResolveMethod(val, "deserialize", reader, this) {
        if isArrayType(readType) && val.rank > 1 then
          throw new IllegalArgumentError("ChplSerializer does not support multidimensional arrays");
        var alias = reader.withDeserializer(new ChplDeserializer(_typename=readType:string));
        val.deserialize(reader=alias, deserializer=alias.deserializer);
      } else {
        val = deserializeType(reader, readType);
      }
    }

    @chpldoc.nodoc
    proc deserializeField(reader: _readerType, name: string, type T) throws {
      if !_firstThing then reader.readLiteral(",");
      else _firstThing = false;

      if !name.isEmpty() {
        reader.readLiteral(name);
        reader.readLiteral("=");
      }

      return reader.read(T);
    }

    // Class helpers
    @chpldoc.nodoc
    proc startClass(reader: _readerType, name: string) throws {
      // TODO: currently ignores 'name' because we can't know the name of the
      // type until it's initialized...
      if _inheritLevel == 0 {
        reader.readLiteral("new " + _typename);
        reader.readLiteral("(");
      }

      _inheritLevel += 1;
    }
    @chpldoc.nodoc
    proc endClass(reader: _readerType) throws {
      if _inheritLevel == 1 {
        reader.readLiteral(")");
      }
      _inheritLevel -= 1;
    }

    // Record helpers
    @chpldoc.nodoc
    proc startRecord(reader: _readerType, name: string) throws {
      reader.readLiteral("new " + _typename);
      reader.readLiteral("(");
    }
    @chpldoc.nodoc
    proc endRecord(reader: _readerType) throws {
      reader.readLiteral(")");
    }

    // Tuple helpers
    @chpldoc.nodoc
    proc startTuple(reader: _readerType) throws {
      reader.readLiteral("(");
    }
    @chpldoc.nodoc
    proc endTuple(reader: _readerType) throws {
      reader.readLiteral(")");
    }

    // List helpers
    @chpldoc.nodoc
    proc startList(reader: _readerType) throws {
      reader._readLiteral("[");
    }
    @chpldoc.nodoc
    proc readListElement(reader: _readerType, type eltType) throws {
      if !_firstThing then reader._readLiteral(", ");
      else _firstThing = false;

      return reader.read(eltType);
    }
    @chpldoc.nodoc
    proc endList(reader: _readerType) throws {
      reader._readLiteral("]");
    }

    // Array helpers
    @chpldoc.nodoc
    proc startArray(reader: _readerType) throws {
      startList(reader);
    }
    @chpldoc.nodoc
    proc startArrayDim(w: _readerType) throws {
    }
    @chpldoc.nodoc
    proc endArrayDim(w: _readerType) throws {
    }

    @chpldoc.nodoc
    proc readArrayElement(reader: _readerType, type eltType) throws {
      return readListElement(reader, eltType);
    }

    @chpldoc.nodoc
    proc endArray(reader: _readerType) throws {
      endList(reader);
    }

    // Map helpers
    @chpldoc.nodoc
    proc startMap(reader: _readerType) throws {
      reader._readLiteral("[");
    }

    @chpldoc.nodoc
    proc readKey(reader: _readerType, type keyType) throws {
      if !_firstThing then reader._readLiteral(",");
      else _firstThing = false;

      return reader.read(keyType);
    }

    @chpldoc.nodoc
    proc readValue(reader: _readerType, type valType) throws {
      reader._readLiteral("=>");
      return reader.read(valType);
    }

    @chpldoc.nodoc
    proc endMap(reader: _readerType) throws {
      reader._readLiteral("]");
    }
  }
}
