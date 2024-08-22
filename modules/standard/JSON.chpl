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

/* Provides serialization in the JSON format.

Intended for use with :ref:`IO support for serializers and deserializers <serialize-deserialize>`.
 */
module JSON {
  private use IO;
  private use CTypes;
  private use Map;
  private use List;

  /* Type Alias for an :record:`IO.fileWriter` that uses a :record:`jsonSerializer` */
  type jsonWriter = fileWriter(serializerType=jsonSerializer, ?);
  /* Type Alias for an :record:`IO.fileReader` that uses a :record:`jsonDeserializer` */
  type jsonReader = fileReader(deserializerType=jsonDeserializer, ?);

  /*
    A JSON format serializer to be used by :record:`~IO.fileWriter`.

    Implements the 'serializeValue' method which is called by a ``fileWriter``
    to produce a serialized representation of a value in JSON format.

    .. warning::

      This serializer is designed to generate valid JSON; however, no
      guarantees are made about the exact formatting w.r.t. whitespace,
      newlines or indentation.

    See the :ref:`IO Serializers<ioSerializers>` technote for more information
    about serializers in general.
  */
  record jsonSerializer {
    // TODO: rewrite in terms of writef, or something
    @chpldoc.nodoc
    proc _oldWrite(ch: jsonWriter, const val:?t) throws {
      var _def = new defaultSerializer();
      var dc = ch.withSerializer(_def);
      var st = dc._styleInternal();
      var orig = st; defer { dc._set_styleInternal(orig); }
      st.realfmt = 2;
      st.string_format = iostringformatInternal.json:uint(8);
      st.aggregate_style = QIO_AGGREGATE_FORMAT_JSON:uint(8);
      st.array_style = QIO_ARRAY_FORMAT_JSON:uint(8);
      st.tuple_style = QIO_TUPLE_FORMAT_JSON:uint(8);
      dc._set_styleInternal(st);
      dc._writeOne(_iokind.dynamic, val, here);
    }

    /* Serialize a value into a :record:`~IO.fileWriter` in JSON format */
    /*
      Serialize ``val`` with ``writer``.

      Numeric values are serialized as though they were written with the format
      of ``%i`` for integers and ``%er`` for ``real`` numbers. Please refer to
      :ref:`the section on Formatted IO<about-io-formatted-io>` for more
      information.

      Booleans are serialized as the literal strings ``true`` or ``false``.

      ``string`` values are serialized by wrapping the string in quotes, and
      escaping quotes inside the string. ``bytes`` values are also wrapped
      in quotes.

      Enums are serialized using the name of the corresponding value. For example
      with an enum like ``enum colors {red, green blue}``, the value ``red``
      would be serialized as a string: ``"red"``.

      The ``nil`` value and nilable class variables storing ``nil`` will be
      serialized as the text ``null``.

      Classes and records will have their ``serialize`` method invoked, passing
      in ``writer`` and this Serializer as arguments. Please see the
      :ref:`serializers technote<ioSerializers>` for more.

      Classes and records are expected to implement the ``writeSerializable``
      or ``serializable`` interface.

      :arg writer: The ``fileWriter`` used to write serialized output
      :arg val: The value to be serialized
    */
    proc ref serializeValue(writer: jsonWriter, const val:?t) throws {
      if t == string  || isEnumType(t) || t == bytes {
        // for quotes around things
        _oldWrite(writer, val);
      } else if isNumericType(t) || isBoolType(t) {
        _oldWrite(writer, val);
      } else if isClassType(t) {
        if val == nil {
          writer.writeLiteral("null");
        } else {
          val!.serialize(writer=writer, serializer=this);
        }
      } else {
        val.serialize(writer=writer, serializer=this);
      }
    }

    /*
      Start serializing a class by writing the character ``{``.

      :arg writer: The ``fileWriter`` to be used when serializing
      :arg name: The name of the class type
      :arg size: The number of fields in the class

      :returns: A new :type:`AggregateSerializer`
    */
    proc startClass(writer: jsonWriter, name: string, size: int) throws {
      writer.writeLiteral("{");
      return new AggregateSerializer(writer, _ending="}");
    }

