/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

Support for parsing and writing YAML files.

Includes a Yaml Serializer/Deserializer as well as an abstract class
representation of a YAML file.

The Yaml Serializer/Deserializer is intended to be used with the standard
IO module's serialization/deserialization API. For example:

.. code-block:: chpl

  use Yaml;

  record R {
    var a: int;
    var b: string;
  }

  var myFile = open("r.yaml", iomode.cwr),
      r1 = new R(1, "hello");

  myFile.writer().withSerializer(new YamlSerializer()).write(r1);

  /* r.yaml:
    --- R!
    a: 1
    b: hello
    ...
  */

  var r2 = myFile.reader().withDeserializer(new YamlDeserializer()).read(R);
  assert(r == r2);

Yaml files can also be parsed and written directly using the YamlValue class.

Dependencies
------------

This module depends on the ``libyaml`` library. Installation instructions
can be found `here <https://github.com/yaml/libyaml>`_.

To compile a program that uses the Yaml module, you will either need to set the
``CHPL_INCLUDE_PATH`` and ``CHPL_LIB_PATH`` environment variables with
``libyaml``s installation location, or use the ``-l`` and ``-L`` compiler
flags to specify the location of the library.


*/

module Yaml {
  @chpldoc.nodoc
  config param YamlVerbose = false;

  // include module LibYamlUtils;
  private use LibYamlUtils, IO, Map, List;

  // ----------------------------------------------------
  // Serializer/Deserializer Definitions
  // ----------------------------------------------------

  /* Type Alias for a :record:`fileWriter` that uses a YamlSerializer */
  type yamlWriter = fileWriter(serializerType=YamlSerializer, ?);
  /* Type Alias for a :record:`fileReader` that uses a YamlDeserializer */
  type yamlReader = fileReader(deserializerType=YamlDeserializer, ?);

  record YamlSerializer {
    @chpldoc.nodoc
    var emitter: shared LibYamlEmitter;

    @chpldoc.nodoc
    var context: shared ContextCounter;

    proc init (
      seqStyle = YamlSequenceStyle.Any,
      mapStyle = YamlMappingStyle.Any,
      scalarStyle = YamlScalarStyle.Any,
      documentStyle = YamlDocumentStyle.Implicit
    ) {
      emitter = new shared LibYamlEmitter(seqStyle, mapStyle, scalarStyle, documentStyle);
      context = new shared ContextCounter();
    }

    // initializer for copying
    @chpldoc.nodoc
    proc init(e: shared LibYamlEmitter, c: shared ContextCounter) {
      emitter = e;
      context = c;
    }

    // ------- fileWriter API -------

    proc serializeValue(writer: yamlWriter, const val: ?t) throws {
      if context.isBase && (_isPrimitiveYamlType(t) || (isClassType(t) && val == nil)){
        // simply translate the value to it's YAML representation
        // e.g., true => Yes, nil => ~, 5.5 => 5.5, etc.
        writer.writeBytes(this.emitter.yamlStringifyPrimitive(val, true));
      } else {
        // translate the value within the current emitter context
        if _isPrimitiveYamlType(t) {
          this.emitter.emitScalar(
            this.emitter.yamlStringifyPrimitive(val, false),
            // don't use the default format if the argument isn't a string or bytes
            styleHint=(if t != string && t != bytes then YamlScalarStyle.Any else YamlScalarStyle.Default)
          );
        } else if isClassType(t) {
          if val == nil
            then this.emitter.emitScalar(b"~", styleHint=YamlScalarStyle.Any);
            else val!.serialize(writer, new YamlSerializer(this.emitter, this.context));
        } else {
          val.serialize(writer, new YamlSerializer(this.emitter, this.context));
        }
      }
    }

    // ------- serialize() API -------

    proc startClass(writer: yamlWriter, name: string, size: int) throws {
      this.context.enterClass();
      this._startMapping(writer, name);
    }
    proc endClass(writer: yamlWriter) throws {
      this._endMapping(writer);
      this.context.leaveClass();
    }

    proc startRecord(writer: yamlWriter, name: string, size: int) throws {
      this._startMapping(writer, name);
    }
    proc endRecord(writer: yamlWriter) throws {
      this._endMapping(writer);
    }

    proc startTuple(writer: yamlWriter, size: int) throws {
      this._startSequence(writer);
    }
    proc endTuple(writer: yamlWriter) throws {
      this._endSequence(writer);
    }

    proc serializeField(writer: yamlWriter, name: string, const val: ?t) throws {
      if name.size > 0 then
        this.emitter.emitScalar(name: bytes);
      this.serializeValue(writer, val);
    }

    proc startList(writer: yamlWriter, size: uint) throws {
      this._startSequence(writer);
    }
    proc endList(writer: yamlWriter) throws {
      this._endSequence(writer);
    }
    proc writeListElement(writer: yamlWriter, const val) throws {
      this.serializeValue(writer, val);
    }

    proc startArray(writer: yamlWriter, size: uint = 0) throws {}
    proc endArray(writer: yamlWriter) throws {}
    proc startArrayDim(writer: yamlWriter, len: uint) throws {
      this._startSequence(writer);
    }
    proc endArrayDim(writer: yamlWriter) throws {
      this._endSequence(writer);
    }
    proc writeArrayElement(writer: yamlWriter, const elt) throws {
      this.serializeValue(writer, elt);
    }

    proc startMap(writer: yamlWriter, size: uint) throws {
      this._startMapping(writer);
    }
    proc endMap(writer: yamlWriter) throws {
      this._endMapping(writer);
    }
    proc writeKey(writer: yamlWriter, const key) throws {
      this.serializeValue(writer, key);
    }
    proc writeValue(writer: yamlWriter, const val) throws {
      this.serializeValue(writer, val);
    }
  }

  record YamlDeserializer {
    @chpldoc.nodoc
    var parser: shared LibYamlParser;

    @chpldoc.nodoc
    var context: shared ContextCounter;

    var strictTypeChecking: bool;

    proc init(strictTypeChecking: bool = false) {
      parser = new shared LibYamlParser();
      context = new shared ContextCounter();
      this.strictTypeChecking = strictTypeChecking;
    }

    // initializer for copying
    @chpldoc.nodoc
    proc init(p: shared LibYamlParser, c: shared ContextCounter, stc: bool) {
      parser = p;
      context = c;
      strictTypeChecking = stc;
    }

    // ------- fileReader API -------

    proc deserializeValue(reader: yamlReader, ref val: ?t) throws {
      if YamlVerbose then writeln("deserializing into: ", val);

      // TODO: full implementation
      var x = this.deserializeType(reader, t);
      val = x;
    }

    proc deserializeType(reader: yamlReader, type t): t throws {
      if YamlVerbose then writeln("deserializing type: ", t:string);

      if context.isBase && _isIoPrimitiveType(t) {
        // parse primitive without context
        if isBoolType(t) {
          // TODO: make this more robust
          const value = reader.readTo(" ");
          return parseYamlBool(value);
        } else{
          var default = reader.withDeserializer(new DefaultDeserializer());
          return default.read(t);
        }
      } else if _isIoPrimitiveType(t) {
        // parse primitive in context
        const value = this._getScalar(reader);
        return YamlParsePrimitive(value, t);
      } else if isNilableClass(t) {
        return this.speculativeParseNilableClass(reader, t);
      } else if canResolveTypeMethod(t, "deserializeFrom", reader, this) || isArrayType(t) {
        // attempt to call the 'deserializeFrom' type-method
        var r = reader.withDeserializer(new YamlDeserializer(this.parser, this.context, this.strictTypeChecking));
        return t.deserializeFrom(reader=r, deserializer=r.deserializer);
      } else {
        // attempt to call the deserializing constructor
        var r = reader.withDeserializer(new YamlDeserializer(this.parser, this.context, this.strictTypeChecking));
        return new t(reader=r, deserializer=r.deserializer);
      }
    }

