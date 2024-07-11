module PrecisionSerializer {
  use IO;
  use CTypes;

  /*
    Default precision to be used by the ``precisionSerializer``
  */
  config const defaultPrecisionSerializerPrecision = 12;

  /*
    Default padding to be used by the ``precisionSerializer``
  */
  config const defaultPrecisionSerializerPadding = 0;

  /*
    A variation of the :record:`~IO.defaultSerializer` that allows for
    finer control over formatting of numerical values.

    See :ref:`the serializers technote<ioSerializers>` for a general overview
    of Serializers and their usage.
  */
  @unstable("'precisionSerializer' is an experimental feature and may change in the future.")
  record precisionSerializer {

    @chpldoc.nodoc
    var precision: int;
    @chpldoc.nodoc
    var padding: int;

    /*
      Create a ``precisionSerializer`` with the default precision and padding
      settings for formatting numeric values.
    */
    proc init() {
      this.precision = defaultPrecisionSerializerPrecision;
      this.padding = defaultPrecisionSerializerPadding;
    }

    /*
      Create a ``precisionSerializer`` with particular precision and padding
      settings for formatting numeric values.

      For example, with a precision of 3 and padding of 5, the number 3.14159
      would be serialized as `` 3.141``.

      :arg precision: The number of digits to display after the decimal point
                      when serializing real values.
      :arg padding: The number of columns to pad the number with when serialzing
                    real values, or the minimum column width when serializing
                    integral values. If zero, no padding/width is enforced.
    */
    proc init(precision: int, padding: int = 0) {
      this.precision = precision;
      this.padding = padding;
    }

    @chpldoc.nodoc
    proc ref _serializeClassOrPtr(writer:fileWriter, x: ?t) : void throws {
      if x == nil {
        writer.writeLiteral("nil");
      } else if isClassType(t) {
        x!.serialize(writer=writer, serializer=this);
      } else {
        x.serialize(writer=writer, serializer=this);
      }
    }

    /*
      Serialize ``val`` with ``writer``.

      Real values are serialized with either ``%*.*r`` or ``%.*r`` depending on
      whether a non-zero padding was specified.

      Integral values are serialized with either ``%*i`` or ``%i`` depending on
      whether a non-zero padding was specified.

      Complex numbers are serialized as ``%z``. Please refer to :ref:`the section
      on Formatted IO<about-io-formatted-io>` for more information.

      Booleans are serialized as the literal strings ``true`` or ``false``.

      ``string`` values are serialized using the same format as ``%s`` — that is,
      literally and without quotes. ``bytes`` values are also serialized
      literally without extra formatting.

      Enums are serialized using the name of the corresponding value. For example
      with an enum like ``enum colors {red, green blue}``, the value ``red``
      would simply be serialized as ``red``.

      The ``nil`` value and nilable class variables storing ``nil`` will be
      serialized as the text ``nil``.

      Classes and records will have their ``serialize`` method invoked, passing
      in ``writer`` and this Serializer as arguments. Please see the
      :ref:`serializers technote<ioSerializers>` for more information.

      Classes and records are expected to implement the ``writeSerializable``
      or ``serializable`` interface.

      :arg writer: The ``fileWriter`` used to write serialized output.
      :arg val: The value to be serialized.
    */
    proc ref serializeValue(writer: fileWriter, const val: ?t) : void throws {
      if isNumericType(t) || isBoolType(t) || isEnumType(t) ||
        t == string || t == bytes {
        if isRealType(t) {
          if this.padding > 0 {
            writer.writef("%*.*dr", this.padding, this.precision, val);
          } else {
            writer.writef("%.*dr", this.precision, val);
          }
        } else if isIntegralType(t) {
          if this.padding > 0 {
            writer.writef("%*i", this.padding, val);
          } else {
            writer._writeOne(_iokind.dynamic, val, writer.getLocaleOfIoRequest());
          }
        } else {
          writer._writeOne(_iokind.dynamic, val, writer.getLocaleOfIoRequest());
        }
      } else if t == _nilType {
        writer.writeLiteral("nil");
      } else if isClassType(t) || isAnyCPtr(t) || chpl_isDdata(t) {
        _serializeClassOrPtr(writer, val);
      } else if isUnionType(t) {
        // From ChapelIO
        // Note: Some kind of weird resolution bug with ChapelIO.writeThis...
        writeThisDefaultImpl(writer, val);
      } else {
        val.serialize(writer=writer, serializer=this);
      }
    }

    // TODO: add ":ref:" for return type, currently can't refer to it.
    /*
      Start serializing a class by writing the character ``{``.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg name: The name of the class type.
      :arg size: The number of fields in the class.

      :returns: A new :type:`AggregateSerializer`
    */
    proc startClass(writer: fileWriter, name: string, size: int) throws {
      writer.writeLiteral("{");
      return new AggregateSerializer(writer, _ending="}");
    }

    /*
      Start serializing a record by writing the character ``(``.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg name: The name of the record type.
      :arg size: The number of fields in the record.

      :returns: A new :type:`AggregateSerializer`
    */
    proc startRecord(writer: fileWriter, name: string, size: int) throws {
      writer.writeLiteral("(");
      return new AggregateSerializer(writer, _ending=")");
    }

    /*
      Returned by ``startClass`` or ``startRecord`` to provide the API for
      serializing classes or records.

      A ``class`` with integer fields 'x' and 'y' with values '0' and '5' would
      be serialized as:

      .. code-block:: text

        {x = 0, y = 5}

      A ``record`` with matching fields would be serialized in the same way, but
      would use ``(`` and ``)`` instead of ``{`` and ``}``.
    */
    record AggregateSerializer {
      @chpldoc.nodoc
      var writer;
      @chpldoc.nodoc
      var _parent = false;
      @chpldoc.nodoc
      var _first : bool = true;
      @chpldoc.nodoc
      const _ending : string;

      // pointer to child's 'first' field so that we can communicate back if/when
      // a field has already been written.
      @chpldoc.nodoc
      var _firstPtr : c_ptr(bool) = nil;

      /*
        Serialize ``field`` named ``name``.

        Serializes fields in the form '<name> = <field>'. Adds a comma before the
        name if this is not the first field.
      */
      proc ref writeField(name: string, const field: ?) throws {
        if !_first then writer.writeLiteral(", ");
        else _first = false;

        writer.writeLiteral(name);
        writer.writeLiteral(" = ");
        writer.write(field);
      }

      /*
        Start serializing a nested class inside the current class. In this format
        inheritance is not represented and parent fields are printed before child
        fields. For example, the following classes with values
        ``x=5`` and ``y=2.0``:

        .. code-block:: chapel

          class Parent {
            var x : int;
          }

          class Child: Parent {
            var y : real;
          }

        would be serialized as:

        .. code-block:: text

          {x = 5, y = 2.0}

        :arg writer: The ``fileWriter`` to be used when serializing. Must match
                    the writer used to create current AggregateSerializer.
        :arg name: The name of the class type.
        :arg size: The number of fields in the class.

        :returns: A new :record:`~IO.defaultSerializer.AggregateSerializer`
      */
      proc ref startClass(writer: fileWriter, name: string, size: int) throws {
        // Note: 'size' of parent might be zero, but 'size' of grandparent might
        // be non-zero.
        return new AggregateSerializer(writer, _parent=true,
                                      _firstPtr=c_addrOf(_first));
      }

      /*
        Ends serialization of the current class by writing the character ``}``

        .. note:: It is an error to call methods on an AggregateSerializer after
                  invoking 'endClass'.
      */
      proc endClass() throws {
        if !_parent then
          writer.writeLiteral(_ending);
        else if _firstPtr != nil then
          _firstPtr.deref() = _first;
      }

      /*
        Ends serialization of the current record by writing the character ``)``

        .. note:: It is an error to call methods on an AggregateSerializer after
                  invoking 'endRecord'.
      */
      proc endRecord() throws {
        writer.writeLiteral(_ending);
      }
    }

    /*
      Start serializing a tuple by writing the character ``(``.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg size: The number of elements in the tuple.

      :returns: A new :record:`TupleSerializer`
    */
    proc startTuple(writer: fileWriter, size: int) throws {
      writer.writeLiteral("(");
      return new TupleSerializer(writer, size);
    }

    /*
      Returned by ``startTuple`` to provide the API for serializing tuples.

      A tuple will be serialized as a comma-separated list between two
      parentheses. For example, the tuple literal ``(1, 2, 3)`` would be
      serialized as:

      .. code-block::

        (1, 2, 3)

      A 1-tuple will be serialized with a trailing comma. For example, the literal
      ``(4,)`` would be serialized as ``(4,)``.

    */
    record TupleSerializer {
      @chpldoc.nodoc
      var writer;
      @chpldoc.nodoc
      const size : int;
      @chpldoc.nodoc
      var _first : bool = true;

      /*
        Serialize ``element``.

        Writes a leading comma before serializing the element if this is not the
        first element in the tuple.
      */
      proc ref writeElement(const element: ?) throws {
        if !_first then writer.writeLiteral(", ");
        else _first = false;

        writer.write(element);
      }

      /*
        Ends serialization of the current tuple by writing the character ``)``.

        Adds a comma between the last value and ``)`` if there was only one
        element.
      */
      proc endTuple() throws {
        if size == 1 then
          writer.writeLiteral(",)");
        else
          writer.writeLiteral(")");
      }
    }

    /*
      Start serializing a list by writing the character ``[``.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg size: The number of elements in the list.

      :returns: A new :record:`ListSerializer`
    */
    proc startList(writer: fileWriter, size: int) throws {
      writer.writeLiteral("[");
      return new ListSerializer(writer);
    }

    /*
      Returned by ``startList`` to provide the API for serializing lists.

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
    }

    /*
      Start serializing an array.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg size: The number of elements in the array.

      :returns: A new :record:`ArraySerializer`
    */
    proc startArray(writer: fileWriter, size: int) throws {
      return new ArraySerializer(writer);
    }

    /*
      Returned by :proc:`~IO.defaultSerializer.startArray` to provide the API for
      serializing arrays.

      In the default format, an array will be serialized as a
      whitespace-separated series of serialized elements.

      A 1D array is serialized simply using spaces:

      ::

        1 2 3 4

      A 2D array is serialized using spaces between elements in a row, and
      prints newlines for new rows:

      ::

        1 2 3
        4 5 6
        7 8 9

      Arrays with three or more dimensions will be serialized as a series of
      2D "panes", with multiple newlines separating new dimensions:

      ::

        1 2 3
        4 5 6
        7 8 9

        10 11 12
        13 14 15
        16 17 18

        19 20 21
        22 23 24
        25 26 27

      Empty arrays result in no output to the :record:`fileWriter`.
    */
    record ArraySerializer {
      @chpldoc.nodoc
      var writer;
      @chpldoc.nodoc
      var _arrayDim : int;
      @chpldoc.nodoc
      var _arrayMax : int;
      @chpldoc.nodoc
      var _first : bool = true;

      /*
        Inform the :record:`~IO.defaultSerializer.ArraySerializer` to start
        serializing a new dimension of size ``size``.
      */
      proc ref startDim(size: int) throws {
        _arrayDim += 1;

        if _arrayMax >= _arrayDim then
          writer.writeNewline();
        else
          _arrayMax = _arrayDim;
      }

      /*
        End the current dimension.
      */
      proc ref endDim() throws {
        _arrayDim -= 1;
        _first = true;
      }

      /*
        Serialize ``element``.

        Adds a space if this is not the first element in the row.
      */
      proc ref writeElement(const element: ?) throws {
        if !_first then writer.writeLiteral(" ");
        else _first = false;

        writer.write(element);
      }

      /*
        Ends serialization of the current array.
      */
      proc endArray() throws { }
    }

    /*
      Start serializing a map by writing the character ``{``.

      :arg writer: The ``fileWriter`` to be used when serializing.
      :arg size: The number of entries in the map.

      :returns: A new :record:`MapSerializer`
    */
    proc startMap(writer: fileWriter, size: int) throws {
      writer.writeLiteral("{");
      return new MapSerializer(writer);
    }

    /*
      Returned by :proc:`~IO.defaultSerializer.startMap` to provide the API for
      serializing maps.

      Maps are serialized as a comma-separated series of pairs between curly
      braces. Pairs are serialized with a ``:`` separating the key and value. For
      example, the keys ``1``, ``2``, and ``3`` with values corresponding to
      their squares would be serialized as:

      ::

        {1: 1, 2: 4, 3: 9}

      Empty maps will be serialized as ``{}``.
    */
    record MapSerializer {
      @chpldoc.nodoc
      var writer;
      @chpldoc.nodoc
      var _first : bool = true;

      /*
        Serialize ``key``.

        Adds a leading comma if this is not the first pair in the map.
      */
      proc ref writeKey(const key: ?) throws {
        if !_first then writer.writeLiteral(", ");
        else _first = false;

        writer.write(key);
      }

      /*
        Serialize ``val``, preceded by the character ``:``.
      */
      proc writeValue(const val: ?) throws {
        writer.writeLiteral(": ");
        writer.write(val);
      }

      /*
        Ends serialization of the current map by writing the character ``}``
      */
      proc endMap() throws {
        writer.writeLiteral("}");
      }
    }
  }
}
