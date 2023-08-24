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
 The Json module provides a JsonSerializer and JsonDeserializer that allow
 for reading and writing data in the JSON format.
 */
@unstable("JSON module is considered unstable pending naming changes")
module JSON {
  private use IO;
  private use CTypes;
  private use Map;
  private use List;

  @chpldoc.nodoc
  type _writeType = fileWriter(serializerType=JsonSerializer, ?);
  @chpldoc.nodoc
  type _readerType = fileReader(deserializerType=JsonDeserializer, ?);

  record JsonSerializer {
    // TODO: rewrite in terms of writef, or something
    @chpldoc.nodoc
    proc _oldWrite(ch: _writeType, const val:?t) throws {
      var _def = new DefaultSerializer();
      var dc = ch.withSerializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }
      st.realfmt = 2;
      st.string_format = iostringformatInternal.json:uint(8);
      st.aggregate_style = QIO_AGGREGATE_FORMAT_JSON:uint(8);
      st.array_style = QIO_ARRAY_FORMAT_JSON:uint(8);
      st.tuple_style = QIO_TUPLE_FORMAT_JSON:uint(8);
      dc._set_styleInternal(st);
      dc._writeOne(dc._kind, val, here);
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
          val!.serialize(writer=writer, serializer=this);
        }
      } else {
        val.serialize(writer=writer, serializer=this);
      }
    }

    record AggregateSerializer {
      var writer;
      var _parent : bool = false;
      var _first : bool = true;
      const _ending : string;

      proc serializeField(name: string, const val: ?) throws {
        if !_first then writer._writeLiteral(", ");
        else _first = false;

        writer.write(name);
        writer._writeLiteral(":");
        writer.write(val);
      }

      proc startClass(writer, name: string, size: int) throws {
        _first = size == 0;
        return new AggregateSerializer(writer, _parent=true);
      }

      @chpldoc.nodoc
      proc endClass() throws {
        if !_parent then
          writer._writeLiteral(_ending);
      }

      @chpldoc.nodoc
      proc endRecord() throws {
        writer._writeLiteral(_ending);
      }
    }

    @chpldoc.nodoc
    proc startClass(writer: _writeType, name: string, size: int) throws {
      writer.writeLiteral("{");
      return new AggregateSerializer(writer, _ending="}");
    }

    @chpldoc.nodoc
    proc startRecord(writer: _writeType, name: string, size: int) throws {
      writer.writeLiteral("{");
      return new AggregateSerializer(writer, _ending="}");
    }

    record ListSerializer {
      var writer;
      var _first : bool = true;

      proc writeElement(const val: ?) throws {
        if !_first then writer._writeLiteral(", ");
        else _first = false;

        writer.write(val);
      }

      @chpldoc.nodoc
      proc endList() throws {
        writer.writeLiteral("]");
      }

      @chpldoc.nodoc
      proc endTuple() throws {
        endList();
      }
    }

    @chpldoc.nodoc
    proc startTuple(writer: _writeType, size: int) throws {
      writer.writeLiteral("[");
      return new ListSerializer(writer);
    }

    @chpldoc.nodoc
    proc startList(writer: _writeType, size: uint) throws {
      writer.writeLiteral("[");
      return new ListSerializer(writer);
    }

    record ArraySerializer {
      var writer;
      var _arrayDim : int;
      var _arrayFirst : list(bool);
      var _arrayMax : int;
      var _first : bool = true;

      // Write out multidimensional arrays like so:
      // [
      //  [0, 1, 2],
      //  [3, 4, 5],
      //  [6, 7, 8]
      // ]
      @chpldoc.nodoc
      proc startDim(size: uint) throws {
        _arrayDim += 1;

        // '_arrayFirst' will be a list of bools of a size equal to the maximum
        // dimension encountered so far.
        if _arrayFirst.size < _arrayDim {
          _arrayFirst.pushBack(true);
        }

        // Look at the previous dimension's 'first' indicator. If this is the
        // first nested dimension inside the previous dimension, then we set
        // the previous dimension's 'first' indicator to false.
        //
        // If this isn't the first nested dimension, print a comma.
        //
        // In the example above, "[0, 1, 2]" is the first, and ",\n[3, 4, 5]" is
        // the second.
        if _arrayFirst[_arrayDim-1] {
          _arrayFirst[_arrayDim-1] = false;
        } else {
          writer._writeLiteral(",");
        }

        _arrayMax = max(_arrayMax, _arrayDim);

        // Spacing suitable to each dimension's nested level
        if _arrayDim > 1 {
          writer.writeNewline();
          writer.writeLiteral(" " * (_arrayDim-1));
        }

        // Actually start the JSON list format
        writer._writeLiteral("[");
      }

      @chpldoc.nodoc
      proc endDim() throws {
        // For all but the 'last' dimension, we want the closing square bracket
        // to be on a newline to match the opening bracket. For example:
        /*
        [
         [
          [1, 2, 3]   <--- innermost dimension, keep on the same line
         ]   <--- otherwise, match nesting from opening bracket
        ]
        */
        if _arrayDim < _arrayMax {
          writer.writeNewline();
          writer._writeLiteral(" " * (_arrayDim-1));
        }

        // Actually close out the list
        writer._writeLiteral("]");

        // Reset the 'first' property so that a later 'pane' of this dimension
        // prints correctly. For example:
        /*
        [
         [
          [1, 2, 3]
         ],
         [
          [4, 5, 6]    <--- the 'first' thing despite previous '[1, 2, 3]'
         ]
        ]
        */
        if _arrayDim < _arrayFirst.size then
          _arrayFirst[_arrayDim] = true;

        // Reset state
        _arrayDim -= 1;
        _first = true;
      }

      @chpldoc.nodoc
      proc writeElement(const val: ?) throws {
        if !_first then writer._writeLiteral(", ");
        else _first = false;

        writer.write(val);
      }

      @chpldoc.nodoc
      proc endArray() throws {
      }
    }

    @chpldoc.nodoc
    proc startArray(writer: _writeType, size: uint) throws {
      return new ArraySerializer(writer);
    }

    record MapSerializer {
      var writer;
      var _first : bool = true;

      @chpldoc.nodoc
      proc writeKey(const key: ?) throws {
        if !_first {
          writer._writeLiteral(", ");
          writer.writeNewline();
          writer._writeLiteral("  ");
        } else {
          writer.writeNewline();
          writer._writeLiteral("  ");
          _first = false;
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

      @chpldoc.nodoc
      proc writeValue(const val: ?) throws {
        writer._writeLiteral(": ");
        writer.write(val);
      }

      @chpldoc.nodoc
      proc endMap() throws {
        writer.writeNewline();
        writer._writeLiteral("}");
      }
    }

    // Write maps like so:
    // {
    //   "east": "west",
    //   "hello": "goodbye",
    //   "north": "south",
    //   "day": "night"
    // }
    @chpldoc.nodoc
    proc startMap(writer: _writeType, size: uint) throws {
      writer._writeLiteral("{");
      return new MapSerializer(writer);
    }
  }

  private extern proc qio_channel_skip_json_field(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;

  // Read the JSON object in ahead of time, and produce a map of field names
  // to file offsets that we can look at later in deserializeField.
  // TODO: use a proper JSON library.
  private proc outOfOrderHelper(reader: _readerType) throws {
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

    // Keep track of information gained from reading ahead

    // State to track position in a multidimensional array

    @chpldoc.nodoc
    proc init() {
      this.complete();
    }

    // TODO: rewrite in terms of writef, or something
    @chpldoc.nodoc
    proc _oldRead(ch: _readerType, ref val:?t) throws {
      var _def = new DefaultDeserializer();
      var dc = ch.withDeserializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }
      st.realfmt = 2;
      st.bytes_prefix = 0;
      st.string_format = iostringformatInternal.json:uint(8);
      st.aggregate_style = QIO_AGGREGATE_FORMAT_JSON:uint(8);
      st.array_style = QIO_ARRAY_FORMAT_JSON:uint(8);
      st.tuple_style = QIO_TUPLE_FORMAT_JSON:uint(8);
      dc._set_styleInternal(st);
      dc._readOne(dc._kind, val, here);
    }

    proc deserializeType(reader:_readerType, type readType) : readType throws {
      if isNilableClassType(readType) && reader.matchLiteral("null") {
        return nil:readType;
      }

      // TODO:
      // - escaped strings
      if isNumericType(readType) || isBoolType(readType) {
        var x : readType;
        reader._readOne(reader._kind, x, here);
        return x;
      } else if isStringType(readType) || isBytesType(readType) {
        // TODO: Ideally something like:
        //   reader.readf("%{\"S\"}", tmp);
        // But this doesn't work well with escape strings...
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
        return readType.deserializeFrom(reader=reader, deserializer=this);
      } else {
        return new readType(reader=reader, deserializer=this);
      }
    }

    proc deserializeValue(reader: _readerType, ref val: ?readType) : void throws {
      if canResolveMethod(val, "deserialize", reader, this) {
        val.deserialize(reader=reader, deserializer=this);
      } else {
        val = deserializeType(reader, readType);
      }
    }

    record AggregateDeserializer {
      var reader;
      var _fieldOffsets : map(string, int);
      var _lastPos = -1;
      var _parent : bool = false;

      @chpldoc.nodoc
      proc _readFieldName(reader: _readerType, key: string) throws {
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

      @chpldoc.nodoc
      proc deserializeField(name: string, type T) throws {
        if _fieldOffsets.contains(name) {
          // Use 'advance' instead of 'seek' to support reading in a marked
          // channel, which can happen during 'readf'.
          //
          // Use 'mark' to rewind the position since 'advance' doesn't support
          // negative values. This means that 'deserializeField' does not advance
          // the channel's position until 'endClass' or 'endRecord' are called.
          reader.mark();
          const dist =  _fieldOffsets[name] - reader.offset();
          reader.advance(dist);
        } else if !name.isEmpty() {
          throw new IllegalArgumentError("field '" + name + "' not found.");
        }

        var ret = reader.read(T);

        // note: trailing commas not allowed in json
        reader.matchLiteral(",");

        // Rewind so that we can call 'advance' again
        if !name.isEmpty() then
          reader.revert();

        return ret;
      }

      proc startClass(reader, name: string) {
        return new AggregateDeserializer(reader, _fieldOffsets, _lastPos, _parent=true);
      }

      @chpldoc.nodoc
      proc endClass() throws {
        if !_parent {
          const dist =  _lastPos - reader.offset();
          reader.advance(dist);
          reader.readLiteral("}");
        }
      }

      @chpldoc.nodoc
      proc endRecord() throws {
        endClass();
      }
    }

    @chpldoc.nodoc
    proc startClass(reader: _readerType, name: string) throws {
      //
      // TODO: Should we only compute the mapping if the fields are being
      // read out of order?
      //
      var (m, last) = outOfOrderHelper(reader);
      reader.readLiteral("{");

      return new AggregateDeserializer(reader, m, last);
    }

    @chpldoc.nodoc
    proc startRecord(reader: _readerType, name: string) throws {
      return startClass(reader, name);
    }

    record ListDeserializer {
      var reader;
      var _first : bool = true;

      @chpldoc.nodoc
      proc readElement(type eltType) throws {
        if !_first then reader._readLiteral(",");
        else _first = false;

        // preemptively check if a list will end with the next byte
        //  to avoid an uncaught BadFormatError in eltType's
        //  deserializing initializer
        if !this.hasMore()
          then throw new BadFormatError();
          else return reader.read(eltType);
      }

      @chpldoc.nodoc
      proc endList() throws {
        reader._readLiteral("]");
      }

      @chpldoc.nodoc
      proc endTuple() throws {
        endList();
      }

      proc hasMore() : bool throws {
        reader.mark();
        if reader.matchLiteral("]") {
          reader.revert();
          return false;
        } else {
          reader.commit(); // doesn't do anything
          return true;
        }
      }
    }

    @chpldoc.nodoc
    proc startTuple(reader: _readerType) throws {
      return startList(reader);
    }

    @chpldoc.nodoc
    proc startList(reader: _readerType) throws {
      reader._readLiteral("[");
      return new ListDeserializer(reader);
    }

    record ArrayDeserializer {
      var reader;
      var _first : bool = true;
      var _arrayDim = 0;
      var _arrayMax = 0;
      var _arrayFirst : list(bool);

      // See comments in writing case for explanation
      @chpldoc.nodoc
      proc startDim() throws {
        _arrayDim += 1;
        if _arrayFirst.size < _arrayDim {
          _arrayFirst.pushBack(true);
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

      @chpldoc.nodoc
      proc readElement(type eltType) throws {
        if !_first then reader._readLiteral(", ");
        else _first = false;

        return reader.read(eltType);
      }

      @chpldoc.nodoc
      proc endDim() throws {
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
        _first = true;
      }

      @chpldoc.nodoc
      proc endArray() throws {
      }
    }

    @chpldoc.nodoc
    proc startArray(reader: _readerType) throws {
      return new ArrayDeserializer(reader);
    }

    record MapDeserializer {
      var reader;
      var _first : bool = true;

      @chpldoc.nodoc
      proc readKey(type keyType) throws {
        if !_first then reader._readLiteral(",");
        else _first = false;

        if keyType == string {
          return reader.read(string);
        } else {
          // Read in a JSON string, write it to a memory file, then read that
          // file as a JSON entity for the 'readType'.
          var f = openMemFile();
          var s = reader.read(string);
          {
            f.writer().withSerializer(DefaultSerializer).write(s);
          }
          return f.reader().withDeserializer(JsonDeserializer).read(keyType);
        }
      }

      @chpldoc.nodoc
      proc readValue(type valType) throws {
        reader._readLiteral(":");
        return reader.read(valType);
      }

      @chpldoc.nodoc
      proc endMap() throws {
        reader._readLiteral("}");
      }

      proc hasMore() : bool throws {
        reader.mark();
        defer reader.revert();
        return !reader.matchLiteral("}");
      }
    }

    @chpldoc.nodoc
    proc startMap(reader: _readerType) throws {
      reader._readLiteral("{");
      return new MapDeserializer(reader);
    }

  }
}