    // ------- deserialize() API -------

    proc startClass(reader: yamlReader, name: string) throws {
      if YamlVerbose then writeln("starting class: ", name);

      this.context.enterClass();
      this._startMapping(reader, name);
    }
    proc endClass(reader: yamlReader) throws {
      if YamlVerbose then writeln("ending class");

      this._endMapping(reader);
      this.context.leaveClass();
    }

    proc startRecord(reader: yamlReader, name: string) throws {
      if YamlVerbose then writeln("starting record: ", name);

      this._startMapping(reader, name);
    }
    proc endRecord(reader: yamlReader) throws {
      if YamlVerbose then writeln("ending record");

      this._endMapping(reader);
    }

    proc startTuple(reader: yamlReader) throws {
      if YamlVerbose then writeln("starting tuple");

      this._startSequence(reader);
    }
    proc endTuple(reader: yamlReader) throws {
      if YamlVerbose then writeln("ending tuple");

      this._endSequence(reader);
    }

    proc deserializeField(reader: yamlReader, name: string, type t): t throws {
      if YamlVerbose then writeln("deserializing field: ", name, " of type: ", t:string);

      if name.size > 0 {
        var foundName: string;
        try {
          foundName = this._getScalar(reader);
        } catch e: YamlUnexpectedEventError {
          throw new BadFormatError("unexpected event: " + e.message());
        }
        if foundName != name then
          throw new BadFormatError("unexpected field name: " + foundName + " (expected: " + name + ")");
      }

      const value = this.deserializeType(reader, t);
      if YamlVerbose then writeln("  got value: ", value);
      return value;
    }

    proc startArray(reader: yamlReader) throws { }
    proc endArray(reader: yamlReader) throws { }
    proc startArrayDim(reader: yamlReader) throws {
      this._startSequence(reader);
    }
    proc endArrayDim(reader: yamlReader) throws {
      this._endSequence(reader);
    }
    proc readArrayElement(reader: yamlReader, type t): t throws {
      if this.parser.peekFor(reader, EventType.SequenceEnd)
        then throw new BadFormatError("sequence end event");
        else return reader.read(t);
    }

    proc startList(reader: yamlReader) throws {
      this._startSequence(reader);
    }
    proc endList(reader: yamlReader) throws {
      this._endSequence(reader);
    }
    proc readListElement(reader: yamlReader, type t): t throws {
      if this.parser.peekFor(reader, EventType.SequenceEnd)
        then throw new BadFormatError("sequence end event");
        else return reader.read(t);
    }

    proc startMap(reader: yamlReader) throws {
      this._startMapping(reader, "");
    }
    proc endMap(reader: yamlReader) throws {
      this._endMapping(reader);
    }
    proc readKey(reader: yamlReader, type t): t throws {
      if this.parser.peekFor(reader, EventType.MappingEnd)
        then throw new BadFormatError("mapping end event");
        else return reader.read(t);
    }
    proc readValue(reader: yamlReader, type t): t throws {
      return reader.read(t);
    }
  }

  // ------- serializer helpers -------

  @chpldoc.nodoc
  proc YamlSerializer._startMapping(writer: yamlWriter, name: string = "") throws {
    if this.context.isBase then this.emitter.openContext(
      if name.size > 0
        then YamlDocumentStyle.Explicit
        else YamlDocumentStyle.Default
    );
    this.context.enter();

    if !this.context.inSuperClass {
      var nb = if name.size > 0 then b"!" + name: bytes else b"";
      this.emitter.startMapping(nb);
    }
  }

  @chpldoc.nodoc
  proc YamlSerializer._endMapping(writer: yamlWriter) throws {
    this.context.leave();
    if !this.context.inSuperClass then this.emitter.endMapping();
    if this.context.isBase then writer.writeBytes(this.emitter.closeContext());
  }

  @chpldoc.nodoc
  proc YamlSerializer._startSequence(writer: yamlWriter, name: string = "") throws {
    if this.context.isBase then this.emitter.openContext();
    this.context.enter();

    var nb = if name.size > 0 then b"!" + name: bytes else b"";
    this.emitter.startSequence(nb);
  }

  @chpldoc.nodoc
  proc YamlSerializer._endSequence(writer: yamlWriter) throws {
    this.context.leave();
    this.emitter.endSequence();
    if this.context.isBase then writer.writeBytes(this.emitter.closeContext());
  }

  @chpldoc.nodoc
  proc _isPrimitiveYamlType(type t) param: bool {
    return isBoolType(t) || isNumericType(t) || isEnumType(t) ||
      isNothingType(t) || isRangeType(t) || isDomainType(t) ||
      t == string || t == bytes;
  }

  // ---- deserializer helpers -------

  @chpldoc.nodoc
  proc YamlDeserializer.YamlParsePrimitive(rawValue: string, type t): t throws {
    if YamlVerbose then writeln("parsing primitive: ", t:string, " from: ", rawValue);

    if rawValue.startsWith("!!") {
      // yaml native type annotation
      const (tag, _, value) = rawValue.partition(" ");
      var matches = true;
      if this.strictTypeChecking {
        select tag {
          when "!!float" do matches = t == real;
          when "!!int" do matches = t == int;
          when "!!str" do matches = t == string || t == bytes;
          when "!!bool" do matches = t == bool;
          when "!!binary" do matches = t == bytes; //???
          otherwise matches = false;
        }
      }

      if !matches then throw new YamlParserError("Cannot parse a '" + tag + "' as a '" + t:string + "'");
      return if t == bool
        then parseYamlBool(value)
        else value : t;

    } else if rawValue.startsWith("!") {
      // user defined type annotation
      const (tag, _, _) = rawValue.partition(" ");
      throw new YamlParserError("Cannot parse a '" + tag + "' as a '" + t:string + "'");

    } else {
      // no type annotation
      return if t == bool
        then parseYamlBool(rawValue)
        else rawValue : t;
    }
  }

  @chpldoc.nodoc
  proc YamlDeserializer.speculativeParseNilableClass(reader: yamlReader, type t): t throws {
    if context.isBase {
      // parse nilable class without context

      reader.mark();
      // TODO: make this more robust
      const nextValue = reader.readTo(" ");

      if nextValue == "~" {
        reader.commit();
        return nil;
      } else {
        reader.revert();
        var r = reader.withDeserializer(new YamlDeserializer(this.parser, this.context, this.strictTypeChecking));
        return new t(reader=r, deserializer=r.deserializer);
      }
    } else {
      // parse nilable class in context
      try {
      const maybeNil = this._getScalar(reader);
      if maybeNil == "~"
        then return nil;
        else throw new YamlParserError("expected Class or nil value, got scalar: " + maybeNil);
      } catch e: YamlUnexpectedEventError {
        var r = reader.withDeserializer(new YamlDeserializer(this.parser, this.context, this.strictTypeChecking));
        return new t(reader=r, deserializer=r.deserializer);
      }
    }
    return nil;
  }

