
module BinaryIO {
  private use IO;
  private use CTypes;
  private use Map;
  private use List;
  private use Types;

  // TODO: should we implement these things in a single type for brevity?
  // TODO: Should endianness be param?
  // TODO: Right now this is a very homebrew format, but we should switch to
  //       something more standard...

  type _writeType = fileWriter(serializerType=BinarySerializer, ?);
  type _readerT = fileReader(deserializerType=BinaryDeserializer, ?);

  record BinarySerializer {
    const endian : IO.ioendian = IO.ioendian.native;
    var _size: uint;
    var _nesting = 0;

    proc _fork() {
      return new BinarySerializer(endian=endian);
    }

    // TODO: rewrite in terms of writef, or something
    // Uses old 'iostyle' for now so that I don't have to think about 'writef'
    proc _oldWrite(ch: _writeType, const val:?t) throws {
      var _def = new DefaultSerializer();
      var dc = ch.withSerializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }

      st.binary = 1;
      st.byteorder = 1 + endian:uint(8); 
      st.str_style = stringStyleWithVariableLength():int(64);

      dc._set_styleInternal(st);
      dc._writeOne(dc.kind, val, here);
    }

    proc serializeValue(writer: _writeType, const val:?t) throws {
      if isNumericType(t) {
        select endian {
          when ioendian.native do writer.writeBinary(val, ioendian.native);
          when ioendian.little do writer.writeBinary(val, ioendian.little);
          when ioendian.big do writer.writeBinary(val, ioendian.big);
        }
      } else if t == string  || isEnumType(t) || t == bytes ||
         isBoolType(t) {
        _oldWrite(writer, val);
      } else if t == nothing {
        // nothing...
      } else if isClassType(t) {
        if val == nil {
          writer.writeByte(0);
        } else {
          writer.writeByte(1);
          var alias = writer.withSerializer(_fork());
          val!.serialize(writer=alias, serializer=alias.serializer);
        }
      } else {
        var alias = writer.withSerializer(_fork());
        val.serialize(writer=alias, serializer=alias.serializer);
      }
    }

    proc serializeField(writer: _writeType, name: string, const val: ?T) throws {
      _size -= 1;
      writer.write(val);
    }

    proc startClass(writer: _writeType, name: string, size: int) throws {
      _startComposite(writer, size);
    }
    proc endClass(writer: _writeType) throws {
      _endComposite(writer);
    }

    proc startRecord(writer: _writeType, name: string, size: int) throws {
      _startComposite(writer, size);
    }
    proc endRecord(writer: _writeType) throws {
      _endComposite(writer);
    }

    proc startTuple(writer: _writeType, size: int) throws {
      _startComposite(writer, size);
    }
    proc endTuple(writer: _writeType) throws {
      _endComposite(writer);
    }

    proc _startComposite(writer: _writeType, size: int) throws {
      _size += size.safeCast(uint);
      _nesting += 1;
    }
    proc _endComposite(writer: _writeType) throws {
      _nesting -= 1;
    }

    // TODO: could add some verification here...
    proc startList(writer: _writeType, size: uint) throws {
      writer.write(size);
    }
    proc writeListElement(writer: _writeType, const val: ?) throws {
      writer.write(val);
    }
    proc endList(writer: _writeType) throws {
    }

    // TODO: add stuff for known sizes, size 'hints'
    // TODO: add support for 'hasNext' kind of thing
    //
    // LATER:
    // - TODO: support for unknown size
    //   - idea: serialize into internal buffer, count calls, then actually write things
    // - TODO: check size mismatches

    proc startArray(writer: _writeType, size: uint) throws {
    }

    proc startArrayDim(writer: _writeType, size: uint) throws {
    }
    proc endArrayDim(writer: _writeType) throws {
    }

    proc writeArrayElement(writer: _writeType, const val: ?) throws {
      // TODO: validate number of elements being written...
      writer.write(val);
    }

    proc writeBulkElements(writer: _writeType, data: c_ptr(?eltType), numElements: uint) throws
    where isNumericType(eltType) {
      const n = c_sizeof(eltType)*numElements;
      writer.writeBinary(data, n.safeCast(int));
    }

    proc endArray(writer: _writeType) throws {
    }

    proc startMap(writer: _writeType) throws {
      throw new Error("maps of unknown size are not yet supported by BinarySerializer");
    }

    proc startMap(writer: _writeType, size: uint) throws {
      writer.write(size);
      _size = size;
    }

    proc writeKey(writer: _writeType, const key: ?) throws {
      writer.write(key);
    }

    proc writeValue(writer: _writeType, const val: ?) throws {
      writer.write(val);
    }

