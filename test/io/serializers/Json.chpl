
module Json {
  private use IO;
  private use CTypes;
  private use Map;
  private use List;

  type _writeType = fileWriter(serializerType=JsonSerializer, ?);
  type _readerT = fileReader(deserializerType=JsonDeserializer, ?);

  record JsonSerializer {
    var firstField = true;
    var _inheritLevel = 0;
    var _arrayDim = 0;
    var _arrayMax = 0;
    var _arrayFirst : list(bool);

    // TODO: rewrite in terms of writef, or something
    proc _oldWrite(ch: _writeType, const val:?t) throws {
      var _def = new DefaultSerializer();
      var dc = ch.withSerializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }
      st.realfmt = 2;
      st.string_format = iostringformat.json:uint(8);
      st.aggregate_style = QIO_AGGREGATE_FORMAT_JSON:uint(8);
      st.array_style = QIO_ARRAY_FORMAT_JSON:uint(8);
      st.tuple_style = QIO_TUPLE_FORMAT_JSON:uint(8);
      dc._set_styleInternal(st);
      dc._writeOne(dc.kind, val, here);
    }

    proc serializeValue(writer: _writeType, const x:?t) throws {
      if t == string  || isEnumType(t) || t == bytes {
        // for quotes around things
        _oldWrite(writer, x);
      } else if isNumericType(t) || isBoolType(t) {
        _oldWrite(writer, x);
      } else if isClassType(t) {
        if x == nil {
          writer._writeLiteral("null");
        } else {
          var alias = writer.withSerializer(new JsonSerializer());
          x!.serialize(writer=alias, serializer=alias.serializer);
        }
      } else {
        var alias = writer.withSerializer(new JsonSerializer());
        x.serialize(writer=alias, serializer=alias.serializer);
      }
    }

    proc serializeField(writer: _writeType, key: string, const val: ?T) throws {
      if !firstField then
        writer.writeLiteral(", ");

      if !key.isEmpty() {
        writer.write(key);
        writer.writeLiteral(":");
      }

      writer.write(val);
      firstField = false;
    }

    proc startClass(writer: _writeType, name: string, size: int) throws {
      if _inheritLevel == 0 {
        writer.writeLiteral("{");
      }

      _inheritLevel += 1;
    }
    proc endClass(writer: _writeType) throws {
      if _inheritLevel == 1 {
        writer.writeLiteral("}");
      }

      _inheritLevel -= 1;
    }

    proc startRecord(writer: _writeType, name: string, size: int) throws {
      writer.writeLiteral("{");
    }
    proc endRecord(writer: _writeType) throws {
      writer.writeLiteral("}");
    }

    proc startTuple(writer: _writeType, size: int) throws {
      writer.writeLiteral("[");
    }
    proc endTuple(writer: _writeType) throws {
      writer.writeLiteral("]");
    }

    proc startArray(w: _writeType, _size:uint = 0) throws {
      _arrayDim += 1;
      if _arrayFirst.size < _arrayDim {
        _arrayFirst.append(true);
      }

      if _arrayFirst[_arrayDim-1] {
        _arrayFirst[_arrayDim-1] = false;
      } else {
        w._writeLiteral(",");
      }

      _arrayMax = max(_arrayMax, _arrayDim);

      if _arrayDim > 1 {
        w.writeNewline();
        w.writeLiteral(" " * (_arrayDim-1));
      }
      w._writeLiteral("[");
    }

    proc writeArrayElement(w: _writeType, const val: ?) throws {
      if !firstField then
        w._writeLiteral(", ");
      else
        firstField = false;
      w.write(val);
    }

    proc endArray(w: _writeType) throws {
      if _arrayDim < _arrayMax {
        w.writeNewline();
        w._writeLiteral(" " * (_arrayDim-1));
      }
      w._writeLiteral("]");

      if _arrayDim < _arrayFirst.size then
      _arrayFirst[_arrayDim] = true;

      _arrayDim -= 1;
      firstField = true;
    }

    proc startMap(w: _writeType, _size:uint = 0) throws {
      w._writeLiteral("{");
    }