  @chpldoc.nodoc
  private proc parseYamlBool(b: string): bool throws {
    if b == "Yes" || b == "yes" || b == "YES" || b == "True" || b == "true" || b == "TRUE" {
      return true;
    } else if b == "No" || b == "no" || b == "NO" || b == "False" || b == "false" || b == "FALSE" {
      return false;
    } else {
      throw new YamlParserError("Cannot parse '" + b + "' as a boolean");
    }
  }

  @chpldoc.nodoc
  proc YamlDeserializer._startMapping(reader: yamlReader, name: string) throws {
    // TODO: extract the type name from the document/mapping start and do type checking

    // start a document if we're in the base context
    if this.context.isBase
      then this.parser.expectEvent(reader, EventType.DocumentStart);

    // start a mapping if we're not entering superclass's deserializer
    if !this.context.inSuperClass
      then this.parser.expectEvent(reader, EventType.MappingStart);

    this.context.enter();
  }

  @chpldoc.nodoc
  proc YamlDeserializer._endMapping(reader: yamlReader) throws {
    this.context.leave();

    // end the mapping if we're not leaving a superclass's deserializer
    if !this.context.inSuperClass
      then this.parser.expectEvent(reader, EventType.MappingEnd);

    // end the document if we've returned to the base context
    if this.context.isBase
      then this.parser.expectEvent(reader, EventType.DocumentEnd);
  }

  @chpldoc.nodoc
  proc YamlDeserializer._startSequence(reader: yamlReader) throws {
    if YamlVerbose then writeln("\tstarting sequence");

    // start a document if we're in the base context
    if this.context.isBase
      then this.parser.expectEvent(reader, EventType.DocumentStart);

    // start a sequence
    this.parser.expectEvent(reader, EventType.SequenceStart);

    this.context.enter();
  }

  @chpldoc.nodoc
  proc YamlDeserializer._endSequence(reader: yamlReader) throws {
    if YamlVerbose then writeln("\nending sequence");
    this.context.leave();

    // end a sequence
    this.parser.expectEvent(reader, EventType.SequenceEnd);

    // if we're in the base context, end the document
    if this.context.isBase
      then this.parser.expectEvent(reader, EventType.DocumentEnd);
  }

  @chpldoc.nodoc
  proc YamlDeserializer._speculativeRead(reader: yamlReader, type t) throws {
    try {
      return reader.read(t);
    } catch e: YamlUnexpectedEventError {
      throw new BadFormatError("unexpected event: " + e.message());
    } catch e { throw e; }
  }

  @chpldoc.nodoc
  proc  YamlDeserializer._getScalar(reader: yamlReader): string throws {
    const (start, end) = this.parser.expectEvent(reader, EventType.Scalar);
    reader.seek((start:int)..);
    return reader.readString((end - start):int);
  }


  // ----------------------------------------------------
  // Enums and Error Types
  // ----------------------------------------------------

  enum YamlSequenceStyle {
    Default,
    Any,
    Block,
    Flow
  }

  enum YamlMappingStyle {
    Default,
    Any,
    Block,
    Flow
  }

  enum YamlScalarStyle {
    Default,
    Any,
    Plain,
    SingleQuoted,
    DoubleQuoted,
    Literal,
    Folded
  }

  enum YamlDocumentStyle {
    Default,
    Implicit,
    Explicit
  }

  class YamlEmitterError: Error {
    proc init(msg: string) {
      super.init(msg);
    }
  }

  class YamlParserError: Error {
    proc init(msg: string) {
      super.init(msg);
    }
  }

  class YamlUnexpectedEventError: YamlParserError {
    proc init(expected: string, actual: string) {
      super.init("Expected event: '" + expected + "', but got '" + actual + "'.");
    }
  }

  // ----------------------------------------------------
  // Yaml Abstract Class
  // ----------------------------------------------------

  /* write a yaml file to the give path */
  proc writeYamlFile(path: string, value: borrowed YamlValue) throws {
    var emitter = new owned LibYamlEmitter();
    emitter.openFile(path);

    emitter._startOutputStream();
    emitter.startDocument();

    emitYamlValue(emitter, value);

    emitter.endDocument();
    emitter._endOutputStream();
  }

  @chpldoc.nodoc
  proc emitYamlValue(emitter: borrowed LibYamlEmitter, value: borrowed YamlValue) throws {
    select value.valueType {
      when YamlValueType.Mapping {
        emitter.startMapping();
        const x = value: borrowed YamlMapping;
        for (key, val) in x {
          emitYamlValue(emitter, key);
          emitYamlValue(emitter, val);
        }
        emitter.endMapping();
      }
      when YamlValueType.Sequence {
        const x = value: borrowed YamlSequence;
        emitter.startSequence();
        for val in x do
          emitYamlValue(emitter, val);
        emitter.endSequence();
      }
      when YamlValueType.Scalar {
        const x = value: borrowed YamlScalar;
        var v = x.value:bytes, t = x.tag:bytes;
        emitter.emitScalar(v, t);
      }
      when YamlValueType.Null do emitter.emitScalar(b"~");
      when YamlValueType.Alias {
        const x = value: borrowed YamlAlias;
        var a = x._alias:bytes;
        emitter.emitAlias(a);
      }
      otherwise do throw new YamlParserError("cannot emit a yaml value of type: None");
    }
  }

  /* parse a yaml file at the given path and return an array of documents
  */
  proc parseYamlFile(path: string): [] owned YamlValue throws {
    var f = open(path, ioMode.r),
        reader = f.reader(locking = false);

    var parser = new owned LibYamlParser();
    var documents = parseUntilEvent(parser, EventType.StreamEnd, reader);

    return documents;
  }

  @chpldoc.nodoc
  iter parseUntilEvent(parser: borrowed LibYamlParser, event: EventType, reader: fileReader): owned YamlValue {
    while true {
      const (t, start, end) = try! parser.parseNext(reader);

      select t {
        when EventType.StreamStart {
          for e in parseUntilEvent(parser, EventType.StreamEnd, reader) do
            yield e;
        }
        when EventType.StreamEnd {
          assert(event == EventType.StreamEnd);
          break;
        }
        when EventType.DocumentStart {
          for e in parseUntilEvent(parser, EventType.DocumentEnd, reader) do
            yield e;
        }
        when EventType.DocumentEnd {
          assert(event == EventType.DocumentEnd);
          break;
        }
        when EventType.Alias {
          try! reader.seek(start:int..);
          yield new YamlScalar(try! reader.readString((end - start):int));
        }
        when EventType.Scalar {
          try! reader.seek(start:int..);
          yield new YamlScalar(try! reader.readString((end - start):int));
        }
        when EventType.MappingStart {
          var mapping = new YamlMapping(),
              nextKey = new owned YamlValue(),
              key = true;

          for e in parseUntilEvent(parser, EventType.MappingEnd, reader) {
            // TODO: is there a better way to do this without using unmanaged?
            // should this pattern be allowed for cpy:owned?
            var cpy: unmanaged = owned.release(e);
            if key {
              nextKey = owned.adopt(cpy);
              key = false;
            } else {
              mapping._add(nextKey, owned.adopt(cpy));
              key = true;
            }
          }
          yield mapping;
        }
        when EventType.MappingEnd {
          assert(event == EventType.MappingEnd);
          break;
        }
        when EventType.SequenceStart {
          var sequence = new YamlSequence();

          for e in parseUntilEvent(parser, EventType.SequenceEnd, reader) do
            sequence._push(e);
        }
        when EventType.SequenceEnd {
          assert(event == EventType.SequenceEnd);
          break;
        }
        otherwise do halt("unexpected event type: " + t:string);
      }
    }
  }

