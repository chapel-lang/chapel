/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

/* Provides serialization for Chapel data types in a binary format.

Intended for use with :ref:`IO support for serializers and deserializers <serialize-deserialize>`.
 */
@unstable("The ObjectSerialization module is unstable. The module's name, its types, and serialization format are subject to change.")
module ObjectSerialization {
  private use IO;
  private use CTypes;

  /*
    A binary Serializer that implements a simple binary format.

    This Serializer supports an ``endian`` field which may be configured at
    execution time.

    See :ref:`the serializers technote<ioSerializers>` for a general overview
    of Serializers and their usage.
  */
  record objectSerializer {
    /*
      'endian' represents the endianness of the binary output produced by this
      Serializer.
    */
    const endian : endianness = endianness.native;

    @chpldoc.nodoc
    const _structured = true;

    // TODO: rewrite to use correct IO methods (e.g. writeBinary)
    // For now, this is just a helper to mirror the old behavior for basic
    // types
    @chpldoc.nodoc
    proc _oldWrite(ch: fileWriter(?), const val:?t) throws {
      var _def = new defaultSerializer();
      var dc = ch.withSerializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }

      st.binary = 1;
      st.byteorder = 1 + endian:uint(8);
      if _structured then
        st.str_style = iostringstyleInternal.lenVb_data: int(64);
      else
        st.str_style = iostringstyleInternal.data_toeof: int(64);

      dc._set_styleInternal(st);
      dc._writeOne(_iokind.dynamic, val, here);
    }

    /*
      Serialize ``val`` with ``writer``.

      Numeric values like integers, real numbers, and complex numbers are
      serialized directly to the associated ``fileWriter`` as binary data in the
      specified endianness.

      Booleans are serialized as single byte unsigned values of either ``0`` or
      ``1``.

      ``string`` values are serialized beginning with a length represented by a
      variable-length byte scheme (which is always the same no matter what
      endianness). In this scheme, the high bit of each encoded length byte records
      whether or not there are more length bytes (and the remaining bits encode the
      length in a big-endian manner). Then, the raw binary data of the string is
      written to the ``writer``.

      The ``nil`` value is serialized as a single unsigned byte of value ``0``.

      Classes are serialized beginning with a single unsigned byte of either ``0``
      or ``1`` indicating ``nil``. Nilable classes that are ``nil`` will always
      serialize as ``0``, and non-nilable classes will always begin serializing
      as ``1``.

      Classes and records will have their ``serialize`` method invoked, passing
      in ``writer`` and this Serializer as arguments. Please see the
      :ref:`serializers technote<ioSerializers>` for more on the ``serialize``
      method.

      Classes and records are expected to implement the ``writeSerializable``
      interface. The ``serializable`` interface is also acceptable.

      .. note::

        Serializing and deserializing enums is not stable in this format.

      :arg writer: The ``fileWriter`` used to write serialized output.
      :arg val: The value to be serialized.
    */
    proc ref serializeValue(writer: fileWriter(serializerType=objectSerializer, locking=false, ?),
                        const val:?t) throws {
      if isNumericType(t) {
        select endian {
          when endianness.native do writer.writeBinary(val, endianness.native);
          when endianness.little do writer.writeBinary(val, endianness.little);
          when endianness.big do writer.writeBinary(val, endianness.big);
        }
      } else if t == string  || isEnumType(t) || t == bytes ||
                isBoolType(t) {
        _oldWrite(writer, val);
      } else if t == nothing {

      } else if isClassType(t) {
        // Classes always prefixed with a bit indicating nil-ness
        //
        // TODO: Should 'startClass' handle this case?
        if val == nil {
          if _structured then
            writer.writeByte(0);
        } else {
          if _structured then
            writer.writeByte(1);
          val!.serialize(writer=writer, serializer=this);
        }
      } else {
        val.serialize(writer=writer, serializer=this);
      }
    }