    proc writeKey(w: _writeType, const key: ?) throws {
      if !firstField {
        w._writeLiteral(", ");
        w.writeNewline();
        w._writeLiteral("  ");
      } else {
        w.writeNewline();
        w._writeLiteral("  ");
        firstField = false;
      }

      if key.type == string {
        w.write(key);
      } else {
        // Write the key as json, then turn it into a json string to use
        // it as a proper key for the map.
        var f = openMemFile();
        {
          f.writer().withSerializer(JsonSerializer).write(key);
        }
        var tmp : string;
        f.reader().readAll(tmp);
        w.write(tmp);
      }
    }

    proc writeValue(w: _writeType, const val: ?) throws {
      w._writeLiteral(": ");
      w.write(val);
    }

    proc endMap(w: _writeType) throws {
      w.writeNewline();
      w._writeLiteral("}");
    }
  }

  private extern proc qio_channel_skip_json_field(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;

  // Read the JSON object in ahead of time, and produce a map of field names
  // to file offsets that we can look at later in deserializeField.
  // TODO: use a proper JSON library.
  private proc helper(reader: _readerT) throws {
    var m : map(string, int);
    reader.mark();

    reader.readLiteral("{");
    var done = false;
    var lastPos = -1;
    while !done {
      if reader.matchLiteral("}") {
        lastPos = reader.offset() - 1;
        done = true;
        break;
      }

      reader.mark();
      const name = reader.read(string);
      reader.readLiteral(":");
      m.add(name, reader.offset());
      reader.revert();

      qio_channel_skip_json_field(false, reader._channel_internal);
      reader.matchLiteral(",");
    }

    reader.revert();

    return (m, lastPos);
  }

  record JsonDeserializer {
    var outOfOrder = false;
    var _inheritLevel = 0;
    var _names : domain(string);
    var _offsets : [_names] int;
    var _lastPos = -1;

    var firstField = true;
    var _arrayDim = 0;
    var _arrayMax = 0;
    var _arrayFirst : list(bool);

    proc init() {
      this.complete();
    }

    // TODO: rewrite in terms of writef, or something
    proc _oldRead(ch: _readerT, ref val:?t) throws {
      var _def = new DefaultDeserializer();
      var dc = ch.withDeserializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }
      st.realfmt = 2;
      st.string_format = iostringformat.json:uint(8);
      st.aggregate_style = QIO_AGGREGATE_FORMAT_JSON:uint(8);
      st.array_style = QIO_ARRAY_FORMAT_JSON:uint(8);
      st.tuple_style = QIO_TUPLE_FORMAT_JSON:uint(8);
      dc._set_styleInternal(st);
      dc._readOne(dc.kind, val, here);
    }

    proc deserialize(reader:fileReader, type readType) : readType throws {
      if isNilableClassType(readType) && reader.matchLiteral("null") {
        return nil:readType;
      }

      // TODO:
      // - escaped strings
      // - arrays/lists
      if isNumericType(readType) || isBoolType(readType) {
        var x : readType;
        reader._readOne(reader.kind, x, here);
        return x;
      } else if isStringType(readType) {
        // TODO: Ideally:
        //reader.readf("%{\"S\"}", tmp);
        var tmp : readType;
        _oldRead(reader, tmp);
        return tmp;
      } else if isEnumType(readType) {
        reader.readLiteral('"');
        var ret = reader.withDeserializer(DefaultDeserializer).read(readType);
        reader.readLiteral('"');
        return ret;
      } else if canResolveTypeMethod(readType, "deserializeFrom", reader, this) ||
                isArrayType(readType) {
        var alias = reader.withDeserializer(new JsonDeserializer());
        return readType.deserializeFrom(reader=alias, deserializer=alias.deserializer);
      } else {
        var alias = reader.withDeserializer(new JsonDeserializer());
        return new readType(reader=alias, deserializer=alias.deserializer);
      }
    }

    proc _readFieldName(r: _readerT, key: string) throws {
      try {
        r._readLiteral('"');
        r._readLiteral(key);
        r._readLiteral('"');
        r._readLiteral(":");
      } catch e: BadFormatError {
        return false;
      }

      return true;
    }


    proc deserializeField(r: _readerT, key: string, type T) throws {
      if _names.contains(key) {
        r.seek(_offsets[key]..);
      } else if !key.isEmpty() {
        throw new Error("field not found...");
      }

      var ret = r.read(T);

      // note: trailing commas not allowed in json
      r.matchLiteral(",");

      return ret;
    }

    proc startTuple(r: fileReader, size: int) throws {
      r.readLiteral("[");
    }
    proc endTuple(r: fileReader) throws {
      r.readLiteral("]");
    }

    proc startClass(r: fileReader, name: string, size: int) throws {
      _startComposite(r, size);
    }
    proc endClass(r: fileReader) throws {
      _endComposite(r);
    }

    proc startRecord(r: fileReader, name: string, size: int) throws {
      _startComposite(r, size);
    }
    proc endRecord(r: fileReader) throws {
      _endComposite(r);
    }

    proc _startComposite(r: fileReader, size: int) throws {
      if _inheritLevel == 0 {
        //
        // TODO: When should we try to do this? Use of '_startComposite', etc.
        // likely indicates "buying in" to the JSON format, but in a way
        // it might be cleaner or more efficient to do when creating the
        // new temporary JsonDeserializer for the given type. But if we do the
        // JSON parsing at that point, it ignores the user-defined
        // initializer. So, we do it here in '_startComposite'.
        //
        // TODO: Should we only compute the mapping if the fields are being
        // read out of order?
        //
        var (m, last) = helper(r);
        for (k, v) in zip(m.keys(), m.values()) {
          _names.add(k);
          _offsets[k] = v;
        }
        _lastPos = last;
        r.readLiteral("{");
      }

      _inheritLevel += 1;
    }

    proc _endComposite(r: fileReader) throws {
      if _inheritLevel == 1 {
        r.seek(_lastPos..);
        r.readLiteral("}");
      }
      _inheritLevel -= 1;
    }

    proc startArray(r: fileReader) throws {
      _arrayDim += 1;
      if _arrayFirst.size < _arrayDim {
        _arrayFirst.append(true);
      }

      if _arrayFirst[_arrayDim-1] {
        _arrayFirst[_arrayDim-1] = false;
      } else {
        r._readLiteral(",");
      }

      _arrayMax = max(_arrayMax, _arrayDim);

      // Don't need to read the newline and pretty-printed spaces, as JSON
      // arrays can come in other forms. Relies on 'readLiteral' ignoring
      // whitespace by default.
      r._readLiteral("[");
    }

    proc readArrayElement(r: fileReader, type eltType) throws {
      if !firstField then
        r._readLiteral(", ");
      else
        firstField = false;
      return r.read(eltType);
    }

    proc endArray(r: fileReader) throws {
      if _arrayDim < _arrayMax {
        r.readNewline();
        r._readLiteral(" " * (_arrayDim-1));
      }

      // Don't need to read the newline and pretty-printed spaces, as JSON
      // arrays can come in other forms. Relies on 'readLiteral' ignoring
      // whitespace by default.
      r._readLiteral("]");

      if _arrayDim < _arrayFirst.size then
        _arrayFirst[_arrayDim] = true;

      _arrayDim -= 1;
      firstField = true;
    }

    proc startMap(r: fileReader) throws {
      r._readLiteral("{");
    }

    proc readKey(r: fileReader, type keyType) throws {
      if !firstField {
        r._readLiteral(",");
      } else {
        firstField = false;
      }

      if keyType == string {
        return r.read(string);
      } else {
        var f = openMemFile();
        var s = r.read(string);
        {
          f.writer().withSerializer(DefaultSerializer).write(s);
        }
        return f.reader().withDeserializer(JsonDeserializer).read(keyType);
      }
    }

    proc readValue(r: fileReader, type valType) throws {
      r._readLiteral(":");
      return r.read(valType);
    }

    proc endMap(r: fileReader) throws {
      r._readLiteral("}");
    }
  }
}