  class YamlValue {
    /* index into a YAML mapping by string */
    proc this(key: string): borrowed YamlValue throws {
      throw new YamlIndexError("indexing not supported");
    }

    /* index into a YAML mapping by YAML value */
    proc this(key: owned YamlValue): borrowed YamlValue throws {
      throw new YamlIndexError("indexing not supported");
    }

    /* index into a YAML sequence */
    proc this(idx: int): borrowed YamlValue throws {
      throw new YamlIndexError("indexing not supported");
    }

    /* get the size of a YAML sequence or mapping */
    proc size(): int {
      throw new YamlIndexError("size not supported");
    }

    /* get the tag of a YAML value */
    proc tag: string {
      return "";
    }

    proc valueType: YamlValueType {
      return YamlValueType.None;
    }

    proc asString(strict: bool = false): string throws {
      throw new YamlTypeError("cannot convert to string");
    }

    proc asBytes(strict: bool = false): bytes throws {
      throw new YamlTypeError("cannot convert to bytes");
    }

    proc asReal(strict: bool = false): real throws {
      throw new YamlTypeError("cannot convert to real");
    }

    proc asInt(strict: bool = false): int throws {
      throw new YamlTypeError("cannot convert to int");
    }

    proc asBool(strict: bool = false): bool throws {
      throw new YamlTypeError("cannot convert to bool");
    }

    proc asMapOf(type valType): map(string, valType) throws {
      throw new YamlTypeError("cannot convert to map");
    }

    proc asListOf(type t): list(t) throws {
      throw new YamlTypeError("cannot convert to list");
    }

  }

  class YamlMapping: YamlValue {
    @chpldoc.nodoc
    var _map: map(owned YamlValue, owned YamlValue);

    proc init() {
      this._map = new map(owned YamlValue, owned YamlValue);
    }

    @chpldoc.nodoc
    proc _add(in key: owned YamlValue, in val: owned YamlValue) {
      this._map.add(key, val);
    }

    override proc this(key: string): borrowed YamlValue throws {
      var yamlKey = new YamlScalar(key);
      return this[yamlKey: owned YamlValue];
    }

    override proc this(key: owned YamlValue): borrowed YamlValue throws {
      return this(key);
    }

    override proc size(): int {
      return this._map.size;
    }

    override proc valueType: YamlValueType {
      return YamlValueType.Mapping;
    }

    iter these(): (YamlValue, YamlValue) {
      var keys = this._map.keys();
      for k in keys do
        yield (k.borrow(), try! _map[k].borrow());
    }

    proc asMapOf(type kt, type vt): map(kt, vt) throws {
      var m = new map(kt, vt);

      for k in this._map.keys(){
        try {
          const key = k : kt;
          const val = this._map[k] : vt;
          m.add(key, val);
        } catch {
          throw new YamlTypeError("cannot convert to map of type: " + kt:string + " => " + vt:string);
        }
      }
    }
  }

  class YamlSequence: YamlValue {
    @chpldoc.nodoc
    var _seq: list(owned YamlValue);

    proc init() {
      this._seq = new list(owned YamlValue);
    }

    @chpldoc.nodoc
    proc _push(in val: owned YamlValue) {
      this._seq.append(val);
    }

    /* index into a YAML sequence */
    override proc this(idx: int): borrowed YamlValue throws {
      return this._seq(idx);
    }

    override proc size(): int {
      return this._map.size;
    }

    override proc valueType: YamlValueType {
      return YamlValueType.Sequence;
    }

    iter these(): YamlValue {
      for v in this._seq do
        yield v.borrow();
    }

    proc asListOf(type t): list(t) throws {
      var l = new list(t);

      for v in this._seq {
        try {
          const val = v: t;
          l.pushBack(v: t);
        } catch e {
          throw new YamlTypeError("cannot convert to list of type: " + t:string);
        }
      }

      return l;
    }
  }

  class YamlScalar: YamlValue {
    @chpldoc.nodoc
    var yamlType: YamlScalarType;
    @chpldoc.nodoc
    var userType: string = "";
    @chpldoc.nodoc
    var value: string;

    proc init(rawValue: string) {
      if rawValue.startsWith("!!") {
        const (typeTag, _, value) = rawValue.partition(" ");
        select typeTag {
          when "!!float" do this.yamlType = YamlScalarType.YamlFloat;
          when "!!int" do this.yamlType = YamlScalarType.YamlInt;
          when "!!str" do this.yamlType = YamlScalarType.YamlStr;
          when "!!bool" do this.yamlType = YamlScalarType.YamlBool;
          when "!!binary" do this.yamlType = YamlScalarType.YamlBinary;
          otherwise do halt("Unknown Yaml-type tag in YamlScalar.init: " + typeTag);
        }
        this.value = value;
      } else if rawValue.startsWith("!") {
        const (typeTag, _, value) = rawValue.partition(" ");
        this.yamlType = YamlScalarType.UserDefined;
        this.userType = typeTag[1..];
        this.value = value;
      } else {
        this.yamlType = YamlScalarType.Implicit;
        if rawValue.startsWith("|") then
          this.value = rawValue[1..].dedent().strip();
        else if rawValue.startsWith(">") then
          this.value = rawValue[1..].dedent().strip().replace("\n", " ");
        else
          this.value = rawValue;
      }
    }

    override proc valueType(): YamlValueType {
      return YamlValueType.Scalar;
    }

    override proc asString(strict: bool = false): string throws {
      if strict && (
        yamlType != YamlScalarType.YamlStr ||
        yamlType != YamlScalarType.Implicit
      ) then throw new YamlTypeError("cannot convert value to string");

      return value;
    }

    override proc asBytes(strict: bool = false): bytes throws {
      if strict && (
        yamlType != YamlScalarType.YamlBinary ||
        yamlType != YamlScalarType.YamlStr ||
        yamlType != YamlScalarType.Implicit
      ) then throw new YamlTypeError("cannot convert value to bytes");

      return value: bytes;
    }

    override proc asReal(strict: bool = false): real throws {
      if strict && (
        yamlType != YamlScalarType.YamlFloat ||
        yamlType != YamlScalarType.Implicit
      ) then throw new YamlTypeError("cannot convert value to real");

      try {
        return value : real;
      } catch {
        throw new YamlTypeError("cannot convert value to real");
      }
    }

    override proc asInt(strict: bool = false): int throws {
      if strict && (
          yamlType != YamlScalarType.YamlInt ||
          yamlType != YamlScalarType.Implicit
      ) then throw new YamlTypeError("cannot convert value to int");

      try {
        return value : int;
      } catch {
        throw new YamlTypeError("cannot convert value to int");
      }
    }