    /*
      Start serializing a record by writing the character ``{``.

      :arg writer: The ``fileWriter`` to be used when serializing
      :arg name: The name of the class type
      :arg size: The number of fields in the class

      :returns: A new :type:`AggregateSerializer`
    */
    proc startRecord(writer: jsonWriter, name: string, size: int) throws {
      writer.writeLiteral("{");
      return new AggregateSerializer(writer, _ending="}");
    }

    /*
      Returned by ``startClass`` or ``startRecord`` to provide the API for
      serializing classes or records.

      Classes and records are serialized as JSON objects. For example, a
      ``class`` or ``record`` with integer fields ``x`` and ``y`` with values
      ``0`` and ``5`` would be serialized as ``{"x":0, "y":5}``.

    */
    record AggregateSerializer {
      @chpldoc.nodoc
      var writer;
      @chpldoc.nodoc
      var _parent : bool = false;
      @chpldoc.nodoc
      var _first : bool = true;
      @chpldoc.nodoc
      const _ending : string;
      @chpldoc.nodoc
      var _firstPtr : c_ptr(bool) = nil;

      /*
        Serialize ``field`` named ``name``.

        Serializes fields in the form ``"<name>":<field>``. Adds a comma before
        the name if this is not the first field.
      */
      proc ref writeField(name: string, const field: ?) throws {
        if !_first then writer.writeLiteral(", ");
        else _first = false;

        writer.write(name);
        writer.writeLiteral(":");
        writer.write(field);
      }

      /*
        Start serializing a nested class inside the current class. In this format
        inheritance is not represented and parent fields are printed before child
        fields. For example, the following classes with values ``x=5`` and
        ``y=2``:

        .. code-block:: chapel

          class Parent {
            var x : int;
          }

          class Child: Parent {
            var y : int;
          }

        would be serialized as:

        .. code-block:: text

          {"x":5, "y":2}

        :arg writer: The ``fileWriter`` to be used when serializing. Must match
          the writer used to create current AggregateSerializer
        :arg name: The name of the class type
        :arg size: The number of fields in the class

        :returns: A new AggregateSerializer
      */
      proc ref startClass(writer, name: string, size: int) throws {
        return new AggregateSerializer(writer, _parent=true,
                                       _firstPtr=c_addrOf(_first));
      }

      /*
        Ends serialization of the current class by writing the character ``}``.
      */
      proc endClass() throws {
        if !_parent then
          writer.writeLiteral(_ending);
        else if _firstPtr != nil then
          _firstPtr.deref() = _first;
      }

      /*
        Ends serialization of the current record by writing the character
        ``}``.
      */
      proc endRecord() throws {
        writer.writeLiteral(_ending);
      }
    }

    /*
      Start serializing a tuple by writing the character ``[``.

      In this format tuples are serialized as JSON lists. For example, the
      tuple ``(1, 2, 3)`` would be serialized as ``[1, 2, 3]``.

      :arg writer: The ``fileWriter`` to be used when serializing
      :arg size: The number of elements in the tuple

      :returns: A new :type:`ListSerializer`
    */
    proc startTuple(writer: jsonWriter, size: int) throws {
      writer.writeLiteral("[");
      return new ListSerializer(writer);
    }

    /*
      Start serializing a list by writing the character ``[``.

      :arg writer: The ``fileWriter`` to be used when serializing
      :arg size: The number of elements in the list

      :returns: A new :type:`ListSerializer`
    */
    proc startList(writer: jsonWriter, size: int) throws {
      writer.writeLiteral("[");
      return new ListSerializer(writer);
    }

    /*
      Returned by ``startList`` or ``startTuple`` to provide the API for
      serializing JSON lists.

      A list will be serialized as a comma-separated series of serialized
      elements between two square brackets. For example, serializing a list
      with elements ``1``, ``2``, and ``3`` will produce the text:

      .. code-block:: text

        [1, 2, 3]

      Empty lists will be serialized as ``[]``.
    */
    record ListSerializer {
      @chpldoc.nodoc
      var writer;
      @chpldoc.nodoc
      var _first : bool = true;

      /*
        Serialize ``element``.

        Writes a leading comma before serializing the element if this is not the
        first element in the list.
      */
      proc ref writeElement(const element: ?) throws {
        if !_first then writer.writeLiteral(", ");
        else _first = false;

        writer.write(element);
      }