    /*
      Start serializing a class and return a new ``AggregateSerializer``.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg name: The name of the class type.
      :arg size: The number of fields in the class.

      :returns: A new :type:`AggregateSerializer`
    */
    proc startClass(writer: fileWriter(?), name: string, size: int) throws {
      return new AggregateSerializer(writer);
    }

    /*
      Start serializing a record and return a new ``AggregateSerializer``.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg name: The name of the record type.
      :arg size: The number of fields in the class.

      :returns: A new :type:`AggregateSerializer`
    */
    proc startRecord(writer: fileWriter(?), name: string, size: int) throws {
      return new AggregateSerializer(writer);
    }

    /*
      Returned by ``startClass`` or ``startRecord`` to provide the API for
      serializing classes or records.

      In this simple binary format, classes and records do not begin or end with
      any bytes indicating size, and instead serialize their field values in
      ``objectSerializer``'s format.

      For example, a record with two ``uint(8)`` fields with values ``1`` and
      ``2`` would be serialized as ``0x01`` followed by ``0x02`` (in raw binary).
    */
    record AggregateSerializer {
      @chpldoc.nodoc
      var writer : fileWriter(false, objectSerializer);

      /*
        Serialize ``field`` in ``objectSerializer``'s format.
      */
      proc writeField(name: string, const field: ?T) throws {
        writer.write(field);
      }

      /*
        Start serializing a nested class inside the current class. In this
        binary format, this has no impact on the serialized output.
      */
      proc startClass(writer, name: string, size: int) throws {
        return this;
      }

      /*
        End deserialization of this class.
      */
      proc endClass() throws {
      }

      /*
        End deserialization of this record.
      */
      proc endRecord() throws {
      }
    }

    /*
      Start serializing a tuple and return a new ``TupleSerializer``.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg size: The number of elements in the tuple.

      :returns: A new TupleSerializer
    */
    proc startTuple(writer: fileWriter(?), size: int) throws {
      return new TupleSerializer(writer);
    }

    /*
      Returned by ``startTuple`` to provide the API for serializing tuples.

      In this simple binary format, tuples do not begin or end with any bytes
      indicating size, and instead serialize their elements sequentially in
      ``objectSerializer``'s format.
    */
    record TupleSerializer {
      @chpldoc.nodoc
      var writer : fileWriter(false, objectSerializer);

      /*
        Serialize ``element`` in ``objectSerializer``'s format.
      */
      proc writeElement(const element: ?T) throws {
        writer.write(element);
      }

      /*
        Ends serialization of the current tuple.
      */
      proc endTuple() throws {
      }
    }

    /*
      Start serializing a list by serializing ``size``.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg size: The number of elements in the list.

      :returns: A new ListSerializer
    */
    proc startList(writer: fileWriter(?), size: int) throws {
      writer.write(size);
      return new ListSerializer(writer);
    }

    /*
      Returned by ``startList`` to provide the API for serializing lists.

      In this simple binary format, lists begin with the serialization of an
      ``int`` representing the size of the list. This data is then followed by
      the binary serialization of the specified number of elements.
    */
    record ListSerializer {
      @chpldoc.nodoc
      var writer : fileWriter(false, objectSerializer);

      /*
        Serialize ``element`` in ``objectSerializer``'s format.
      */
      proc writeElement(const element: ?) throws {
        writer.write(element);
      }

      /*
        Ends serialization of  the current list.
      */
      proc endList() throws {
      }
    }

    /*
      Start serializing an array and return a new ``ArraySerializer``.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg size: The number of elements in the array.

      :returns: A new ArraySerializer
    */
    proc startArray(writer: fileWriter(?), size: int) throws {
      return new ArraySerializer(writer, endian);
    }

    /*
      Returned by ``startArray`` to provide the API for serializing arrays.

      In this simple binary format, arrays are serialized element by element
      in the order indicated by the caller of ``writeElement``. Dimensions and
      the start or end of the array are not represented.
    */
    record ArraySerializer {
      @chpldoc.nodoc
      var writer : fileWriter(false, objectSerializer);
      @chpldoc.nodoc
      const endian : endianness;