    override proc asBool(strict: bool = false): bool throws {
      if strict && (
          yamlType != YamlScalarType.YamlInt ||
          yamlType != YamlScalarType.Implicit
      ) then throw new YamlTypeError("cannot convert value to bool");

      if value == "Yes" then return true;
      if value == "No" then return false;
      throw new YamlTypeError("cannot convert to bool");
    }

    override proc tag: string {
      if this.yamlType == YamlScalarType.UserDefined then
        return "!" + this.userType;
      else
        return "";
    }
  }

  class YamlAlias: YamlValue {
    @chpldoc.nodoc
    var _alias: string;

    proc init(alias: string) {
      this._alias = alias;
    }

    override proc valueType(): YamlValueType {
      return YamlValueType.Alias;
    }

    override proc asString(strict: bool = false): string throws {
      return this._alias;
    }

    override proc asBytes(strict: bool = false): bytes throws {
      return this._alias : bytes;
    }
  }

  enum YamlValueType {
    Null,
    Sequence,
    Mapping,
    Alias,
    Scalar,
    None,
  }

  enum YamlScalarType {
  YamlFloat,
  YamlInt,
  YamlStr,
  YamlBool,
  YamlBinary,
  Implicit,
  UserDefined,
}

  class YamlIndexError: Error {
    proc init(msg: string) {
      super.init(msg);
    }
  }

  class YamlTypeError: Error {
    proc init(msg: string) {
      super.init(msg);
    }
  }

  module LibYamlUtils {
    private use CTypes;
    require "yaml.h", "-lyaml";

    config param YamlUtilsVerbose = false;

    import super.YamlSequenceStyle,
          super.YamlMappingStyle,
          super.YamlScalarStyle,
          super.YamlDocumentStyle;

    import super.YamlEmitterError,
          super.YamlParserError,
          super.YamlUnexpectedEventError;

    // ----------------------------------------------------
    // Event Types
    // ----------------------------------------------------

    extern const YAML_NO_EVENT: c_int;
    extern const YAML_STREAM_START_EVENT: c_int;
    extern const YAML_STREAM_END_EVENT: c_int;
    extern const YAML_DOCUMENT_START_EVENT: c_int;
    extern const YAML_DOCUMENT_END_EVENT: c_int;
    extern const YAML_ALIAS_EVENT: c_int;
    extern const YAML_SCALAR_EVENT: c_int;
    extern const YAML_SEQUENCE_START_EVENT: c_int;
    extern const YAML_SEQUENCE_END_EVENT: c_int;
    extern const YAML_MAPPING_START_EVENT: c_int;
    extern const YAML_MAPPING_END_EVENT: c_int;

    enum EventType {
      None,           // YAML_NO_EVENT
      StreamStart,    // YAML_STREAM_START_EVENT
      StreamEnd,      // YAML_STREAM_END_EVENT
      DocumentStart,  // YAML_DOCUMENT_START_EVENT
      DocumentEnd,    // YAML_DOCUMENT_END_EVENT
      Alias,          // YAML_ALIAS_EVENT
      Scalar,         // YAML_SCALAR_EVENT
      SequenceStart,  // YAML_SEQUENCE_START_EVENT
      SequenceEnd,    // YAML_SEQUENCE_END_EVENT
      MappingStart,   // YAML_MAPPING_START_EVENT
      MappingEnd      // YAML_MAPPING_END_EVENT
    }

    inline proc getEvent(flag: c_int): EventType {
      select flag {
        when YAML_NO_EVENT do return EventType.None;
        when YAML_STREAM_START_EVENT do return EventType.StreamStart;
        when YAML_STREAM_END_EVENT do return EventType.StreamEnd;
        when YAML_DOCUMENT_START_EVENT do return EventType.DocumentStart;
        when YAML_DOCUMENT_END_EVENT do return EventType.DocumentEnd;
        when YAML_ALIAS_EVENT do return EventType.Alias;
        when YAML_SCALAR_EVENT do return EventType.Scalar;
        when YAML_SEQUENCE_START_EVENT do return EventType.SequenceStart;
        when YAML_SEQUENCE_END_EVENT do return EventType.SequenceEnd;
        when YAML_MAPPING_START_EVENT do return EventType.MappingStart;
        when YAML_MAPPING_END_EVENT do return EventType.MappingEnd;
        otherwise halt("unknown libyaml event type");
      }
    }

    operator :(e: EventType, type t:string) {
      select e {
        when EventType.None do return "NO EVENT";
        when EventType.StreamStart do return "STREAM START";
        when EventType.StreamEnd do return "STREAM END";
        when EventType.DocumentStart do return "DOCUMENT START";
        when EventType.DocumentEnd do return "DOCUMENT END";
        when EventType.Alias do return "ALIAS";
        when EventType.Scalar do return "SCALAR";
        when EventType.SequenceStart do return "SEQUENCE START";
        when EventType.SequenceEnd do return "SEQUENCE END";
        when EventType.MappingStart do return "MAPPING START";
        when EventType.MappingEnd do return "MAPPING END";
        otherwise do return "UNKNOWN EVENT";
      }
    }

    // ----------------------------------------------------
    // structures
    // ----------------------------------------------------

    extern record yaml_parser_t { }
    extern record yaml_emitter_t { }
    extern record yaml_event_t {
      extern "type" var t: c_int;   // event type
      var start_mark: yaml_mark_t;  // start location in file
      var end_mark: yaml_mark_t;    // end location in file
    }
    extern record yaml_mark_t {
      extern "index" var idx: c_size_t; // byte index
      var line: c_size_t;
      var column: c_size_t;
    }
    extern record yaml_version_directive_t {
      var major: c_int;
      var minor: c_int;
    }
    extern record yaml_tag_directive_t {
      var handle: c_string;
      var prefix: c_string;
    }

    // ----------------------------------------------------
    // emitter API
    // ----------------------------------------------------

    private extern proc yaml_emitter_initialize(emitter: c_ptr(yaml_emitter_t)): c_int;
    private extern proc yaml_emitter_set_output_file(emitter: c_ptr(yaml_emitter_t), file: c_FILE): c_int;
    private extern proc yaml_emitter_set_canonical(emitter: c_ptr(yaml_emitter_t), isC: c_int);
    private extern proc yaml_emitter_set_unicode(emitter: c_ptr(yaml_emitter_t), isU: c_int);
    private extern proc yaml_emitter_emit(emitter: c_ptr(yaml_emitter_t), event: c_ptr(yaml_event_t)): c_int;
    private extern proc yaml_emitter_delete(emitter: c_ptr(yaml_emitter_t));
    private extern proc yaml_emitter_flush(emitter: c_ptr(yaml_emitter_t)): c_int;

    // ----------------------------------------------------
    // parser API
    // ----------------------------------------------------

    private extern proc yaml_parser_initialize(parser: c_ptr(yaml_parser_t)): c_int;
    private extern proc yaml_parser_set_input_file(parser: c_ptr(yaml_parser_t), file: c_FILE);
    private extern proc yaml_parser_parse(parser: c_ptr(yaml_parser_t), event: c_ptr(yaml_event_t)): c_int;
    private extern proc yaml_parser_delete(parser: c_ptr(yaml_parser_t)): c_int;

    // ----------------------------------------------------
    // event API
    // ----------------------------------------------------