    proc endMap(writer: _writeType) throws {
    }
  }

  record BinaryDeserializer {
    const endian : IO.ioendian = IO.ioendian.native;

    var _sizeKnown : bool = false;
    var _numElements : uint;
    var _size : uint;
    var _nesting = 0;

    proc init(endian: IO.ioendian = IO.ioendian.native) {
      this.endian = endian;
      this.complete();
    }

    proc _fork() {
      return new BinaryDeserializer(endian=endian);
    }

    // TODO: rewrite in terms of writef, or something
    proc _oldRead(ch: _readerT, ref val:?t) throws {
      var _def = new DefaultDeserializer();
      var dc = ch.withDeserializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }

      st.binary = 1;
      st.byteorder = 1 + endian:uint(8); 
      st.str_style = stringStyleWithVariableLength():int(64);

      dc._set_styleInternal(st);
      dc._readOne(dc.kind, val, here);
    }

    proc deserializeType(reader:fileReader, type readType) : readType throws {
      if isClassType(readType) {
        const notNil = reader.readByte();
        if notNil == 0 {
          if isNilableClassType(readType) then
            return nil:readType;
          else
            throw new Error("cannot read nil class into non-nilable class");
        } else if notNil != 1 {
          throw new BadFormatError("invalid binary format for class nilability");
        }
        // else: not nil, proceed to try initializing
      }

      if isNumericType(readType) {
        var x : readType;
        var ret : bool;
        select endian {
          when ioendian.native do ret = reader.readBinary(x, ioendian.native);
          when ioendian.little do ret = reader.readBinary(x, ioendian.little);
          when ioendian.big    do ret = reader.readBinary(x, ioendian.big);
        }
        if !ret then
          throw new EofError();
        return x;
      } else if isBoolType(readType) ||
                isEnumType(readType) || isStringType(readType) ||
                isBytesType(readType) {
        var x : readType;
        _oldRead(reader, x);
        return x;
      } else if readType == nothing {
        // nothing...
      } else if canResolveTypeMethod(readType, "deserializeFrom", reader, this) ||
                isArrayType(readType) {
        var alias = reader.withDeserializer(_fork());
        return readType.deserializeFrom(reader=alias, deserializer=alias.deserializer);
      } else {
        var alias = reader.withDeserializer(_fork());
        return new readType(reader=alias, deserializer=alias.deserializer);
      }
    }

    proc deserializeValue(reader: fileReader, ref val: ?readType) : void throws {
      if canResolveMethod(val, "deserialize", reader, this) {
        var alias = reader.withDeserializer(_fork());
        val.deserialize(reader=alias, deserializer=alias.deserializer);
      } else {
        val = deserializeType(reader, readType);
      }
    }

    proc deserializeField(reader: _readerT, name: string, type T) throws {
      _size -= 1;
      return reader.read(T);
    }

    proc startClass(reader: fileReader, name: string) throws {
      _startComposite(reader);
    }
    proc endClass(reader: fileReader) throws {
    }

    proc startRecord(reader: fileReader, name: string) throws {
      _startComposite(reader);
    }
    proc endRecord(reader: fileReader) throws {
    }

    proc startTuple(reader: fileReader) throws {
      _startComposite(reader);
    }
    proc endTuple(reader: fileReader) throws {
    }

    proc _startComposite(reader: fileReader) throws {
      _nesting += 1;
    }
    proc _endComposite(reader: fileReader) throws {
      _nesting -= 1;
    }

    proc startList(reader: fileReader) throws {
      _numElements = reader.read(uint);
      _sizeKnown = _numElements != 0;
    }
    proc readListElement(reader: fileReader, type eltType) throws {
      if _sizeKnown && _numElements <= 0 then
        throw new BadFormatError("no more list elements remain");

      if _sizeKnown then _numElements -= 1;

      return reader.read(eltType);
    }
    proc endList(reader: fileReader) throws {
      if _sizeKnown && _numElements != 0 then
        throw new Error("read too few elements for list");
    }

    proc startArray(reader: fileReader) throws {
    }

    proc startArrayDim(reader: fileReader) throws {
    }
    proc endArrayDim(reader: fileReader) throws {
    }

    proc readArrayElement(reader: fileReader, type eltType) throws {
      if _sizeKnown then _numElements -= 1;
      return reader.read(eltType);
    }

    proc readBulkElements(reader: fileReader, data: c_ptr(?eltType), numElements: uint) throws
    where isNumericType(eltType) {
      const n = c_sizeof(eltType)*numElements;
      const got = reader.readBinary(data, n.safeCast(int));
      if got < n then throw new EofError();
      else _numElements -= numElements;
    }

    proc endArray(reader: fileReader) throws {
    }

    proc startMap(reader: fileReader) throws {
      _sizeKnown = true;
      _numElements = reader.read(uint);
    }

    proc readKey(reader: fileReader, type keyType) throws {
      if _numElements <= 0 then
        throw new BadFormatError("no more map elements remain!");

      _numElements -= 1;

      return reader.read(keyType);
    }

    proc readValue(reader: fileReader, type valType) throws {
      return reader.read(valType);
    }

    proc endMap(reader: fileReader) throws {
      if _numElements != 0 then
        throw new Error("failed to read all expected elements in map!");
    }
  }
}
