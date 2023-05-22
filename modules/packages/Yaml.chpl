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

Support for doing IO with YAML files.

Includes a Yaml Serializer/Deserializer as well as an abstract class
representation of a YAML file.

*/

module Yaml {
  include module LibYamlUtils;
  private use LibYamlUtils;

  // ----------------------------------------------------
  // Serializer/Deserializer Definitions
  // ----------------------------------------------------

  type _writeType = fileWriter(serializerType=YamlSerializer, ?);
  type _readType = fileReader(deserializerType=YamlDeserializer, ?);

  record YamlSerializer {
    @chpldoc.nodoc
    var emitter: shared LibYamlEmitter;

    @chpldoc.nodoc
    var context: shared ContextCounter;

    proc init (
      seqStyle: YamlSequenceStyle = YamlSequenceStyle.Any,
      mapStyle: YamlMappingStyle = YamlMappingStyle.Any,
      scalarStyle: YamlScalarStyle = YamlScalarStyle.Any,
      documentStyle: YamlDocumentStyle = YamlDocumentStyle.Explicit
    ) {
      emitter = new LibYamlEmitter(seqStyle, mapStyle, scalarStyle, documentStyle);
      context = new ContextCounter();
    }

    @chpldoc.nodoc
    proc init(e: shared LibYamlEmitter, c: shared ContextCounter) {
      emitter = e;
      context = c;
    }
  }

  record YamlDeserializer {
    @chpldoc.nodoc
    var parser: shared LibYamlParser;

    @chpldoc.nodoc
    var context: shared ContextCounter;

    var strictTypeChecking: bool;

    proc init(strictTypeChecking = false) {
      parser = new LibYamlParser();
      context = new ContextCounter();
      strictTypeChecking = strictTypeChecking;
    }

    @chpldoc.nodoc
    proc init(p: shared LibYamlParser, c: shared ContextCounter, stc: bool) {
      parser = p;
      context = c;
      strictTypeChecking = stc;
    }
  }

  // ----------------------------------------------------
  // Serializer implementation
  // ----------------------------------------------------

  // ------- fileWriter API -------

  proc YamlSerializer.serializeValue(writer: _writeType, const val: t?) throws {
    if context.isBase && _hasSimpleYamlType(val) {
      // simply translate the value to it's YAML representation
      // e.g., true => Yes, nil => ~, 5.5 => 5.5, etc.
      writer.writeBytes(this.emitter.yamlStringifySimple(val, true));
    } else {
      // translate the value within the current emitter context
      if _hasSimpleYamlType(val) {
        var x = this.emitter.yamlStringifySimple(val, false);
        this.emitter.emitScalar(x);
      } else if isClassType(t) {
        t!.serialize(writer, new YamlSerializer(this.emitter, this.context));
      } else {
        t.serialize(writer, new YamlSerializer(this.emitter, this.context));
      }
    }
  }

  // ------- serialize() API -------

  proc YamlSerializer.startClass(writer: _writeType, name: string, size: int) throws {
    this.context.enterClass();
    this._startMapping(writer, name);
  }

  proc YamlSerializer.endClass(writer: _writeType) throws {
    this._endMapping(writer);
    this.context.leaveClass();
  }

  proc YamlSerializer.startRecord(writer: _writeType) throws {
    this._startMapping(writer);
  }

  proc YamlSerializer.endRecord(writer: _writeType) throws {
    this._endMapping(writer);
  }

  proc YamlSerializer.serializeField(writer: _writeType, key: string, const val: ?t) throws {
    if key.size > 0 then {
      var kb = key: bytes;
      this.emitter.emitScalar(kb);
    }
    this.serializeValue(writer, val);
  }

  proc YamlSerializer.startTuple(writer: _writeType, size: int) throws {
    this._startSequence(writer);
  }

  proc YamlSerializer.endTuple(writer: _writeType) throws {
    this._endSequence(writer);
  }

  proc YamlSerializer.startArray(writer: _writeType, size: uint = 0) throws {}

  proc YamlSerializer.endArray(writer: _writeType) throws {}

  proc YamlSerializer.startArrayDim(writer: _writeType, len: uint) throws {
    this._startSequence(writer);
  }

  proc YamlSerializer.endArrayDim(writer: _writeType) throws {
    this._endSequence(writer);
  }

  proc YamlSerializer.writeArrayElement(writer: _writeType, const elt: ?t) throws {
    this.serializeValue(writer, elt);
  }

  proc YamlSerializer.startMap(writer: _writeType, size: uint = 0) throws {
    this._startMapping(writer);
  }

  proc YamlSerializer.endMap(writer: _writeType) throws {
    this._endMapping(writer);
  }

  proc YamlSerializer.writeKey(writer: _writeType, const key: ?t) throws {
    this.serializeValue(writer, key);
  }

  proc YamlSerializer.writeValue(writer: _writeType, const val: ?t) throws {
    this.serializeValue(writer, val);
  }

  // ------- internal -------

  @chpldoc.nodoc
  proc YamlSerializer._startMapping(writer: _writeType, name: string = "") throws {
    if this.context.isBase then this.emitter.openContext();
    this.context.enter();

    if !this.context.inSuperClass {
      var nb = if name.size > 0 then b"!" + name: bytes else b"";
      this.emitter.startMapping(nb);
    }
  }

  @chpldoc.nodoc
  proc YamlSerializer._endMapping(writer: _writeType) throws {
    this.context.leave();
    if !this.context.inSuperClass then this.emitter.endMapping();
    if this.context.isBase then writer.writeBytes(this.emitter.closeContext());
  }

  @chpldoc.nodoc
  proc YamlSerializer._startSequence(writer: _writeType, name: string = "") throws {
    if this.context.isBase then this.emitter.openContext();
    this.context.enter();

    var nb = if name.size > 0 then b"!" + name: bytes else b"";
    this.emitter.startSequence(nb);
  }

  @chpldoc.nodoc
  proc YamlSerializer._endSequence(writer: _writeType) throws {
    this.context.leave();
    this.emitter.endSequence();
    if this.context.isBase then writer.writeBytes(this.emitter.closeContext());
  }

  @chpldoc.nodoc
  proc _hasSimpleYamlType(val: ?t): bool {
    return val == nil || isBoolType(t) || isNumericType(t) ||
      isNothingType(t) || isRangeType(t) || isDomainType(t) ||
      t == string || t == bytes;
  }

  // ----------------------------------------------------
  // Deserializer implementation
  // ----------------------------------------------------

  // ------- fileReader API -------

  proc YamlDeserializer.deserializeValue(reader: _readType, ref val: ?t) throws {

  }

  proc YamlDeserializer.deserializeType(reader: _readType, type t): t throws {

  }

  // ------- deserialize() API -------


  // ------- internal -------



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

    override proc message(): string {
      return super.message();
    }
  }

  class YamlParserError: Error {
    proc init(msg: string) {
      super.init(msg);
    }

    override proc message(): string {
      return super.message();
    }
  }

  class YamlUnexpectedEventError: YamlParserError {
    proc init(expected: string, actual: string) {
      super.init("Expected event: '" + expected + "', but got '" + actual + "'.");
    }

    override proc message(): string {
      return super.message();
    }
  }

  proc doSomething(yss: YamlSequenceStyle) {
    return seqStyleToFlag(yss);
  }

}