    private extern proc yaml_event_delete(event: c_ptr(yaml_event_t));

    private extern proc yaml_stream_start_event_initialize(event: c_ptr(yaml_event_t), encoding: c_int): c_int;
    private extern proc yaml_stream_end_event_initialize(event: c_ptr(yaml_event_t)): c_int;

    private extern proc yaml_document_start_event_initialize(
                        event: c_ptr(yaml_event_t),
                        version: c_ptr(yaml_version_directive_t),
                        start: c_ptr(yaml_tag_directive_t),
                        end: c_ptr(yaml_tag_directive_t),
                        implicit: c_int
                      ): c_int;
    private extern proc yaml_document_end_event_initialize(event: c_ptr(yaml_event_t), implicit: c_int): c_int;

    private extern proc yaml_sequence_start_event_initialize(
                          event: c_ptr(yaml_event_t),
                          anchor: c_ptr(c_uchar),
                          tag: c_ptr(c_uchar),
                          implicit: c_int,
                          style: c_int
                        ): c_int;
    private extern proc yaml_sequence_end_event_initialize(event: c_ptr(yaml_event_t)): c_int;

    private extern proc yaml_mapping_start_event_initialize(
                          event: c_ptr(yaml_event_t),
                          anchor: c_ptr(c_uchar),
                          tag: c_ptr(c_uchar),
                          implicit: c_int,
                          style: c_int
                        ): c_int;
    private extern proc yaml_mapping_end_event_initialize(event: c_ptr(yaml_event_t)): c_int;

    private extern proc yaml_scalar_event_initialize(
                          event: c_ptr(yaml_event_t),
                          anchor: c_ptr(c_uchar),
                          tag: c_ptr(c_uchar),
                          value: c_ptr(c_uchar),
                          length: c_int,
                          plain_implicit: c_int,
                          quoted_implicit: c_int,
                          style: c_int
                        ): c_int;
    private extern proc yaml_alias_event_initialize(event: c_ptr(yaml_event_t), anchor: c_ptr(c_uchar)): c_int;

    // ----------------------------------------------------
    // encodings and styles
    // ----------------------------------------------------

    extern const YAML_ANY_ENCODING: c_int;
    extern const YAML_UTF8_ENCODING: c_int;
    extern const YAML_UTF16LE_ENCODING: c_int;
    extern const YAML_UTF16BE_ENCODING: c_int;

    extern const YAML_ANY_SEQUENCE_STYLE: c_int;
    extern const YAML_BLOCK_SEQUENCE_STYLE: c_int;
    extern const YAML_FLOW_SEQUENCE_STYLE: c_int;

    extern const YAML_ANY_MAPPING_STYLE: c_int;
    extern const YAML_BLOCK_MAPPING_STYLE: c_int;
    extern const YAML_FLOW_MAPPING_STYLE: c_int;

    extern const YAML_ANY_SCALAR_STYLE: c_int;
    extern const YAML_PLAIN_SCALAR_STYLE: c_int;
    extern const YAML_SINGLE_QUOTED_SCALAR_STYLE: c_int;
    extern const YAML_DOUBLE_QUOTED_SCALAR_STYLE: c_int;
    extern const YAML_LITERAL_SCALAR_STYLE: c_int;
    extern const YAML_FOLDED_SCALAR_STYLE: c_int;

    inline proc seqStyleToFlag(style: YamlSequenceStyle): c_int {
      select style {
        when YamlSequenceStyle.Any do return YAML_ANY_SEQUENCE_STYLE;
        when YamlSequenceStyle.Block do return YAML_BLOCK_SEQUENCE_STYLE;
        when YamlSequenceStyle.Flow do return YAML_FLOW_SEQUENCE_STYLE;
      }
      return YAML_ANY_SEQUENCE_STYLE;
    }

    inline proc mapStyleToFlag(style: YamlMappingStyle): c_int {
      select style {
        when YamlMappingStyle.Any do return YAML_ANY_MAPPING_STYLE;
        when YamlMappingStyle.Block do return YAML_BLOCK_MAPPING_STYLE;
        when YamlMappingStyle.Flow do return YAML_FLOW_MAPPING_STYLE;
      }
      return YAML_ANY_MAPPING_STYLE;
    }

    inline proc scalarStyleToFlag(style: YamlScalarStyle): c_int {
      select style {
        when YamlScalarStyle.Any do return YAML_ANY_SCALAR_STYLE;
        when YamlScalarStyle.Plain do return YAML_PLAIN_SCALAR_STYLE;
        when YamlScalarStyle.SingleQuoted do return YAML_SINGLE_QUOTED_SCALAR_STYLE;
        when YamlScalarStyle.DoubleQuoted do return YAML_DOUBLE_QUOTED_SCALAR_STYLE;
        when YamlScalarStyle.Literal do return YAML_LITERAL_SCALAR_STYLE;
        when YamlScalarStyle.Folded do return YAML_FOLDED_SCALAR_STYLE;
      }
      return YAML_ANY_SCALAR_STYLE;
    }

    // ----------------------------------------------------
    // Emitter wrapper
    // ----------------------------------------------------

    class LibYamlEmitter {
      var seqStyle: YamlSequenceStyle;
      var mapStyle: YamlMappingStyle;
      var scalarStyle: YamlScalarStyle;
      var documentStyle: YamlDocumentStyle;

      var emitter: yaml_emitter_t;
      var event: yaml_event_t;

      var file: c_FILE;
    }

    // -------- initialization --------

    proc LibYamlEmitter.init(
      sequences = YamlSequenceStyle.Any,
      mappings = YamlMappingStyle.Any,
      scalars = YamlScalarStyle.Any,
      documents = YamlDocumentStyle.Explicit
    ) {
      this.seqStyle = sequences;
      this.mapStyle = mappings;
      this.scalarStyle = scalars;
      this.documentStyle = documents;

      var emitter: yaml_emitter_t;
      var event: yaml_event_t;
      c_memset(c_ptrTo(emitter):c_void_ptr, 0, c_sizeof(yaml_emitter_t));
      c_memset(c_ptrTo(event):c_void_ptr, 0, c_sizeof(yaml_event_t));
      this.emitter = emitter;
      this.event = event;
    }

    // assumes 'closeContext' has already been called
    proc LibYamlEmitter.deinit() {
      yaml_event_delete(c_ptrTo(this.event));
      yaml_emitter_delete(c_ptrTo(this.emitter));
    }

    proc LibYamlEmitter.serialize(fw, serializer) throws {
      fw.write("LibYamlEmitter");
    }

    // -------- context management --------

    proc LibYamlEmitter.openContext(styleHint = YamlDocumentStyle.Default) throws {
      c_memset(c_ptrTo(emitter):c_void_ptr, 0, c_sizeof(yaml_emitter_t));
      if !yaml_emitter_initialize(c_ptrTo(this.emitter))
        then throw new YamlEmitterError("Failed to initialize emitter");

      this.file = tmpfile();

      yaml_emitter_set_output_file(c_ptrTo(this.emitter), this.file);
      yaml_emitter_set_canonical(c_ptrTo(this.emitter), 0);
      yaml_emitter_set_unicode(c_ptrTo(this.emitter), 1);

      this._startOutputStream();
      this.startDocument(styleHint);
    }

