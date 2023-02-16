
module Json {
  private use IO;
  private use CTypes;
  private use Map;

  type _writeType = fileWriter(fmtType=JsonWriter);
  type _readerT = fileReader(fmtType=JsonReader);
  record JsonWriter {
    var firstField = true;
    var _inheritLevel = 0;

    // TODO: rewrite in terms of writef, or something
    proc _oldWrite(ch: _writeType, const val:?t) throws {
      var _def = new DefaultWriter();
      var dc = ch.withFormatter(_def);
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

    proc encode(writer: _writeType, const x:?t) throws {
      if t == string  || isEnumType(t) || t == bytes {
        writer.writeLiteral('"');
        writer.withFormatter(DefaultWriter).write(x);
        writer.writeLiteral('"');
      } else if isNumericType(t) || isBoolType(t) {
        _oldWrite(writer, x);
      } else if t == ioLiteral {
        writer._writeLiteral(x.val);
      } else if t == ioNewline {
        // TODO: Need to account for 'skipWhitespaceOnly' somehow...
        writer._writeOne(writer.kind, x, writer.getLocaleOfIoRequest());
      } else if isClassType(t) {
        if x == nil {
          writer._writeLiteral("null");
        } else {
          x!.encodeTo(writer.withFormatter(new JsonWriter()));
        }
      } else {
        x.encodeTo(writer.withFormatter(new JsonWriter()));
      }
    }

    proc writeField(writer: _writeType, key: string, const val: ?T) throws {
      if !firstField then
        writer.writeLiteral(", ");

      if !key.isEmpty() {
        writer.write(key);
        writer.writeLiteral(":");
      }

      writer.write(val);
      firstField = false;
    }

    proc writeTypeStart(writer: _writeType, type T) throws {
      if _inheritLevel == 0 {
        if isClassType(T) || isRecordType(T) {
          writer.writeLiteral("{");
        } else if isTupleType(T) {
          writer.writeLiteral("[");
        }
      }

      _inheritLevel += 1;
    }

    proc writeTypeEnd(writer: _writeType, type T) throws {
      if _inheritLevel == 1 {
        if isClassType(T) || isRecordType(T) {
          writer.writeLiteral("}");
        } else if isTupleType(T) {
          writer.writeLiteral("]");
        }
      }

      _inheritLevel -= 1;
    }
  }

  private extern proc qio_channel_skip_json_field(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;

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
      m[name] = reader.offset();
      reader.revert();

      qio_channel_skip_json_field(false, reader._channel_internal);
      reader.matchLiteral(",");
    }

    reader.revert();

    return (m, lastPos);
  }

  record JsonReader {
    var outOfOrder = false;
    var _inheritLevel = 0;
    var _names : domain(string);
    var _offsets : [_names] int;
    var _lastPos = -1;

    proc init() {
      this.complete();
    }

    proc decode(reader:fileReader, type readType) : readType throws {
      // TODO:
      // - escaped strings
      // - arrays/lists
      if isNumericType(readType) || isBoolType(readType) {
        var x : readType;
        reader._readOne(reader.kind, x, here);
        return x;
      } else if isStringType(readType) {
        var tmp : readType;
        reader.readf("%{\"S\"}", tmp);
        return tmp;
      } else if isEnumType(readType) {
        reader.readLiteral('"');
        var ret = reader.withFormatter(DefaultReader).read(readType);
        reader.readLiteral('"');
        return ret;
      } else if isClassType(readType) {
        if isNilableClassType(readType) && reader.matchLiteral("null") {
          return nil:readType;
        } else {
          return new readType(reader.withFormatter(new JsonReader()));
        }
      } else if canResolveTypeMethod(readType, "decodeFrom", reader) {
        return readType.decodeFrom(reader.withFormatter(new JsonReader()));
      } else {
        return new readType(reader.withFormatter(new JsonReader()));
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


    proc readField(r: _readerT, key: string, type T) throws {
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

    proc readTypeStart(r: fileReader, type T) throws {
      if _inheritLevel == 0 {
        if isClassType(T) || isRecordType(T) {
          //
          // TODO: When should we try to do this? Use of 'readTypeStart'
          // likely indicates "buying in" to the JSON format, but in a way
          // it might be cleaner or more efficient to do when creating the
          // new temporary JsonReader for the given type. But if we do the
          // JSON parsing at that point, it ignores the user-defined
          // initializer. So, we do it here in 'readTypeStart'.
          //
          // TODO: Should we only compute the mapping if the fields are being
          // read out of order?
          //
          var (m, last) = helper(r);
          for (k, v) in m.items() {
            _names.add(k);
            _offsets[k] = v;
          }
          _lastPos = last;
          r.readLiteral("{");
        } else if isTupleType(T) {
          r.readLiteral("[");
        }
      }

      _inheritLevel += 1;
    }

    proc readTypeEnd(r: fileReader, type T) throws {
      if _inheritLevel == 1 {
        if isClassType(T) || isRecordType(T) {
          r.seek(_lastPos..);
          r.readLiteral("}");
        } else if isTupleType(T) {
          r.readLiteral("]");
        }
      }
    }
  }
}