      /*
        Ends serialization of the current list by writing the character ``]``.
      */
      proc endList() throws {
        writer.writeLiteral("]");
      }

      /*
        Ends serialization of the current tuple by writing the character ``]``.
      */
      proc endTuple() throws {
        endList();
      }
    }

    /*
      Start serializing an array.

      :arg writer: The ``fileWriter`` to be used when serializing
      :arg size: The number of elements in the array

      :returns: A new :type:`ArraySerializer`
    */
    proc startArray(writer: jsonWriter, size: int) throws {
      return new ArraySerializer(writer);
    }

    /*
      Returned by ``startArray`` to provide the API for serializing arrays.

      In this format, a 1D array will be serialized as a JSON list. For
      example, an array with three values would be serialized as:

      .. code-block:: text

        [1, 2, 3]

      Multidimensional arrays will be serialized as JSON lists of lists:

      .. code-block:: text

        [
         [0, 1, 2],
         [3, 4, 5],
         [6, 7, 8]
        ]

      The indenting of these multidimensional arrays is not considered stable.

    */
    record ArraySerializer {
      @chpldoc.nodoc
      var writer;
      @chpldoc.nodoc
      var _arrayDim : int;
      @chpldoc.nodoc
      var _arrayFirst : list(bool);
      @chpldoc.nodoc
      var _arrayMax : int;
      @chpldoc.nodoc
      var _first : bool = true;

      /*
        Start serializing a new dimension of size ``size``.
      */
      proc ref startDim(size: int) throws {
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
          writer.writeLiteral(",");
        }

        _arrayMax = max(_arrayMax, _arrayDim);

        // Spacing suitable to each dimension's nested level
        if _arrayDim > 1 {
          writer.writeNewline();
          writer.writeLiteral(" " * (_arrayDim-1));
        }

        // Actually start the JSON list format
        writer.writeLiteral("[");
      }

      /*
        End the current dimension.
      */
      proc ref endDim() throws {
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
          writer.writeLiteral(" " * (_arrayDim-1));
        }

        // Actually close out the list
        writer.writeLiteral("]");

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

      /*
        Serialize ``element``.

        Writes a leading comma if this is not the first element in the row.
      */
      proc ref writeElement(const element: ?) throws {
        if !_first then writer.writeLiteral(", ");
        else _first = false;

        writer.write(element);
      }

      /*
        Ends serialization of the current array.
      */
      proc endArray() throws {
      }
    }

    /*
      Start serializing a map by writing the character ``{``.

      :arg writer: The ``fileWriter`` to be used when serializing
      :arg size: The number of entries in the map

      :returns: A new :type:`MapSerializer`
    */
    proc startMap(writer: jsonWriter, size: int) throws {
      writer.writeLiteral("{");
      return new MapSerializer(writer);
    }

    /*
      Returned by ``startMap`` to provide the API for serializing maps.

      Maps are serialized as JSON objects. For example, a map of strings to
      strings would be serialized as:

      .. code-block:: text

        {
          "east": "west",
          "hello": "goodbye",
          "north": "south",
          "day": "night"
        }

      The indenting and whitespace of this example is not considered stable.

    */
    record MapSerializer {
      @chpldoc.nodoc
      var writer;
      @chpldoc.nodoc
      var _first : bool = true;

      /*
        Serialize ``key``.

        .. note::

          JSON itself only supports strings as names in objects. This module
          supports non-string key types by serializing them as an escaped JSON
          string containing the serialized key. For a simple integer key, this
          results in output like ``"123"``. For a record key with two integer
          fields, the output could look like:

          .. code-block:: text

            "{\"x\":1, \"y\":2}"

        Adds a leading comma if this is not the first pair in the map.
      */
      proc ref writeKey(const key: ?) throws {
        if !_first {
          writer.writeLiteral(", ");
          writer.writeNewline();
          writer.writeLiteral("  ");
        } else {
          writer.writeNewline();
          writer.writeLiteral("  ");
          _first = false;
        }

        if key.type == string {
          writer.write(key);
        } else {
          // Write the key as json, then turn it into a json string to use
          // it as a proper key for the map.
          var f = openMemFile();
          {
            f.writer(locking=false).withSerializer(jsonSerializer).write(key);
          }
          var tmp : string;
          f.reader(locking=false).readAll(tmp);
          writer.write(tmp);
        }
      }