    proc LibYamlEmitter.closeContext(): bytes throws {
      this.endDocument();
      this._endOutputStream();
      yaml_emitter_delete(c_ptrTo(this.emitter));

      fseek(this.file, 0, SEEK_END);
      var size = ftell(this.file);
      fseek(this.file, 0, SEEK_SET);

      var buf = c_malloc(uint(8), size+1);
      fread(buf, 1, size, this.file);
      buf[size] = 0;

      const b = bytes.createCopyingBuffer(buf, size, size+1);
      c_free(buf);

      fclose(this.file);
      return b;
    }

    proc LibYamlEmitter.openFile(path: string, styleHint = YamlDocumentStyle.Default) throws {
      c_memset(c_ptrTo(emitter):c_void_ptr, 0, c_sizeof(yaml_emitter_t));
      if !yaml_emitter_initialize(c_ptrTo(this.emitter))
        then throw new YamlEmitterError("Failed to initialize emitter");

      var p = path, w = "w";
      var f = fopen(c_ptrTo(p), c_ptrTo(w));
      this.file = f;

      yaml_emitter_set_output_file(c_ptrTo(this.emitter), this.file);
      yaml_emitter_set_canonical(c_ptrTo(this.emitter), 0);
      yaml_emitter_set_unicode(c_ptrTo(this.emitter), 1);
    }

    // -------- event emission --------

    proc LibYamlEmitter.emitEvent(param name: string) throws {
      if !yaml_emitter_emit(c_ptrTo(this.emitter), c_ptrTo(this.event)) then
        throw new YamlEmitterError("Failed to emit a libyaml '" + name + "' event.");
    }

    proc LibYamlEmitter.startSequence(ref tag: bytes = b"", styleHint = YamlSequenceStyle.Default) throws {
      if !yaml_sequence_start_event_initialize(
        c_ptrTo(this.event),
        nil, // TODO: anchor support
        if tag.numBytes > 0 then c_ptrTo(tag) else nil,
        (if tag.numBytes > 0 then 0 else 1): c_int,
        this.chooseSeqStyle(styleHint)
      ) then throw new YamlEmitterError("Failed to initialize 'sequence start' event.");

      this.emitEvent("sequence start");
    }

    proc LibYamlEmitter.endSequence() throws {
      if !yaml_sequence_end_event_initialize(c_ptrTo(this.event))
        then throw new YamlEmitterError("Failed to initialize 'sequence end' event.");

      this.emitEvent("sequence end");
    }

    proc LibYamlEmitter.startMapping(ref tag: bytes = b"", styleHint = YamlMappingStyle.Default) throws {
      if !yaml_mapping_start_event_initialize(
        c_ptrTo(this.event),
        nil, // TODO: anchor support
        if tag.numBytes > 0 then c_ptrTo(tag) else nil,
        (if tag.numBytes > 0 then 0 else 1): c_int,
        this.chooseMapStyle(styleHint)
      ) then throw new YamlEmitterError("Failed to initialize 'mapping start' event.");

      this.emitEvent("mapping start");
    }

    proc LibYamlEmitter.endMapping() throws {
      if !yaml_mapping_end_event_initialize(c_ptrTo(this.event))
        then throw new YamlEmitterError("Failed to initialize 'mapping end' event.");

      this.emitEvent("mapping end");
    }

    proc LibYamlEmitter.emitScalar(
      value: bytes,
      ref tag: bytes = b"",
      styleHint = YamlScalarStyle.Default
    ) throws {
      var localvalue = value;

      if !yaml_scalar_event_initialize(
        c_ptrTo(this.event),
        nil, // TODO: anchor support
        if tag.numBytes > 0 then c_ptrTo(tag) else nil,
        c_ptrTo(localvalue),
        value.numBytes: c_int,
        (if tag.numBytes > 0 then 0 else 1): c_int,
        (if tag.numBytes > 0 then 0 else 1): c_int,
        this.chooseScalarStyle(styleHint)
      ) then throw new YamlEmitterError("Failed to initialize 'scalar' event.");

      this.emitEvent("scalar");
    }

    proc LibYamlEmitter.emitAlias(ref value: bytes) throws {
      if !yaml_alias_event_initialize(c_ptrTo(this.event), c_ptrTo(value))
        then throw new YamlEmitterError("Failed to initialize 'alias' event.");

      this.emitEvent("alias");
    }

    proc LibYamlEmitter.startDocument(styleHint = YamlDocumentStyle.Default) throws {
      if !yaml_document_start_event_initialize(
        c_ptrTo(this.event),
        nil, nil, nil,
        this.chooseDocumentStyle(styleHint)
      ) then throw new YamlEmitterError("Failed to initialize 'document start' event");

      this.emitEvent("start document");
    }

    proc LibYamlEmitter.endDocument(styleHint = YamlDocumentStyle.Default) throws {
      if !yaml_document_end_event_initialize(
        c_ptrTo(this.event),
        this.chooseDocumentStyle(styleHint)
      ) then throw new YamlEmitterError("Failed to initialize document end event");

      this.emitEvent("end document");
    }

    proc LibYamlEmitter._startOutputStream() throws {
      // TODO: make the encoding configurable
      if !yaml_stream_start_event_initialize(c_ptrTo(this.event), YAML_UTF8_ENCODING)
        then throw new YamlEmitterError("Failed to initialize 'stream start' event");

      this.emitEvent("stream start");
    }

    proc LibYamlEmitter._endOutputStream() throws {
      if !yaml_stream_end_event_initialize(c_ptrTo(this.event))
        then throw new YamlEmitterError("Failed to initialize 'stream end' event");

      this.emitEvent("stream end");
    }

    // -------- helpers --------

    proc LibYamlEmitter.chooseSeqStyle(styleHint: YamlSequenceStyle): c_int {
      if styleHint == YamlSequenceStyle.Default
        then return seqStyleToFlag(this.seqStyle);
        else return seqStyleToFlag(styleHint);
    }

    proc LibYamlEmitter.chooseMapStyle(styleHint: YamlMappingStyle): c_int {
      if styleHint == YamlMappingStyle.Default
        then return mapStyleToFlag(this.mapStyle);
        else return mapStyleToFlag(styleHint);
    }

    proc LibYamlEmitter.chooseScalarStyle(styleHint: YamlScalarStyle): c_int {
      if styleHint == YamlScalarStyle.Default
        then return scalarStyleToFlag(this.scalarStyle);
        else return scalarStyleToFlag(styleHint);
    }

    proc LibYamlEmitter.chooseDocumentStyle(hint: YamlDocumentStyle): c_int {
      if hint == YamlDocumentStyle.Default {
        select this.documentStyle {
          when YamlDocumentStyle.Explicit do return 0;
          when YamlDocumentStyle.Implicit do return 1;
        }
      } else {
        select hint {
          when YamlDocumentStyle.Explicit do return 0;
          when YamlDocumentStyle.Implicit do return 1;
        }
      }
      return 1;
    }