      /*
        Start serializing a new dimension of the array.
      */
      proc startDim(size: int) throws {
      }

      /*
        Ends serialization of this dimension.
      */
      proc endDim() throws {
      }

      /*
        Serialize ``element`` in ``objectSerializer``'s format.
      */
      proc writeElement(const element: ?) throws {
        writer.write(element);
      }

      /*
        Serialize ``numElements`` number of elements in ``data``, provided that
        the element type of ``data`` is a numeric type.

        This performance-motivated implementation of the optional
        ``writeBulkElements`` will write the elements of ``data`` in the order
        in which they are represented in memory.

        .. note::

          This method is only optimized for the case where the
          ``objectSerializer`` has been configured for ``native`` endianness.
      */
      proc writeBulkElements(data: c_ptr(?eltType), numElements: int) throws
      where isNumericType(eltType) {
        if endian == endianness.native {
          const n = c_sizeof(eltType)*numElements;
          writer.writeBinary(data, n.safeCast(int));
        } else {
          for i in 0..<numElements do writer.write(data[i]);
        }
      }

      /*
        Ends serialization of the current array.
      */
      proc endArray() throws {
      }
    }

    /*
      Start serializing a map by serializing ``size``.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg size: The number of entries in the map.

      :returns: A new MapSerializer
    */
    proc startMap(writer: fileWriter(?), size: int) throws {
      writer.write(size);
      return new MapSerializer(writer);
    }

    /*
      Returned by ``startMap`` to provide the API for serializing maps.

      In this simple binary format, maps begin with the serialization of an
      ``int`` representing the size of the map. This data is then followed by the
      binary serialization of the specified number of key-value pairs. The binary
      serialization of a key-value pair has no structure, and simply consists of
      the serialization of the key followed by the serialization of the value.
    */
    record MapSerializer {
      @chpldoc.nodoc
      var writer : fileWriter(false, objectSerializer);

      /*
        Serialize ``key`` in ``objectSerializer``'s format.
      */
      proc writeKey(const key: ?) throws {
        writer.write(key);
      }

      /*
        Serialize ``val`` in ``objectSerializer``'s format.
      */
      proc writeValue(const val: ?) throws {
        writer.write(val);
      }