      /*
        Serialize ``val``, preceded by the character ``:``.
      */
      proc writeValue(const val: ?) throws {
        writer.writeLiteral(": ");
        writer.write(val);
      }

      /*
        Ends serialization of the current map by writing the character ``}``.
      */
      proc endMap() throws {
        writer.writeNewline();
        writer.writeLiteral("}");
      }
    }
  }

  private extern proc qio_channel_skip_json_field(threadsafe:c_int, ch:qio_channel_ptr_t):errorCode;

  // Read the JSON object in ahead of time, and produce a map of field names
  // to file offsets that we can look at later in readField.
  // TODO: use a proper JSON library.
  private proc outOfOrderHelper(reader: jsonReader) throws {
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

  /*
    A JSON format deserializer to be used by :record:`~IO.fileReader`.

    See :ref:`the serializers technote<ioSerializers>` for a general overview
    of Deserializers and their usage.

    Otherwise, please refer to :type:`jsonSerializer` for a description of the
    JSON serialization format. Individual methods on this type may clarify
    behavior specific to deserialization.

    Implements the ``deserializeType`` and ``deserializeValue`` methods which are
    called by a ``fileReader`` to deserialize a serialized representation of
    a type or value in JSON format.

    This deserializer supports reading class and record fields out-of-order
    by default. I.e., the fields of a JSON object do not need to match the
    declaration order in a Chapel type definition to be deserialized into that
    type.
  */
  record jsonDeserializer {

    @chpldoc.nodoc
    proc init() {
      init this;
    }

    // TODO: rewrite in terms of writef, or something
    @chpldoc.nodoc
    proc _oldRead(ch: jsonReader, ref val:?t) throws {
      var _def = new defaultDeserializer();
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
      dc._readOne(_iokind.dynamic, val, here);
    }

    /*
      Deserialize type ``readType`` with ``reader``.

      Classes and records will be deserialized using an appropriate initializer,
      passing in ``reader`` and this Deserializer as arguments. If an
      initializer is unavailable, this method may invoke the class or record's
      ``deserialize`` method. Please see the :ref:`serializers technote<ioSerializers>` for more.

      Classes and records are expected to implement either the
      ``initDeserializable`` or ``readDeserializable`` interfaces (or both).
      Alternatively, types implementing the entire ``serializable`` interface
      are also accepted.

      :arg reader: The ``fileReader`` from which types are deserialized
      :arg readType: The type to be deserialized

      :returns: A value of type ``readType``.
    */
    proc ref deserializeType(reader: jsonReader, type readType) : readType throws {
      if isNilableClassType(readType) && reader.matchLiteral("null") {
        return nil:readType;
      }

      // TODO:
      // - escaped strings
      if isNumericType(readType) || isBoolType(readType) {
        var x : readType;
        reader._readOne(_iokind.dynamic, x, here);
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
        var ret = reader.withDeserializer(defaultDeserializer).read(readType);
        reader.readLiteral('"');
        return ret;
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
      than creating a new value. For classes and records, this method will
      first attempt to invoke a ``deserialize`` method. If the ``deserialize``
      method is unavailable, this method may fall back on invoking a suitable
      initializer and assigning the resulting value into ``val``.. Please see
      the :ref:`serializers technote<ioSerializers>` for more.

      Classes and records are expected to implement either the
      ``initDeserializable`` or ``readDeserializable`` interfaces (or both).
      Alternatively, types implementing the entire ``serializable`` interface
      are also accepted.

      :arg reader: The ``fileReader`` from which values are deserialized
      :arg val: The value into which this Deserializer will deserialize
    */
    proc ref deserializeValue(reader: jsonReader, ref val: ?readType) : void throws {
      if canResolveMethod(val, "deserialize", reader, this) {
        val.deserialize(reader=reader, deserializer=this);
      } else {
        val = deserializeType(reader, readType);
      }
    }

    /*
      Start deserializing a class by reading the character ``{``.

      :arg reader: The ``fileReader`` to use when deserializing
      :arg name: The name of the class type

      :returns: A new :type:`AggregateDeserializer`
    */
    proc startClass(reader: jsonReader, name: string) throws {
      //
      // TODO: Should we only compute the mapping if the fields are being
      // read out of order?
      //
      var (m, last) = outOfOrderHelper(reader);
      reader.readLiteral("{");

      return new AggregateDeserializer(reader, m, last);
    }

    /*
      Start deserializing a record by reading the character ``{``.

      :arg reader: The ``fileReader`` to use when deserializing
      :arg name: The name of the record type

      :returns: A new :type:`AggregateDeserializer`
    */
    proc startRecord(reader: jsonReader, name: string) throws {
      return startClass(reader, name);
    }

    /*
      Returned by ``startClass`` or ``startRecord`` to provide the API for
      deserializing classes or records.

      See ``jsonSerializer.AggregateSerializer`` for details of the JSON format
      for classes and records.
    */
    record AggregateDeserializer {
      @chpldoc.nodoc
      var reader;
      @chpldoc.nodoc
      var _fieldOffsets : map(string, int);
      @chpldoc.nodoc
      var _lastPos = -1;
      @chpldoc.nodoc
      var _parent : bool = false;

      @chpldoc.nodoc
      proc _readFieldName(reader: jsonReader, key: string) throws {
        try {
          reader.readLiteral('"');
          reader.readLiteral(key);
          reader.readLiteral('"');
          reader.readLiteral(":");
        } catch e: BadFormatError {
          return false;
        }

        return true;
      }

      /*
        Deserialize a field named ``name`` of type ``fieldType``.

        .. note::

          The position of the underlying ``fileReader`` is undefined after a
          call to ``readField`` in order to support out-of-order reading.

        :returns: A deserialized value of type ``fieldType``.
      */
      proc readField(name: string, type fieldType) : fieldType throws {
        if _fieldOffsets.contains(name) {
          // Use 'advance' instead of 'seek' to support reading in a marked
          // channel, which can happen during 'readf'.
          //
          // Use 'mark' to rewind the position since 'advance' doesn't support
          // negative values. This means that 'readField' does not advance
          // the channel's position until 'endClass' or 'endRecord' are called.
          reader.mark();
          const dist =  _fieldOffsets[name] - reader.offset();
          reader.advance(dist);
        } else if !name.isEmpty() {
          throw new IllegalArgumentError("field '" + name + "' not found.");
        }

        var ret = reader.read(fieldType);

        // note: trailing commas not allowed in json
        reader.matchLiteral(",");

        // Rewind so that we can call 'advance' again
        if !name.isEmpty() then
          reader.revert();

        return ret;
      }

      /*
        Deserialize a field named ``name`` in-place.

        .. note::

          The position of the underlying ``fileReader`` is undefined after a
          call to ``readField`` in order to support out-of-order reading.

      */
      proc readField(name: string, ref field) throws {
        if _fieldOffsets.contains(name) {
          // Use 'advance' instead of 'seek' to support reading in a marked
          // channel, which can happen during 'readf'.
          //
          // Use 'mark' to rewind the position since 'advance' doesn't support
          // negative values. This means that 'readField' does not advance
          // the channel's position until 'endClass' or 'endRecord' are called.
          reader.mark();
          const dist =  _fieldOffsets[name] - reader.offset();
          reader.advance(dist);
        } else if !name.isEmpty() {
          throw new IllegalArgumentError("field '" + name + "' not found.");
        }

        reader.read(field);

        // note: trailing commas not allowed in json
        reader.matchLiteral(",");

        // Rewind so that we can call 'advance' again
        if !name.isEmpty() then
          reader.revert();
      }

      /*
        Start deserializing a nested class inside the current class.

        See ``jsonSerializer.AggregateSerializer.startClass`` for details
        on inheritance on the JSON format.

        :returns: A new AggregateDeserializer
      */
      proc ref startClass(reader, name: string) {
        // TODO: 'ref' intent probably required by use of 'map' field...
        return new AggregateDeserializer(reader, _fieldOffsets, _lastPos, _parent=true);
      }

      /*
        End deserialization of the current class by reading the character
        ``}``.
      */
      proc endClass() throws {
        if !_parent {
          const dist =  _lastPos - reader.offset();
          reader.advance(dist);
          reader.readLiteral("}");
        }
      }

      /*
        End deserialization of the current record by reading the character
        ``}``.
      */
      proc endRecord() throws {
        endClass();
      }
    }

    /*
      Start deserializing a tuple by reading the character ``[``.

      :arg reader: The ``fileReader`` to use when deserializing

      :returns: A new :type:`ListDeserializer`
    */
    proc startTuple(reader: jsonReader) throws {
      return startList(reader);
    }

    /*
      Start deserializing a list by reading the character ``[``.

      :arg reader: The ``fileReader`` to use when deserializing

      :returns: A new :type:`ListDeserializer`
    */
    proc startList(reader: jsonReader) throws {
      reader.readLiteral("[");
      return new ListDeserializer(reader);
    }

    /*
      Returned by ``startTuple`` or ``startList`` to provide the API for
      deserializing tuples or lists.

      See ``jsonSerializer.ListSerializer`` for details of the JSON format for
      tuples and lists.
    */
    record ListDeserializer {
      @chpldoc.nodoc
      var reader;
      @chpldoc.nodoc
      var _first : bool = true;

      /*
        Deserialize and return an element.

        :returns: A deserialized value of type ``eltType``.
      */
      proc ref readElement(type eltType) : eltType throws {
        if !_first then reader.readLiteral(",");
        else _first = false;

        // preemptively check if a list will end with the next byte
        //  to avoid an uncaught BadFormatError in eltType's
        //  deserializing initializer
        if !this.hasMore()
          then throw new BadFormatError();
          else return reader.read(eltType);
      }

      /*
        Deserialize ``element`` in-place.
      */
      proc ref readElement(ref element) throws {
        if !_first then reader.readLiteral(",");
        else _first = false;

        if !this.hasMore()
          then throw new BadFormatError();
          else reader.read(element);
      }

      /*
        End deserialization of the current list by reading the character
        ``]``.
      */
      proc endList() throws {
        reader.readLiteral("]");
      }

      /*
        End deserialization of the current tuple by reading the character
        ``]``.
      */
      proc endTuple() throws {
        endList();
      }

      /*
        :returns: Returns ``true`` if there are more elements to read.
      */
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

    /*
      Start deserializing an array.

      :arg reader: The ``fileReader`` to use when deserializing

      :returns: A new :type:`ArrayDeserializer`
    */
    proc startArray(reader: jsonReader) throws {
      return new ArrayDeserializer(reader);
    }

    /*
      Returned by ``startArray`` to provide the API for deserializing arrays.

      See ``jsonSerializer.ArraySerializer`` for details of the JSON format for
      arrays.
    */
    record ArrayDeserializer {
      @chpldoc.nodoc
      var reader;
      @chpldoc.nodoc
      var _first : bool = true;
      @chpldoc.nodoc
      var _arrayDim = 0;
      @chpldoc.nodoc
      var _arrayMax = 0;
      @chpldoc.nodoc
      var _arrayFirst : list(bool);

      /*
        Inform the ``ArrayDeserializer`` to start deserializing a new dimension.
      */
      proc ref startDim() throws {
        _arrayDim += 1;
        if _arrayFirst.size < _arrayDim {
          _arrayFirst.pushBack(true);
        }

        if _arrayFirst[_arrayDim-1] {
          _arrayFirst[_arrayDim-1] = false;
        } else {
          reader.readLiteral(",");
        }

        _arrayMax = max(_arrayMax, _arrayDim);

        // Don't need to read the newline and pretty-printed spaces, as JSON
        // arrays can come in other forms. Relies on 'readLiteral' ignoring
        // whitespace by default.
        reader.readLiteral("[");
      }

      /*
        End deserialization of the current dimension.
      */
      proc ref endDim() throws {
        if _arrayDim < _arrayMax {
          reader.readNewline();
          reader.readLiteral(" " * (_arrayDim-1));
        }

        // Don't need to read the newline and pretty-printed spaces, as JSON
        // arrays can come in other forms. Relies on 'readLiteral' ignoring
        // whitespace by default.
        reader.readLiteral("]");

        if _arrayDim < _arrayFirst.size then
          _arrayFirst[_arrayDim] = true;

        _arrayDim -= 1;
        _first = true;
      }

      /*
        Deserialize and return an element of the array.

        :returns: A deserialized value of type ``eltType``.
      */
      proc ref readElement(type eltType) : eltType throws {
        if !_first then reader.readLiteral(", ");
        else _first = false;

        return reader.read(eltType);
      }

      /*
        Deserialize ``element`` in-place as an element of the array.
      */
      proc ref readElement(ref element) throws {
        if !_first then reader.readLiteral(", ");
        else _first = false;

        reader.read(element);
      }

      /*
        End deserialization of the current array.
      */
      proc endArray() throws {
      }
    }

    /*
      Start deserializing a map by reading the character ``{``.

      :arg reader: The ``fileReader`` to use when deserializing

      :returns: A new :type:`MapDeserializer`
    */
    proc startMap(reader: jsonReader) throws {
      reader.readLiteral("{");
      return new MapDeserializer(reader);
    }

    /*
      Returned by ``startMap`` to provide the API for deserializing arrays.

      See ``jsonSerializer.MapSerializer`` for details of the JSON format for
      maps.
    */
    record MapDeserializer {
      @chpldoc.nodoc
      var reader;
      @chpldoc.nodoc
      var _first : bool = true;

      /*
        Deserialize and return a key of type ``keyType``.
      */
      proc ref readKey(type keyType) : keyType throws {
        if !_first then reader.readLiteral(",");
        else _first = false;

        if keyType == string {
          return reader.read(string);
        } else {
          // Read in a JSON string, write it to a memory file, then read that
          // file as a JSON entity for the 'keyType'.
          var f = openMemFile();
          var s = reader.read(string);
          {
            f.writer(locking=false).withSerializer(defaultSerializer).write(s);
          }
          return f.reader(locking=false).withDeserializer(jsonDeserializer).read(keyType);
        }
      }

      /*
        Deserialize ``key`` in-place as a key of the map.
      */
      proc ref readKey(ref key: ?t) throws {
        if !_first then reader.readLiteral(",");
        else _first = false;

        if t == string || t == bytes {
          reader.read(key);
        } else {
          // Read in a JSON string, write it to a memory file, then read that
          // file as a JSON entity for the 'readType'.
          var f = openMemFile();
          var s = reader.read(string);
          {
            f.writer(locking=false).withSerializer(defaultSerializer).write(s);
          }
          return f.reader(locking=false).withDeserializer(jsonDeserializer).read(key);
        }
      }

      /*
        Deserialize and return a value of type ``valType``.
      */
      proc readValue(type valType) : valType throws {
        reader.readLiteral(":");
        return reader.read(valType);
      }

      /*
        Deserialize ``value`` in-place as a value of the map.
      */
      proc readValue(ref value) throws {
        reader.readLiteral(":");
        reader.read(value);
      }

      /*
        End deserialization of the current map by reading the character ``}``.
      */
      proc endMap() throws {
        reader.readLiteral("}");
      }

      /*
        :returns: Returns ``true`` if there are more elements to read.

        .. warning::

          Behavior of 'hasMore' is undefined when called between ``readKey`` and
          ``readValue``.
      */
      proc hasMore() : bool throws {
        reader.mark();
        defer reader.revert();
        return !reader.matchLiteral("}");
      }
    }
  }

  /* Given a JSON string, use the :type:`jsonDeserializer` to deserialize it
     into a value of type ``loadAs``. The type must support deserialization
     using the ``readDeserializable`` interface.

     :arg jsonString: The JSON string to parse into a Chapel value
     :arg loadAs: The type to deserialize the JSON string into

     :returns: A value of type ``loadAs``.
   */
  @unstable
  proc fromJson(jsonString: string, type loadAs): loadAs throws {
    var fileReader = openStringReader(jsonString,
                                      deserializer=new jsonDeserializer());
    return fileReader.read(loadAs);
  }

  /* Given a Chapel value, serialize it into a JSON string using the
     :type:`jsonSerializer`. The type must support serialization using the
     ``writeSerializable`` interface.

     :arg arg: The value to serialize into a JSON string

     :returns: A JSON string representing the Chapel value
   */
  @unstable
  proc toJson(arg): string throws {
    var memWriter = openMemFile();
    var writer = memWriter.writer(locking=false).withSerializer(jsonSerializer);
    writer.write(arg);
    writer.close();

    return memWriter.reader(locking=false).readAll(string);
  }
}