    // used to serialize simple types outside the context of a sequence or mapping
    proc LibYamlEmitter.yamlStringifyPrimitive(const val: ?t, processStrings:bool): bytes {
      if isBoolType(t) {
        return if val then b"Yes" else b"No";
      } else if t == string || t == bytes {
        if processStrings {
          select this.scalarStyle {
            when YamlScalarStyle.Plain do return val: bytes;
            when YamlScalarStyle.SingleQuoted do return b"'" + val: bytes + b"'";
            when YamlScalarStyle.DoubleQuoted do return b"\"" + val: bytes + b"\"";
            when YamlScalarStyle.Literal do return b"| " + val: bytes;
            when YamlScalarStyle.Folded {
              var left = val.size,
                  lineStart = 0,
                  folded = b"> ";
              while left > 0 {
                const next = val[lineStart..min(lineStart + 80, val.size)]: bytes + b"\n";
                left -= next.size;
                folded += next;
              }
              return folded;
            }
            otherwise return val: bytes;
          }
        } else {
          return val: bytes;
        }
      } else if (isClassType(t) && val == nil) || isNothingType(t) {
        return b"~";
      } else {
        return stringify(val);
      }
    }

    // -------- meta --------
    @chpldoc.nodoc
    proc LibYamlEmitter.serialize(fw, serializer) throws {
      fw.write("---LimYamlEmitter---");
    }

    @chpldoc.nodoc
    proc LibYamlEmitter.writeThis(fw) throws {
      fw.write("---LimYamlEmitter---");
    }

    // ----------------------------------------------------
    // Parser Wrapper
    // ----------------------------------------------------

    class LibYamlParser {
      var parser: yaml_parser_t;
      var event: yaml_event_t;

      /* needed for list and map parsing:
        For these types, the deserializer needs to throw a `BadFormatError`
        to indicate that no more elements are available.
        In practice, this involves expecting a Scalar (or other event),
        but getting a Sequence/MappingEnd event instead. As such, the
        Sequence/MappingEnd event needs to be stored so that it can be
        consumed by the deserializer later (when list/map calls it's closing
        method on the deserializer).
      */
      var cachedEvent: (EventType, uint, uint);

      var fileIsAttached = false; // whether the current file is attached to the parser
      var f: c_FILE; // pointer to a file supplied by the fileWriter
    }

    // -------- initialization --------
    proc LibYamlParser.init() {
      var p: yaml_parser_t;
      var e: yaml_event_t;
      c_memset(c_ptrTo(p):c_void_ptr, 0, c_sizeof(yaml_parser_t));
      c_memset(c_ptrTo(e):c_void_ptr, 0, c_sizeof(yaml_event_t));
      this.parser = p;
      this.event = e;
      this.cachedEvent = (EventType.None, 0, 0);
      this.fileIsAttached = false;
    }

    proc LibYamlParser.deinit() {
      yaml_parser_delete(c_ptrTo(this.parser));
      yaml_event_delete(c_ptrTo(this.event));
    }

    proc LibYamlParser.attachFile(fr) throws {
      // get a file pointer from the fileReader
      const (hasFp, fp) = fr._getFp();
      if !hasFp
        then throw new YamlParserError("Cannot parse yaml from a memory file.");
        else this.f = fp;

      // set the file offset
      fseek(this.f, fr.offset(), SEEK_SET);

      // attach the file to the parser
      if !yaml_parser_initialize(c_ptrTo(this.parser))
        then throw new YamlParserError("Failed to initialize libyaml parser.");

      yaml_parser_set_input_file(c_ptrTo(this.parser), this.f);
      this.fileIsAttached = true;

      this.expectEvent(fr, EventType.StreamStart);
    }

    proc LibYamlParser.finishFile(fr) throws {
      this.expectEvent(fr, EventType.StreamEnd);
      this.fileIsAttached = false;
      this.cachedEvent = (EventType.None, 0, 0);
    }

    // -------- parsing --------

    proc LibYamlParser.expectEvent(fr, e: EventType): 2*uint throws {
      if YamlUtilsVerbose then writeln("\t\texpecting event: ", e, "...");

      const (found, start, end) = this.parseNext(fr);
      if e != found {
        // store the event if it was unexpected
        if YamlUtilsVerbose then writeln("\t\tunexpected event (caching): ", found, "...");
        this.cachedEvent = (found, start, end);
        throw new YamlUnexpectedEventError(e:string, found:string);
      }

      // clean up if this is the end of the document
      if e == EventType.DocumentEnd then this.finishFile(fr);

      return (start, end);
    }

    proc LibYamlParser.peekFor(fr, e: EventType): bool throws {
      if YamlUtilsVerbose then writeln("\t\tpeeking event...");
      const (found, start, end) = this.parseNext(fr);

      // store the event
      this.cachedEvent = (found, start, end);

      return e == found;
    }

    proc LibYamlParser.parseNext(fr): (EventType, uint, uint) throws {
      if !this.fileIsAttached then this.attachFile(fr);

      // return a cached event if there is on
      if this.cachedEvent[0] != EventType.None {
        const (t, start, end) = this.cachedEvent;
        this.cachedEvent = (EventType.None, 0, 0);
        return (t, start, end);
      }

      // otherwise parse the next event
      if !yaml_parser_parse(c_ptrTo(this.parser), c_ptrTo(this.event)) then
        throw new YamlParserError("Failed to parse next libyaml event.");

      return (
        getEvent(event.t),
        this.event.start_mark.idx,
        this.event.end_mark.idx
      );
    }

    // -------- meta --------
    @chpldoc.nodoc
    proc LibYamlParser.serialize(fw, serializer) throws {
      fw.write("---LimYamlParser---");
    }

    @chpldoc.nodoc
    proc LibYamlParser.writeThis(fw) throws {
      fw.write("---LimYamlParser---");
    }

    // ----------------------------------------------------
    // Filesystem stuff
    // ----------------------------------------------------

    private extern proc fopen(filename: c_ptr(c_uchar), mode: c_ptr(c_uchar)): c_FILE;
    private extern proc fclose(file: c_FILE): c_int;
    private extern proc fseek(file: c_FILE, offset: c_long, origin: c_int): c_int;
    private extern proc tmpfile(): c_FILE;
    private extern proc fread(ptr: c_ptr(c_uchar), size: c_size_t, nmemb: c_size_t, stream: c_FILE): c_size_t;
    private extern proc ftell(stream: c_FILE): c_long;
    private extern proc fflush(stream: c_FILE): c_int;
    private extern proc fgets(s: c_ptr(c_uchar), size: c_int, stream: c_FILE): c_ptr(c_uchar);
    @chpldoc.nodoc
    extern const SEEK_SET: c_int;
    @chpldoc.nodoc
    extern const SEEK_END: c_int;

    // ----------------------------------------------------
    // Context Helper
    // ----------------------------------------------------

    @chpldoc.nodoc
    class ContextCounter {
      // used to keep track of the overall context depth
      //  i.e., how many mappings, sequences, etc. are currently open
      var count = 0;
      // use to keep track of the current position in a class hierarchy
      //  when child classes are being (de)serialized, the parent classes
      //  deserializing intializer is also called; however, only one new
      //  mapping should be opened for each class. This counter is used
      //  to prevent multiple mappings from being opened.
      var classDepth = 0;

      proc enter() do
        this.count += 1;

      proc leave() do
        this.count -= 1;

      proc isBase: bool do
        return this.count == 0;

      proc enterClass() do
        this.classDepth += 1;

      proc leaveClass() do
        this.classDepth -= 1;

      proc inSuperClass: bool do
        return this.classDepth > 1;
    }
  }
}