      /*
        Ends serialization of the current map.
      */
      proc endMap() throws {
      }
    }
  }

  /*
    A binary Deserializer that implements a simple binary format.

    This Deserializer supports an ``endian`` field which may be configured at
    execution time.

    See :ref:`the serializers technote<ioSerializers>` for a general overview
    of Deserializers and their usage.

    Otherwise, please refer to :type:`objectSerializer` for a description of the
    binary format. Individual methods on this type may clarify relevant behavior
    specific to deserialization
  */
  record objectDeserializer {
    /*
      'endian' represents the endianness that this Deserializer should use when
      deserializing input.
    */
    const endian : IO.endianness = IO.endianness.native;

    @chpldoc.nodoc
    var _structured = true;

    @chpldoc.nodoc
    proc init(endian: IO.endianness = IO.endianness.native, _structured : bool = true) {
      this.endian = endian;
      this._structured = _structured;
      init this;
    }

    // TODO: rewrite in terms of writef, or something
    @chpldoc.nodoc
    proc _oldRead(ch: fileReader(?), ref val:?t) throws {
      var _def = new defaultDeserializer();
      var dc = ch.withDeserializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }

      st.binary = 1;
      st.byteorder = 1 + endian:uint(8);
      if _structured then
        st.str_style = iostringstyleInternal.lenVb_data: int(64);
      else
        st.str_style = iostringstyleInternal.data_toeof: int(64);

      dc._set_styleInternal(st);
      dc._readOne(_iokind.dynamic, val, here);
    }

    @chpldoc.nodoc
    proc _checkClassNil(reader:fileReader(?), type readType) : bool throws {
      const notNil = if _structured then reader.readByte() else 1;
      if notNil == 0 {
        if isNilableClassType(readType) then
          return true;
        else
          throw new BadFormatError("cannot read nil class into non-nilable class");
      } else if notNil != 1 {
        throw new BadFormatError("invalid binary format found for class nilability");
      }

      return false;
    }

    /*
      Deserialize type ``readType`` with ``reader``.

      Classes and records will be deserialized using an appropriate initializer,
      passing in ``reader`` and this Deserializer as arguments. If an
      initializer is unavailable, this method may invoke the class or record's
      ``deserialize`` method. Please see the :ref:`serializers technote<ioSerializers>` for more.

      Classes and records are expected to implement either the
      ``initDeserializable`` or ``readDeserializable`` interfaces (or both). The
      ``serializable`` interface is also acceptable.

      :arg reader: The ``fileReader`` from which types are deserialized.
      :arg readType: The type to be deserialized.

      :returns: A value of type ``readType``.
    */
    proc ref deserializeType(reader:fileReader(?), type readType) : readType throws {
      if isClassType(readType) {
        var isNil = _checkClassNil(reader, readType);
        if isNilableClassType(readType) && isNil then
          return nil:readType;
      }
      // else: not nil, proceed to try initializing

      if isNumericType(readType) {
        var x : readType;
        var ret : bool;
        select endian {
          when endianness.native do ret = reader.readBinary(x, endianness.native);
          when endianness.little do ret = reader.readBinary(x, endianness.little);
          when endianness.big    do ret = reader.readBinary(x, endianness.big);
        }
        if !ret then
          throw new EofError();
        else
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
        return readType.deserializeFrom(reader=reader, deserializer=this);
      } else {
        return new readType(reader=reader, deserializer=this);
      }
    }

    /*
      Deserialize from ``reader`` directly into ``val``.

      Like :proc:`deserializeType`, but reads into an initialized value rather
      than creating a new value. For classes and records, this method will first
      attempt to invoke a ``deserialize`` method. If the ``deserialize`` method
      is unavailable, this method may fall back on invoking a suitable
      initializer and assigning the resulting value into ``val``.. Please see the
      :ref:`serializers technote<ioSerializers>` for more.

      Classes and records are expected to implement either the
      ``readDeserializable`` or ``initDeserializable`` interfaces (or both). The
      ``serializable`` interface is also acceptable.

      :arg reader: The ``fileReader`` from which values are deserialized.
      :arg val: The value into which this Deserializer will deserialize.
    */
    proc ref deserializeValue(reader: fileReader(?), ref val: ?readType) : void throws {
      if canResolveMethod(val, "deserialize", reader, this) {
        // Skip owned/shared since we'll try again for 'borrowed'
        if isClassType(readType) && !isOwnedClassType(readType) &&
           !isSharedClassType(readType) {
          var isNil = _checkClassNil(reader, readType);
          if isNilableClassType(readType) && isNil then
            val = nil;
        }
        val.deserialize(reader=reader, deserializer=this);
      } else {
        val = deserializeType(reader, readType);
      }
    }

    /*
      Start deserializing a class by returning an ``AggregateDeserializer``.

      :arg reader: The ``fileReader`` to use when deserializing.
      :arg name: The name of the class type.

      :returns: A new :type:`AggregateDeserializer`
    */
    proc startClass(reader: fileReader(?), name: string) throws {
      return new AggregateDeserializer(reader);
    }

    /*
      Start deserializing a record by returning an ``AggregateDeserializer``.

      :arg reader: The ``fileReader`` to use when deserializing.
      :arg name: The name of the record type.

      :returns: A new :type:`AggregateDeserializer`
    */
    proc startRecord(reader: fileReader(?), name: string) throws {
      return new AggregateDeserializer(reader);
    }

    /*
      Returned by ``startClass`` or ``startRecord`` to provide the API for
      deserializing classes or records.

      See ``objectSerializer.AggregateSerializer`` for details of the
      binary format for classes and records.
    */
    record AggregateDeserializer {
      @chpldoc.nodoc
      var reader : fileReader(false, objectDeserializer);

      /*
        Deserialize and return a value of type ``fieldType``.
      */
      proc readField(name: string, type fieldType) : fieldType throws {
        return reader.read(fieldType);
      }

      /*
        Deserialize ``field`` in-place.
      */
      proc readField(name: string, ref field) throws {
        reader.read(field);
      }

      /*
        Start deserializing a nested class inside the current class.

        See ``objectSerializer.AggregateSerializer.startClass`` for details
        on inheritance on the binary format.

        :returns: A new AggregateDeserializer
      */
      proc startClass(reader, name: string) throws {
        return this;
      }

      /*
        End deserialization of the current class.
      */
      proc endClass() throws {
      }

      /*
        End deserialization of the current record.
      */
      proc endRecord() throws {
      }
    }

    /*
      Start deserializing a tuple by returning a ``TupleDeserializer``.

      :arg reader: The ``fileReader`` to use when deserializing.

      :returns: A new :type:`TupleDeserializer`
    */
    proc startTuple(reader: fileReader(?)) throws {
      return new TupleDeserializer(reader);
    }

    /*
      Returned by ``startTuple`` to provide the API for deserializing tuples.

      See ``objectSerializer.TupleSerializer`` for details of the binary format
      for tuples.
    */
    record TupleDeserializer {
      @chpldoc.nodoc
      var reader : fileReader(false, objectDeserializer);

      /*
        Deserialize an element of the tuple.

        :returns: A deserialized value of type ``eltType``.
      */
      proc readElement(type eltType) : eltType throws {
        return reader.read(eltType);
      }

      /*
        Deserialize ``element`` in-place as an element of the tuple.
      */
      proc readElement(ref element) throws {
        reader.read(element);
      }

      /*
        End deserialization of the current tuple.
      */
      proc endTuple() throws {
      }
    }

    /*
      Start deserializing a list by returning a ``ListDeserializer``.

      :arg reader: The ``fileReader`` to use when deserializing.

      :returns: A new :type:`ListDeserializer`
    */
    proc startList(reader: fileReader(?)) throws {
      return new ListDeserializer(reader, reader.read(uint));
    }

    /*
      Returned by ``startList`` to provide the API for deserializing lists.

      See ``objectSerializer.ListSerializer`` for details of the binary format
      for lists.
    */
    record ListDeserializer {
      @chpldoc.nodoc
      var reader : fileReader(false, objectDeserializer);
      @chpldoc.nodoc
      var _numElements : uint;

      /*
        Deserialize an element of the list.

        :returns: A deserialized value of type ``eltType``.
      */
      proc ref readElement(type eltType) : eltType throws {
        if _numElements <= 0 then
          throw new BadFormatError("no more list elements remain");

        _numElements -= 1;

        return reader.read(eltType);
      }

      /*
        Deserialize ``element`` in-place as an element of the list.
      */
      proc ref readElement(ref element) throws {
        if _numElements <= 0 then
          throw new BadFormatError("no more list elements remain");

        _numElements -= 1;

        reader.read(element);
      }

      /*
        End deserialization of the current list.

        :throws: A ``BadFormatError`` if there are remaining elements.
      */
      proc endList() throws {
        if _numElements != 0 then
          throw new BadFormatError("read too few elements for list");
      }

      /*
        :returns: Returns ``true`` if there are more elements to read.
      */
      proc hasMore() : bool throws {
        return _numElements > 0;
      }
    }

    /*
      Start deserializing an array by returning an ``ArrayDeserializer``.

      :arg reader: The ``fileReader`` to use when deserializing.

      :returns: A new :type:`ArrayDeserializer`
    */
    proc startArray(reader: fileReader(?)) throws {
      return new ArrayDeserializer(reader, endian);
    }

    /*
      Returned by ``startArray`` to provide the API for deserializing arrays.

      See ``objectSerializer.ArraySerializer`` for details of the binary format
      for arrays.
    */
    record ArrayDeserializer {
      @chpldoc.nodoc
      var reader : fileReader(false, objectDeserializer);
      @chpldoc.nodoc
      const endian : endianness;

      /*
        Inform the ``ArrayDeserializer`` to start deserializing a new dimension.
      */
      proc startDim() throws {
      }

      /*
        End deserialization of the current dimension.
      */
      proc endDim() throws {
      }

      /*
        Deserialize an element of the list.

        :returns: A deserialized value of type ``eltType``.
      */
      proc readElement(type eltType) : eltType throws {
        return reader.read(eltType);
      }

      /*
        Deserialize ``element`` in-place as an element of the array.
      */
      proc readElement(ref element) throws {
        reader.read(element);
      }

      /*
        Deserialize ``numElements`` number of elements into ``data``, provided
        that the element type of ``data`` is a numeric type.

        This performance-motivated implementation of the optional
        ``readBulkElements`` will read the elements of ``data`` in the order in
        which they are represented in memory.

        .. note::

          This method is only optimized for the case where the
          ``objectDeserializer`` has been configured for ``native`` endianness.
      */
      proc readBulkElements(data: c_ptr(?eltType), numElements: int) throws
      where isNumericType(eltType) {
        if endian == endianness.native {
          const n = c_sizeof(eltType)*numElements;
          const got = reader.readBinary(data, n.safeCast(int));
          if got < n then throw new EofError();
        } else {
          for i in 0..<numElements do
            reader.read(data[i]);
        }
      }

      /*
        End deserialization of the current array.
      */
      proc endArray() throws {
      }
    }


    /*
      Start deserializing a map by returning a ``MapDeserializer``.

      :arg reader: The ``fileReader`` to use when deserializing.

      :returns: A new :type:`MapDeserializer`
    */
    proc startMap(reader: fileReader(?)) throws {
      return new MapDeserializer(reader, reader.read(uint));
    }

    /*
      Returned by ``startMap`` to provide the API for deserializing maps.

      See ``objectSerializer.MapSerializer`` for details of the binary
      format for map.
    */
    record MapDeserializer {
      @chpldoc.nodoc
      var reader;
      @chpldoc.nodoc
      var _numElements : uint;

      /*
        Deserialize and return a key of type ``keyType``.
      */
      proc ref readKey(type keyType) : keyType throws {
        if _numElements <= 0 then
          throw new BadFormatError("no more map elements remain!");

        _numElements -= 1;

        return reader.read(keyType);
      }

      /*
        Deserialize ``key`` in-place as a key of the map.
      */
      proc ref readKey(ref key) throws {
        if _numElements <= 0 then
          throw new BadFormatError("no more map elements remain!");

        _numElements -= 1;

        reader.read(key);
      }

      /*
        Deserialize and return a value of type ``valType``.
      */
      proc readValue(type valType) : valType throws {
        return reader.read(valType);
      }

      /*
        Deserialize ``value`` in-place as a value of the map.
      */
      proc readValue(ref value) throws {
        reader.read(value);
      }

      /*
        End deserialization of the current map.

        :throws: A ``BadFormatError`` if there are entries remaining.
      */
      proc endMap() throws {
        if _numElements != 0 then
          throw new BadFormatError("failed to read all expected elements in map");
      }

      /*
        :returns: Returns ``true`` if there are more elements to read.

        .. warning::

          Behavior of 'hasMore' is undefined when called between ``readKey`` and
          ``readValue``.
      */
      proc hasMore() : bool throws {
        return _numElements > 0;
      }
    }
  }
}
