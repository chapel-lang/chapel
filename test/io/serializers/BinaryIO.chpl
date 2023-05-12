
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
    const verify = false;
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

    proc serializeValue(writer: _writeType, const x:?t) throws {
      if isNumericType(t) {
        select endian {
          when ioendian.native do writer.writeBinary(x, ioendian.native);
          when ioendian.little do writer.writeBinary(x, ioendian.little);
          when ioendian.big do writer.writeBinary(x, ioendian.big);
        }
      } else if t == string  || isEnumType(t) || t == bytes ||
         isBoolType(t) {
        _oldWrite(writer, x);
      } else if t == nothing {
        // nothing...
      } else if isClassType(t) {
        if x == nil {
          writer.writeByte(0);
        } else {
          writer.writeByte(1);
          var alias = writer.withSerializer(_fork());
          x!.serialize(writer=alias, serializer=alias.serializer);
        }
      } else {
        var alias = writer.withSerializer(_fork());
        x.serialize(writer=alias, serializer=alias.serializer);
      }
    }

    proc serializeField(writer: _writeType, key: string, const val: ?T) throws {
      _size -= 1;
      if verify && _size < 0 then throw new Error("Attempted to write more fields than specified");
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
      if verify && _nesting == 0 then
        writer.write(size);
      _size += size.safeCast(uint);
      _nesting += 1;
    }
    proc _endComposite(writer: _writeType) throws {
      _nesting -= 1;
      if verify && _nesting == 0 && _size != 0 then
        throw new Error("Wrote fewer fields than specified");
    }

    // TODO: could add some verification here...
    proc startList(w: _writeType, size: uint) throws {
      w.write(size);
    }
    proc writeListElement(w: _writeType, const val: ?) throws {
      w.write(val);
    }
    proc endList(w: _writeType) throws {
    }

    // TODO: add stuff for known sizes, size 'hints'
    // TODO: add support for 'hasNext' kind of thing
    //
    // LATER:
    // - TODO: support for unknown size
    //   - idea: serialize into internal buffer, count calls, then actually write things
    // - TODO: check size mismatches

    proc startArray(w: _writeType, numElements: uint) throws {
      if verify {
        w.write(numElements);
        _size = numElements;
      }
    }

    proc startArrayDim(w: _writeType, len: uint) throws {
    }
    proc endArrayDim(w: _writeType) throws {
    }

    proc writeArrayElement(w: _writeType, const val: ?) throws {
      // TODO: validate number of elements being written...
      w.write(val);
    }

    proc writeBulkElements(w: _writeType, data: c_ptr(?eltType), numElements: uint) throws
    where isNumericType(eltType) {
      if verify && numElements > _size then throw new IllegalArgumentError("len", "Cannot write more elements than specified in 'startArray'");
      const n = c_sizeof(eltType)*numElements;
      w.writeBinary(data, n.safeCast(int));
    }

    proc endArray(w: _writeType) throws {
    }

    proc startMap(w: _writeType) throws {
      throw new Error("maps of unknown size are not yet supported by BinarySerializer");
    }

    proc startMap(w: _writeType, size: uint) throws {
      w.write(size);
      _size = size;
    }

    proc writeKey(w: _writeType, const key: ?) throws {
      w.write(key);
    }

    proc writeValue(w: _writeType, const val: ?) throws {
      w.write(val);
    }

    proc endMap(w: _writeType) throws {
    }
  }

  record BinaryDeserializer {
    const endian : IO.ioendian = IO.ioendian.native;
    const verify = false;

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

    proc deserializeField(r: _readerT, key: string, type T) throws {
      _size -= 1;
      return r.read(T);
    }

    proc startClass(r: fileReader, name: string, size: int) throws {
      _startComposite(r, size);
    }
    proc endClass(r: fileReader) throws {
    }

    proc startRecord(r: fileReader, name: string, size: int) throws {
      _startComposite(r, size);
    }
    proc endRecord(r: fileReader) throws {
    }

    proc startTuple(r: fileReader, size: int) throws {
      _startComposite(r, size);
    }
    proc endTuple(r: fileReader) throws {
    }

    proc _startComposite(r: fileReader, size: int) throws {
      if verify && _nesting == 0 {
        const n = r.read(int);
        if n != size then
          throw new Error("Mismatch in number of fields expected");
      }
      _size += size.safeCast(uint);
      _nesting += 1;
    }
    proc _endComposite(r: fileReader) throws {
      _nesting -= 1;
      if verify && _nesting == 0 && _size != 0 then
        throw new Error("Wrote fewer fields than specified");
    }

    proc startList(r: fileReader) throws {
      _numElements = r.read(uint);
      _sizeKnown = _numElements != 0;
    }
    proc readListElement(r: fileReader, type eltType) throws {
      if _sizeKnown && _numElements <= 0 then
        throw new BadFormatError("no more list elements remain");

      if _sizeKnown then _numElements -= 1;

      return r.read(eltType);
    }
    proc endList(r: fileReader) throws {
      if _sizeKnown && _numElements != 0 then
        throw new Error("read too few elements for list");
    }

    proc startArray(r: fileReader) throws {
      if verify {
        _numElements = r.read(uint);
        _sizeKnown = _numElements != 0;
      }
    }

    proc startArrayDim(r: fileReader) throws {
    }
    proc endArrayDim(r: fileReader) throws {
    }

    proc readArrayElement(r: fileReader, type eltType) throws {
      if verify && _sizeKnown && _numElements <= 0 then
        throw new BadFormatError("no more array elements remain!");

      if _sizeKnown then _numElements -= 1;
      return r.read(eltType);
    }

    proc readBulkElements(r: fileReader, data: c_ptr(?eltType), numElements: uint) throws
    where isNumericType(eltType) {
      const n = c_sizeof(eltType)*numElements;
      const got = r.readBinary(data, n.safeCast(int));
      if got < n then throw new EofError();
      else _numElements -= numElements;
    }

    proc endArray(r: fileReader) throws {
      if verify && _sizeKnown && _numElements != 0 then
        throw new Error("failed to read all expected elements in array!");
    }

    proc startMap(r: fileReader) throws {
      _sizeKnown = true;
      _numElements = r.read(uint);
    }

    proc readKey(r: fileReader, type keyType) throws {
      if _numElements <= 0 then
        throw new BadFormatError("no more map elements remain!");

      _numElements -= 1;

      return r.read(keyType);
    }

    proc readValue(r: fileReader, type valType) throws {
      return r.read(valType);
    }

    proc endMap(r: fileReader) throws {
      if _numElements != 0 then
        throw new Error("failed to read all expected elements in map!");
    }
  }
}
