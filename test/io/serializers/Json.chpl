
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

    proc serializeValue(writer: _writeType, const val:?t) throws {
      if t == string  || isEnumType(t) || t == bytes {
        // for quotes around things
        _oldWrite(writer, val);
      } else if isNumericType(t) || isBoolType(t) {
        _oldWrite(writer, val);
      } else if isClassType(t) {
        if val == nil {
          writer._writeLiteral("null");
        } else {
          var alias = writer.withSerializer(new JsonSerializer());
          val!.serialize(writer=alias, serializer=alias.serializer);
        }
      } else {
        var alias = writer.withSerializer(new JsonSerializer());
        val.serialize(writer=alias, serializer=alias.serializer);
      }
    }

    proc serializeField(writer: _writeType, name: string, const val: ?T) throws {
      if !firstField then
        writer.writeLiteral(", ");

      if !name.isEmpty() {
        writer.write(name);
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

    proc startList(writer: _writeType, size: uint) throws {
      writer._writeLiteral("[");
      firstField = true;
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
    }

    proc startArrayDim(writer: _writeType, size: uint) throws {
      _arrayDim += 1;
      if _arrayFirst.size < _arrayDim {
        _arrayFirst.append(true);
      }

      if _arrayFirst[_arrayDim-1] {
        _arrayFirst[_arrayDim-1] = false;
      } else {
        writer._writeLiteral(",");
      }

      _arrayMax = max(_arrayMax, _arrayDim);

      if _arrayDim > 1 {
        writer.writeNewline();
        writer.writeLiteral(" " * (_arrayDim-1));
      }
      writer._writeLiteral("[");
    }

    proc endArrayDim(writer: _writeType) throws {
      if _arrayDim < _arrayMax {
        writer.writeNewline();
        writer._writeLiteral(" " * (_arrayDim-1));
      }
      writer._writeLiteral("]");

      if _arrayDim < _arrayFirst.size then
      _arrayFirst[_arrayDim] = true;

      _arrayDim -= 1;
      firstField = true;
    }

    proc writeArrayElement(writer: _writeType, const val: ?) throws {
      if !firstField then
        writer._writeLiteral(", ");
      else
        firstField = false;
      writer.write(val);
    }

    proc endArray(writer: _writeType) throws {
    }

    proc startMap(writer: _writeType, size: uint) throws {
      writer._writeLiteral("{");
    }

    proc writeKey(writer: _writeType, const key: ?) throws {
      if !firstField {
        writer._writeLiteral(", ");
        writer.writeNewline();
        writer._writeLiteral("  ");
      } else {
        writer.writeNewline();
        writer._writeLiteral("  ");
        firstField = false;
      }

      if key.type == string {
        writer.write(key);
      } else {
        // Write the key as json, then turn it into a json string to use
        // it as a proper key for the map.
        var f = openMemFile();
        {
          f.writer().withSerializer(JsonSerializer).write(key);
        }
        var tmp : string;
        f.reader().readAll(tmp);
        writer.write(tmp);
      }
    }

    proc writeValue(writer: _writeType, const val: ?) throws {
      writer._writeLiteral(": ");
      writer.write(val);
    }

    proc endMap(writer: _writeType) throws {
      writer.writeNewline();
      writer._writeLiteral("}");
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

    proc deserializeType(reader:fileReader, type readType) : readType throws {
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

    proc deserializeValue(reader: fileReader, ref val: ?readType) : void throws {
      if canResolveMethod(val, "deserialize", reader, this) {
        var alias = reader.withDeserializer(new JsonDeserializer());
        val.deserialize(reader=alias, deserializer=alias.deserializer);
      } else {
        val = deserializeType(reader, readType);
      }
    }

    proc _readFieldName(reader: _readerT, key: string) throws {
      try {
        reader._readLiteral('"');
        reader._readLiteral(key);
        reader._readLiteral('"');
        reader._readLiteral(":");
      } catch e: BadFormatError {
        return false;
      }

      return true;
    }


    proc deserializeField(reader: _readerT, name: string, type T) throws {
      if _names.contains(name) {
        // Use 'advance' instead of 'seek' to support reading in a marked
        // channel, which can happen during 'readf'.
        //
        // Use 'mark' to rewind the position since 'advance' doesn't support
        // negative values. This means that 'deserializeField' does not advance
        // the channel's position until 'endClass' or 'endRecord' are called.
        reader.mark();
        const dist =  _offsets[name] - reader.offset();
        reader.advance(dist);
      } else if !name.isEmpty() {
        throw new IllegalArgumentError("field '" + name + "' not found.");
      }

      var ret = reader.read(T);

      // note: trailing commas not allowed in json
      reader.matchLiteral(",");
      if !name.isEmpty() then
        reader.revert();

      return ret;
    }

    proc startTuple(reader: fileReader) throws {
      reader.readLiteral("[");
    }
    proc endTuple(reader: fileReader) throws {
      reader.readLiteral("]");
    }

    proc startClass(reader: fileReader, name: string) throws {
      _startComposite(reader);
    }
    proc endClass(reader: fileReader) throws {
      _endComposite(reader);
    }

    proc startRecord(reader: fileReader, name: string) throws {
      _startComposite(reader);
    }
    proc endRecord(reader: fileReader) throws {
      _endComposite(reader);
    }

    proc _startComposite(reader: fileReader) throws {
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
        var (m, last) = helper(reader);
        for (k, v) in zip(m.keys(), m.values()) {
          _names.add(k);
          _offsets[k] = v;
        }
        _lastPos = last;
        reader.readLiteral("{");
      }

      _inheritLevel += 1;
    }

    proc _endComposite(reader: fileReader) throws {
      if _inheritLevel == 1 {
        const dist =  _lastPos - reader.offset();
        reader.advance(dist);
        reader.readLiteral("}");
      }
      _inheritLevel -= 1;
    }

    proc startList(reader: fileReader) throws {
      reader._readLiteral("[");
    }
    proc readListElement(reader: fileReader, type eltType) throws {
      if !firstField then reader._readLiteral(",");
      else firstField = false;

      return reader.read(eltType);
    }
    proc endList(reader: fileReader) throws {
      reader._readLiteral("]");
    }

    proc startArray(reader: fileReader) throws {
    }

    proc startArrayDim(reader: fileReader) throws {
      _arrayDim += 1;
      if _arrayFirst.size < _arrayDim {
        _arrayFirst.append(true);
      }

      if _arrayFirst[_arrayDim-1] {
        _arrayFirst[_arrayDim-1] = false;
      } else {
        reader._readLiteral(",");
      }

      _arrayMax = max(_arrayMax, _arrayDim);

      // Don't need to read the newline and pretty-printed spaces, as JSON
      // arrays can come in other forms. Relies on 'readLiteral' ignoring
      // whitespace by default.
      reader._readLiteral("[");
    }

    proc readArrayElement(reader: fileReader, type eltType) throws {
      if !firstField then
        reader._readLiteral(", ");
      else
        firstField = false;
      return reader.read(eltType);
    }

    proc endArrayDim(reader: fileReader) throws {
      if _arrayDim < _arrayMax {
        reader.readNewline();
        reader._readLiteral(" " * (_arrayDim-1));
      }

      // Don't need to read the newline and pretty-printed spaces, as JSON
      // arrays can come in other forms. Relies on 'readLiteral' ignoring
      // whitespace by default.
      reader._readLiteral("]");

      if _arrayDim < _arrayFirst.size then
        _arrayFirst[_arrayDim] = true;

      _arrayDim -= 1;
      firstField = true;
    }

    proc endArray(reader: fileReader) throws {
    }

    proc startMap(reader: fileReader) throws {
      reader._readLiteral("{");
    }

    proc readKey(reader: fileReader, type keyType) throws {
      if !firstField {
        reader._readLiteral(",");
      } else {
        firstField = false;
      }

      if keyType == string {
        return reader.read(string);
      } else {
        var f = openMemFile();
        var s = reader.read(string);
        {
          f.writer().withSerializer(DefaultSerializer).write(s);
        }
        return f.reader().withDeserializer(JsonDeserializer).read(keyType);
      }
    }

    proc readValue(reader: fileReader, type valType) throws {
      reader._readLiteral(":");
      return reader.read(valType);
    }

    proc endMap(reader: fileReader) throws {
      reader._readLiteral("}");
    }
  }
}
