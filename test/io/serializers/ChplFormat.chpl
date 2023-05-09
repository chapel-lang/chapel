
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

    proc serializeValue(writer: _writeType, const x:?t) throws {
      if t == string  || isEnumType(t) || t == bytes {
        _oldWrite(writer, x);
      } else if isNumericType(t) || isBoolType(t) {
        _oldWrite(writer, x);
      } else if isClassType(t) {
        if x == nil {
          writer._writeLiteral("nil");
        } else {
          var alias = writer.withSerializer(new ChplSerializer(_typename=t:string));
          x!.serialize(writer=alias, serializer=alias.serializer);
        }
      } else {
        if isArray(x) && x.rank > 1 then
          throw new IllegalArgumentError("ChplSerializer does not support multidimensional arrays");
        var alias = writer.withSerializer(new ChplSerializer(_typename=t:string));
        x.serialize(writer=alias, serializer=alias.serializer);
      }
    }

    proc serializeField(writer: _writeType, key: string, const val: ?T) throws {
      if !firstField then
        writer.writeLiteral(", ");

      if !key.isEmpty() {
        writer.writeLiteral(key);
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

    proc startArray(w: _writeType, _size:uint = 0) throws {
      w._writeLiteral("[");
    }

    proc startArrayDim(w: _writeType, len: uint) throws {
    }
    proc endArrayDim(w: _writeType) throws {
    }

    proc writeArrayElement(w: _writeType, const val: ?) throws {
      if !firstField then
        w._writeLiteral(", ");
      else
        firstField = false;
      w.write(val);
    }

    proc endArray(w: _writeType) throws {
      w._writeLiteral("]");
    }

    proc startMap(w: _writeType, _size:uint = 0) throws {
      w._writeLiteral("[");
    }

    proc writeKey(w: _writeType, const key: ?) throws {
      if !firstField {
        w._writeLiteral(", ");
      } else {
        firstField = false;
      }

      w.write(key);
    }

    proc writeValue(w: _writeType, const val: ?) throws {
      w._writeLiteral(" => ");
      w.write(val);
    }

    proc endMap(w: _writeType) throws {
      w._writeLiteral("]");
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

    proc deserialize(reader:fileReader, type readType) : readType throws {
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

    proc deserializeField(r: _readerT, key: string, type T) throws {
      if !firstField then
        r.readLiteral(",");
      firstField = false;

      if !key.isEmpty() {
        r.readLiteral(key);
        r.readLiteral("=");
      }
      return r.read(T);
    }

    proc startTuple(r: fileReader, size: int) throws {
      r.readLiteral("(");
    }
    proc endTuple(r: fileReader) throws {
      r.readLiteral(")");
    }

    proc startClass(r: fileReader, name: string, size: int) throws {
      _startComposite(r, _typename, size);
    }
    proc endClass(r: fileReader) throws {
      _endComposite(r);
    }

    proc startRecord(r: fileReader, name: string, size: int) throws {
      _startComposite(r, _typename, size);
    }
    proc endRecord(r: fileReader) throws {
      _endComposite(r);
    }

    proc _startComposite(r: fileReader, name: string, size: int) throws {
      if _inheritLevel == 0 {
        r.readLiteral("new " + name);
        r.readLiteral("(");
      }

      _inheritLevel += 1;
    }

    proc _endComposite(r: fileReader) throws {
      if _inheritLevel == 1 {
        r.readLiteral(")");
      }
      _inheritLevel -= 1;
    }

    proc startArray(r: fileReader) throws {
      r._readLiteral("[");
    }

    proc startArrayDim(w: _readerT) throws {
    }
    proc endArrayDim(w: _readerT) throws {
    }

    proc readArrayElement(r: fileReader, type eltType) throws {
      if !firstField then
        r._readLiteral(", ");
      else
        firstField = false;
      return r.read(eltType);
    }

    proc endArray(r: fileReader) throws {
      r._readLiteral("]");
    }

    proc startMap(r: fileReader) throws {
      r._readLiteral("[");
    }

    proc readKey(r: fileReader, type keyType) throws {
      if !firstField {
        r._readLiteral(",");
      } else {
        firstField = false;
      }

      return r.read(keyType);
    }

    proc readValue(r: fileReader, type valType) throws {
      r._readLiteral("=>");
      return r.read(valType);
    }

    proc endMap(r: fileReader) throws {
      r._readLiteral("]");
    }
  }
}
