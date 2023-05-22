
module ChplFormat {
  private use IO;
  private use CTypes;
  private use Map;
  private use List;

  type _writeType = fileWriter(serializerType=ChplSerializer, ?);
  type _readerT = fileReader(deserializerType=ChplDeserializer, ?);

  record ChplSerializer {
    var firstField = true;
    var _inheritLevel = 0;
    var _arrayDim = 0;
    var _arrayMax = 0;
    var _arrayFirst : list(bool);
    var _typename : string;

    // TODO: rewrite in terms of writef, or something
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

    proc serializeField(writer: _writeType, name: string, const val: ?T) throws {
      if !firstField then
        writer.writeLiteral(", ");

      if !name.isEmpty() {
        writer.writeLiteral(name);
        writer.writeLiteral(" = ");
      }

      writer.write(val);
      firstField = false;
    }

    // TODO: How should generic types be printed?
    // - 'new G(int,real)(5, 42.0)' vs 'new G(int, real, 5, 42.0)'
    // - is the latter even possible?
    proc startClass(writer: _writeType, name: string, size: int) throws {
      if _inheritLevel == 0 {
        writer._writeLiteral("new ");
        writer._writeLiteral(_typename);
        writer.writeLiteral("(");
      }

      _inheritLevel += 1;
    }
    proc endClass(writer: _writeType) throws {
      if _inheritLevel == 1 {
        writer.writeLiteral(")");
      }

      _inheritLevel -= 1;
    }

    proc startRecord(writer: _writeType, name: string, size: int) throws {
      writer._writeLiteral("new ");
      writer._writeLiteral(_typename);
      writer.writeLiteral("(");
    }
    proc endRecord(writer: _writeType) throws {
      writer.writeLiteral(")");
    }

    proc startTuple(writer: _writeType, size: int) throws {
      writer.writeLiteral("(");
    }
    proc endTuple(writer: _writeType) throws {
      writer.writeLiteral(")");
    }

    proc startList(writer: _writeType, size: uint) throws {
      writer._writeLiteral("[");
    }
    proc writeListElement(writer: _writeType, const val: ?) throws {
      if !firstField then writer._writeLiteral(", ");
      else firstField = false;

      writer.write(val);
    }
    proc endList(writer: _writeType) throws {
      writer._writeLiteral("]");
    }

    proc startArray(writer: _writeType, size: uint) throws {
      startList(writer, size);
    }

    proc startArrayDim(writer: _writeType, size: uint) throws {
    }
    proc endArrayDim(writer: _writeType) throws {
    }

    proc writeArrayElement(writer: _writeType, const val: ?) throws {
      writeListElement(writer, val);
    }

    proc endArray(writer: _writeType) throws {
      endList(writer);
    }

    proc startMap(writer: _writeType, size: uint) throws {
      writer._writeLiteral("[");
    }

    proc writeKey(writer: _writeType, const key: ?) throws {
      if !firstField {
        writer._writeLiteral(", ");
      } else {
        firstField = false;
      }

      writer.write(key);
    }

    proc writeValue(writer: _writeType, const val: ?) throws {
      writer._writeLiteral(" => ");
      writer.write(val);
    }

    proc endMap(writer: _writeType) throws {
      writer._writeLiteral("]");
    }
  }

  // TODO: out of order reading
  record ChplDeserializer {
    var _inheritLevel = 0;
    var _typename : string;

    var firstField = true;
    var _arrayDim = 0;
    var _arrayMax = 0;
    var _arrayFirst : list(bool);

    // TODO: rewrite in terms of writef, or something
    proc _oldRead(ch: _readerT, ref val:?t) throws {
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

    proc deserializeType(reader:fileReader, type readType) : readType throws {
      if isNilableClassType(readType) && reader.matchLiteral("nil") {
        return nil:readType;
      }

      if isNumericType(readType) || isBoolType(readType) {
        var x : readType;
        reader._readOne(reader.kind, x, here);
        return x;
      } else if isStringType(readType) {
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

    proc deserializeValue(reader: fileReader, ref val: ?readType) : void throws {
      if canResolveMethod(val, "deserialize", reader, this) {
        if isArrayType(readType) && chpl__domainFromArrayRuntimeType(readType).rank > 1 then
          throw new IllegalArgumentError("ChplSerializer does not support multidimensional arrays");
        var alias = reader.withDeserializer(new ChplDeserializer(_typename=readType:string));
        val.deserialize(reader=alias, deserializer=alias.deserializer);
      } else {
        val = deserializeType(reader, readType);
      }
    }

    proc deserializeField(reader: _readerT, name: string, type T) throws {
      if !firstField then
        reader.readLiteral(",");
      firstField = false;

      if !name.isEmpty() {
        reader.readLiteral(name);
        reader.readLiteral("=");
      }
      return reader.read(T);
    }

    proc startTuple(reader: fileReader) throws {
      reader.readLiteral("(");
    }
    proc endTuple(reader: fileReader) throws {
      reader.readLiteral(")");
    }

    proc startClass(reader: fileReader, name: string) throws {
      _startComposite(reader, _typename);
    }
    proc endClass(reader: fileReader) throws {
      _endComposite(reader);
    }

    proc startRecord(reader: fileReader, name: string) throws {
      _startComposite(reader, _typename);
    }
    proc endRecord(reader: fileReader) throws {
      _endComposite(reader);
    }

    proc _startComposite(reader: fileReader, name: string) throws {
      if _inheritLevel == 0 {
        reader.readLiteral("new " + name);
        reader.readLiteral("(");
      }

      _inheritLevel += 1;
    }

    proc _endComposite(reader: fileReader) throws {
      if _inheritLevel == 1 {
        reader.readLiteral(")");
      }
      _inheritLevel -= 1;
    }

    proc startList(reader: fileReader) throws {
      reader._readLiteral("[");
    }
    proc readListElement(reader: fileReader, type eltType) throws {
      if !firstField then
        reader._readLiteral(", ");
      else
        firstField = false;
      return reader.read(eltType);
    }
    proc endList(reader: fileReader) throws {
      reader._readLiteral("]");
    }

    proc startArray(reader: fileReader) throws {
      startList(reader);
    }

    proc startArrayDim(w: _readerT) throws {
    }
    proc endArrayDim(w: _readerT) throws {
    }

    proc readArrayElement(reader: fileReader, type eltType) throws {
      return readListElement(reader, eltType);
    }

    proc endArray(reader: fileReader) throws {
      endList(reader);
    }

    proc startMap(reader: fileReader) throws {
      reader._readLiteral("[");
    }

    proc readKey(reader: fileReader, type keyType) throws {
      if !firstField {
        reader._readLiteral(",");
      } else {
        firstField = false;
      }

      return reader.read(keyType);
    }

    proc readValue(reader: fileReader, type valType) throws {
      reader._readLiteral("=>");
      return reader.read(valType);
    }

    proc endMap(reader: fileReader) throws {
      reader._readLiteral("]");
    }
  }
}
