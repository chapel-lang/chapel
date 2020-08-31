/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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
    Support for protocol buffers binary wire format.

    This module provides a Chapel implementation for protocol buffers binary
    `wire format <https://developers.google.com/protocol-buffers/docs/encoding/>`_
    enoding algorithms. It has functions to support serialization and deserialization
    of protocol buffer messages.
 */
module ProtobufProtocolSupport {

  public use WireEncoding;
  public use Fields;
  public use RepeatedFields;

  pragma "no doc"
  module WireEncoding {
    /*
      This module contains the implementation of encoding/decoding algorithms
      for basic proto types. The other module functions are written over
      these implementations.
    */

    use IO;

    type writingChannel = channel(true,iokind.little,false);
    type readingChannel = channel(false,iokind.little,false);

    // wireTypes
    const varint = 0;
    const lengthDelimited = 2;
    const fixed64Type = 1;
    const fixed32Type = 5;

    proc unsignedVarintAppend(val:uint, ch: writingChannel) throws {
      if val == 0 {
        var zero: uint(8);
        ch.write(zero);
        return;
      }
    
      var newVal = val;
      var shiftVal: uint;
      while newVal != 0 {
        shiftVal = newVal >> 7;
        var k = if shiftVal != 0 then 0x80 else 0x00;
        var newByte = (newVal & 0x7F | k):uint(8);
        ch.write(newByte);
        newVal = shiftVal;
      }
    }

    proc unsignedVarintConsume(ch: readingChannel): (uint, int) throws {
      var shift = 0;
      var val:uint;
      var len = 0;
      var s:uint(8);
      while true {
        if !ch.read(s) then return (val, -1);
        val = val + ((s & 0x7F): uint << shift);
        shift = shift + 7;
        len = len + 1;
        if (s & 0x80) == 0 then break;
      }
      return (val, len);
    }

    proc tagConsume(ch: readingChannel) throws {
      var (tag, tlen) = unsignedVarintConsume(ch);
      if tlen == -1 then return (-1, -1);
      var wireType = (tag & 0x7): int;
      var fieldNumber = (tag >> 3): int;
      return (fieldNumber, wireType);
    }

    proc tagAppend(fieldNumber: int, wireType: int, ch: writingChannel) throws {
      unsignedVarintAppend(((fieldNumber << 3) | wireType):uint, ch);
    }
    
    proc uint64AppendBase(val: uint(64), ch: writingChannel) throws {
      var uintVal = val:uint;
      unsignedVarintAppend(uintVal, ch);
    }

    proc uint64ConsumeBase(ch: readingChannel): uint(64) throws {
      var (val, len) = unsignedVarintConsume(ch);
      return val:uint(64);
    }

    proc uint32AppendBase(val: uint(32), ch: writingChannel) throws {
      var uintVal = val:uint;
      unsignedVarintAppend(uintVal, ch);
    }

    proc uint32ConsumeBase(ch: readingChannel): uint(32) throws {
      var (val, len) = unsignedVarintConsume(ch);
      return val:uint(32);
    }

    proc int64AppendBase(val: int(64), ch: writingChannel) throws {
      var uintVal = val:uint;
      unsignedVarintAppend(uintVal, ch);
    }

    proc int64ConsumeBase(ch: readingChannel): int throws {
      var (val, len) = unsignedVarintConsume(ch);
      return val:int(64);
    }

    proc int32AppendBase(val: int(32), ch: writingChannel) throws {
      var uintVal = val:uint;
      unsignedVarintAppend(uintVal, ch);
    }

    proc int32ConsumeBase(ch: readingChannel): int(32) throws {
      var (val, len) = unsignedVarintConsume(ch);
      return val:int(32);
    }

    proc boolAppendBase(val: bool, ch: writingChannel) throws {
      var uintVal = val:uint;
      unsignedVarintAppend(uintVal, ch);
    }

    proc boolConsumeBase(ch: readingChannel): bool throws {
      var (val, len) = unsignedVarintConsume(ch);
      return val:bool;
    }

    proc sint64AppendBase(val: int(64), ch: writingChannel) throws {
      var uintVal = (val << 1):uint ^ (val >> 63):uint;
      unsignedVarintAppend(uintVal, ch);
    }

    proc sint64ConsumeBase(ch: readingChannel): int(64) throws {
      const (val, len) = unsignedVarintConsume(ch);
      return (val >> 1):int(64) ^ (val):int(64) << 63 >> 63;
    }

    proc sint32AppendBase(val: int(64), ch: writingChannel) throws {
      var uintVal = (val << 1):uint ^ (val >> 31):uint;
      unsignedVarintAppend(uintVal, ch);
    }

    proc sint32ConsumeBase(ch: readingChannel): int(32) throws {
      const (val, len) = unsignedVarintConsume(ch);
      return (val >> 1):int(32) ^ (val):int(32) << 31 >> 31;
    }

    proc bytesAppendBase(val: bytes, ch: writingChannel) throws {
      unsignedVarintAppend((val.size):uint, ch);
      ch.write(val);
    }

    proc bytesConsumeBase(ch: readingChannel): bytes throws {
      const (byteLen, len) = unsignedVarintConsume(ch);
      var s:bytes;
      ch.readbytes(s, byteLen:int);
      return s;
    }

    proc stringAppendBase(val: string, ch: writingChannel) throws {
      bytesAppendBase(val.encode(), ch);
    }

    proc stringConsumeBase(ch: readingChannel): string throws {
      return bytesConsumeBase(ch).decode();
    }

    proc fixed32AppendBase(val: uint(32), ch: writingChannel) throws {
      ch.write(val);
    }

    proc fixed32ConsumeBase(ch: readingChannel): uint(32) throws {
      var val: uint(32);
      ch.read(val);
      return val;
    }

    proc fixed64AppendBase(val: uint(64), ch: writingChannel) throws {
      ch.write(val);
    }

    proc fixed64ConsumeBase(ch: readingChannel): uint(64) throws {
      var val: uint(64);
      ch.read(val);
      return val;
    }

    proc floatAppendBase(val: real(32), ch: writingChannel) throws {
      var a = val;
      var b: uint(32);
      c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
      fixed32AppendBase(b, ch);
    }
    
    proc floatConsumeBase(ch: readingChannel): real(32) throws {
      var a = fixed32ConsumeBase(ch);
      var b: real(32);
      c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
      return b;
    }

    proc doubleAppendBase(val: real(64), ch: writingChannel) throws {
      var a = val;
      var b: uint(64);
      c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
      fixed64AppendBase(b, ch);
    }
    
    proc doubleConsumeBase(ch: readingChannel): real(64) throws {
      var a = fixed64ConsumeBase(ch);
      var b: real(64);
      c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
      return b;
    }

    proc sfixed64AppendBase(val: int(64), ch: writingChannel) throws {
      var a = val;
      var b: uint(64);
      c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
      fixed64AppendBase(b, ch);
    }

    proc sfixed64ConsumeBase(ch: readingChannel): int(64) throws {
      var a = fixed64ConsumeBase(ch);
      var b: int(64);
      c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
      return b;
    }

    proc sfixed32AppendBase(val: int(32), ch: writingChannel) throws {
      var a = val;
      var b: uint(32);
      c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
      fixed32AppendBase(b, ch);
    }

    proc sfixed32ConsumeBase(ch: readingChannel): int(32) throws {
      var a = fixed32ConsumeBase(ch);
      var b: int(32);
      c_memcpy(c_ptrTo(b), c_ptrTo(a), c_sizeof(b.type));
      return b;
    }

    proc enumAppendBase(val: uint(64), ch: writingChannel) throws {
      unsignedVarintAppend(val, ch);
    }

    proc enumConsumeBase(ch: readingChannel): uint(64) throws {
      var (val, len) = unsignedVarintConsume(ch);
      return val;
    }

    proc messageAppendBase(val, ch:writingChannel) throws {
     var initialOffset = ch.offset();
     ch.mark();
     val._serialize(ch);
     var currentOffset = ch.offset();
     ch.revert();
     unsignedVarintAppend((currentOffset-initialOffset):uint, ch);
     val._serialize(ch);
   }

   proc messageConsumeBase(ch:readingChannel, ref messageObj, memWriter:writingChannel,
                           memReader:readingChannel) throws {
     var s: bytes;
     var (payloadLength, _) = unsignedVarintConsume(ch);
     ch.readbytes(s, payloadLength:int);
     memWriter.write(s);
     memWriter.close();
     messageObj._deserialize(memReader);
   }

    proc serializeHelper(ref message, ch) throws {
      ch.lock();
      defer { ch.unlock(); }
      var binCh: channel(writing=true, kind=iokind.little, locking=false) = ch;
      message._serialize(binCh);
    }

    proc deserializeHelper(ref message, ch) throws {
      ch.lock();
      defer { ch.unlock(); }
      var binCh: channel(writing=false, kind=iokind.little, locking=false) = ch;
      message._deserialize(binCh);
    }

  }
  
  pragma "no doc"
  module Fields {
    /*
      This module contains functions for encoding/decoding simple non-repetitive protobuf
      fields. A tag (generated using fieldNumber and wireType) is appended to the encoded
      value of the field.
    */

    use IO;
    use super.WireEncoding;

    proc uint64Append(val: uint(64), fieldNumber: int, ch: writingChannel) throws {
      if val == 0 then return;

      tagAppend(fieldNumber, varint, ch);
      uint64AppendBase(val, ch);
    }

    proc uint64Consume(ch: readingChannel): uint(64) throws {
      return uint64ConsumeBase(ch);
    }

    proc uint32Append(val: uint(32), fieldNumber: int, ch: writingChannel) throws {
      if val == 0 then return;

      tagAppend(fieldNumber, varint, ch);
      uint32AppendBase(val, ch);
    }

    proc uint32Consume(ch: readingChannel): uint(32) throws {
      return uint32ConsumeBase(ch);
    }
    
    proc int64Append(val: int(64), fieldNumber: int, ch: writingChannel) throws {
      if val == 0 then return;

      tagAppend(fieldNumber, varint, ch);
      int64AppendBase(val, ch);
    }

    proc int64Consume(ch: readingChannel): int throws {
      return int64ConsumeBase(ch);
    }

    proc int32Append(val: int(32), fieldNumber: int, ch: writingChannel) throws {
      if val == 0 then return;

      tagAppend(fieldNumber, varint, ch);
      int32AppendBase(val, ch);
    }

    proc int32Consume(ch: readingChannel): int(32) throws {
      return int32ConsumeBase(ch);
    }

    proc boolAppend(val: bool, fieldNumber: int, ch: writingChannel) throws {
      if val == false then return;

      tagAppend(fieldNumber, varint, ch);
      boolAppendBase(val, ch);
    }

    proc boolConsume(ch: readingChannel): bool throws {
      return boolConsumeBase(ch);
    }

    proc sint64Append(val: int(64), fieldNumber: int, ch: writingChannel) throws {
      if val == 0 then return;

      tagAppend(fieldNumber, varint, ch);
      sint64AppendBase(val, ch);
    }

    proc sint64Consume(ch: readingChannel): int(64) throws {
      return sint64ConsumeBase(ch);
    }

    proc sint32Append(val: int(64), fieldNumber: int, ch: writingChannel) throws {
      if val == 0 then return;

      tagAppend(fieldNumber, varint, ch);
      sint32AppendBase(val, ch);
    }

    proc sint32Consume(ch: readingChannel): int(32) throws {
      return sint32ConsumeBase(ch);
    }

    proc bytesAppend(val: bytes, fieldNumber: int, ch: writingChannel) throws {
      if val == b"" then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      bytesAppendBase(val, ch);
    }

    proc bytesConsume(ch: readingChannel): bytes throws {
      return bytesConsumeBase(ch);
    }

    proc stringAppend(val: string, fieldNumber: int, ch: writingChannel) throws {
      if val == "" then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      stringAppendBase(val, ch);
    }

    proc stringConsume(ch: readingChannel): string throws {
      return stringConsumeBase(ch);
    }

    proc fixed32Append(val: uint(32), fieldNumber: int, ch: writingChannel) throws {
      if val == 0 then return;

      tagAppend(fieldNumber, fixed32Type, ch);
      fixed32AppendBase(val, ch);
    }

    proc fixed32Consume(ch: readingChannel): uint(32) throws {
      return fixed32ConsumeBase(ch);
    }

    proc fixed64Append(val: uint(64), fieldNumber: int, ch: writingChannel) throws {
      if val == 0 then return;

      tagAppend(fieldNumber, fixed64Type, ch);
      fixed64AppendBase(val, ch);
    }

    proc fixed64Consume(ch: readingChannel): uint(64) throws {
      return fixed64ConsumeBase(ch);
    }

    proc floatAppend(val: real(32), fieldNumber: int, ch: writingChannel) throws {
      if val == 0.0 then return;

      tagAppend(fieldNumber, fixed32Type, ch);
      floatAppendBase(val, ch);
    }
    
    proc floatConsume(ch: readingChannel): real(32) throws {
      return floatConsumeBase(ch);
    }

    proc doubleAppend(val: real(64), fieldNumber: int, ch: writingChannel) throws {
      if val == 0.0 then return;

      tagAppend(fieldNumber, fixed64Type, ch);
      doubleAppendBase(val, ch);
    }
    
    proc doubleConsume(ch: readingChannel): real(64) throws {
      return doubleConsumeBase(ch);
    }

    proc sfixed64Append(val: int(64), fieldNumber: int, ch: writingChannel) throws {
      if val == 0 then return;

      tagAppend(fieldNumber, fixed64Type, ch);
      sfixed64AppendBase(val, ch);
    }

    proc sfixed64Consume(ch: readingChannel): int(64) throws {
      return sfixed64ConsumeBase(ch);
    }

    proc sfixed32Append(val: int(32), fieldNumber: int, ch: writingChannel) throws {
      if val == 0 then return;

      tagAppend(fieldNumber, fixed32Type, ch);
      sfixed32AppendBase(val, ch);
    }

    proc sfixed32Consume(ch: readingChannel): int(32) throws {
      return sfixed32ConsumeBase(ch);
    }

    proc enumAppend(val: uint(64), fieldNumber: int, ch: writingChannel) throws {
      tagAppend(fieldNumber, varint, ch);
      enumAppendBase(val, ch);
    }

    proc enumConsume(ch: readingChannel): uint(64) throws {
      return enumConsumeBase(ch);
    }

    proc messageAppend(val, fieldNumber: int, ch:writingChannel) throws {
      tagAppend(fieldNumber, lengthDelimited, ch);
      messageAppendBase(val, ch);
    }

    proc messageConsume(ch:readingChannel, type messageType) throws {
      var tmpMem = openmem();
      var memWriter = tmpMem.writer(kind=iokind.little, locking=false);
      var memReader = tmpMem.reader(kind=iokind.little, locking=false);

      var tmpObj: messageType;
      messageConsumeBase(ch, tmpObj, memWriter, memReader);
      tmpMem.close();
      return tmpObj;
    }

    proc consumeUnknownField(fieldNumber: int, wireType: int, ch: readingChannel): bytes throws {
      /*
      Opening a file, and generating a writing channel to give as an argument to the
      append functions. The required bytes(unknown length) are first consumed out of
      `ch` as `val` and then `val` is passed to corresponding append function to be
      encoded as bytes and written to `tmpMem`.
      */
      var s: bytes;
      var tmpMem = openmem();
      var memWriter = tmpMem.writer(kind=iokind.little, locking=false);
      var memReader = tmpMem.reader(kind=iokind.little, locking=false);

      tagAppend(fieldNumber, wireType, memWriter);
      if wireType == varint {
        var val = unsignedVarintConsume(ch)(0);
        unsignedVarintAppend(val, memWriter);
      } else if wireType == fixed64Type {
        var val = fixed64ConsumeBase(ch);
        fixed64AppendBase(val, memWriter);
      } else if wireType == lengthDelimited {
        var val = bytesConsumeBase(ch);
        bytesAppendBase(val, memWriter);
      } else if wireType == fixed32Type {
        var val = fixed32ConsumeBase(ch);
        fixed32AppendBase(val, memWriter);
      }

      memWriter.close();
      memReader.readbytes(s);
      tmpMem.close();
      return s;
    }

  }
  
  pragma "no doc"
  module RepeatedFields {
    /*
      This module contains functions for encoding/decoding repetitive protobuf
      fields.
    */

    use super.WireEncoding;
    use List;
    use IO;

    proc uint64RepeatedAppend(valList: list(uint(64)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var initialOffset = ch.offset();
      ch.mark();
      for val in valList {
        uint64AppendBase(val, ch);
      }
      var currentOffset = ch.offset();
      ch.revert();
      unsignedVarintAppend((currentOffset-initialOffset):uint, ch);
      for val in valList {
        uint64AppendBase(val, ch);
      }
    }

    proc uint64RepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(uint(64));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = uint64ConsumeBase(ch);
        returnList.append(val);
      }
      return returnList;
    }

    proc uint32RepeatedAppend(valList: list(uint(32)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var initialOffset = ch.offset();
      ch.mark();
      for val in valList {
        uint32AppendBase(val, ch);
      }
      var currentOffset = ch.offset();
      ch.revert();
      unsignedVarintAppend((currentOffset-initialOffset):uint, ch);
      for val in valList {
        uint32AppendBase(val, ch);
      }
    }
    
    proc uint32RepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(uint(32));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = uint32ConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }
    
    proc int64RepeatedAppend(valList: list(int(64)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var initialOffset = ch.offset();
      ch.mark();
      for val in valList {
        int64AppendBase(val, ch);
      }
      var currentOffset = ch.offset();
      ch.revert();
      unsignedVarintAppend((currentOffset-initialOffset):uint, ch);
      for val in valList {
        int64AppendBase(val, ch);
      }
    }
    
    proc int64RepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(int(64));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = int64ConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }
    
    proc int32RepeatedAppend(valList: list(int(32)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var initialOffset = ch.offset();
      ch.mark();
      for val in valList {
        int32AppendBase(val, ch);
      }
      var currentOffset = ch.offset();
      ch.revert();
      unsignedVarintAppend((currentOffset-initialOffset):uint, ch);
      for val in valList {
        int32AppendBase(val, ch);
      }
    }
    
    proc int32RepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(int(32));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = int32ConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }
    
    proc boolRepeatedAppend(valList: list(bool), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var payloadLength = valList.size; 
      unsignedVarintAppend(payloadLength:uint, ch);
      for val in valList {
        boolAppendBase(val, ch);
      }
    }

    proc boolRepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(bool);
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = boolConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }
    
    proc sint64RepeatedAppend(valList: list(int(64)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var initialOffset = ch.offset();
      ch.mark();
      for val in valList {
        sint64AppendBase(val, ch);
      }
      var currentOffset = ch.offset();
      ch.revert();
      unsignedVarintAppend((currentOffset-initialOffset):uint, ch);
      for val in valList {
        sint64AppendBase(val, ch);
      }
    }
    
    proc sint64RepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(int(64));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = sint64ConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }
    
    proc sint32RepeatedAppend(valList: list(int(32)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var initialOffset = ch.offset();
      ch.mark();
      for val in valList {
        sint32AppendBase(val, ch);
      }
      var currentOffset = ch.offset();
      ch.revert();
      unsignedVarintAppend((currentOffset-initialOffset):uint, ch);
      for val in valList {
        sint32AppendBase(val, ch);
      }
    }
    
    proc sint32RepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(int(32));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = sint32ConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }
    
    proc bytesRepeatedAppend(valList: list(bytes), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;
      for val in valList {
        tagAppend(fieldNumber, lengthDelimited, ch);
        bytesAppendBase(val, ch);
      }
    }

    proc bytesRepeatedConsume(ch: readingChannel) throws {
      var returnList: list(bytes);
      var val = bytesConsumeBase(ch);
      returnList.append(val);
      return returnList;
    }

    proc stringRepeatedAppend(valList: list(string), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;
      for val in valList {
        tagAppend(fieldNumber, lengthDelimited, ch);
        stringAppendBase(val, ch);
      }
    }

    proc stringRepeatedConsume(ch: readingChannel) throws {
      var returnList: list(string);
      var val = stringConsumeBase(ch);
      returnList.append(val);
      return returnList;
    }

    proc fixed32RepeatedAppend(valList: list(uint(32)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var payloadLength = valList.size * 4; 
      unsignedVarintAppend(payloadLength:uint, ch);
      for val in valList {
        fixed32AppendBase(val, ch);
      }
    }

    proc fixed32RepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(uint(32));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = fixed32ConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }
    
    proc fixed64RepeatedAppend(valList: list(uint(64)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var payloadLength = valList.size * 8; 
      unsignedVarintAppend(payloadLength:uint, ch);
      for val in valList {
        fixed64AppendBase(val, ch);
      }
    }

    proc fixed64RepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(uint(64));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = fixed64ConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }

    proc floatRepeatedAppend(valList: list(real(32)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var payloadLength = valList.size * 4; 
      unsignedVarintAppend(payloadLength:uint, ch);
      for val in valList {
        floatAppendBase(val, ch);
      }
    }

    proc floatRepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(real(32));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = floatConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }
    
    proc doubleRepeatedAppend(valList: list(real(64)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var payloadLength = valList.size * 8; 
      unsignedVarintAppend(payloadLength:uint, ch);
      for val in valList {
        doubleAppendBase(val, ch);
      }
    }

    proc doubleRepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(real(64));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = doubleConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }
    
    proc sfixed64RepeatedAppend(valList: list(int(64)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var payloadLength = valList.size * 8; 
      unsignedVarintAppend(payloadLength:uint, ch);
      for val in valList {
        sfixed64AppendBase(val, ch);
      }
    }

    proc sfixed64RepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(int(64));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = sfixed64ConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }
    
    proc sfixed32RepeatedAppend(valList: list(int(32)), fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var payloadLength = valList.size * 4; 
      unsignedVarintAppend(payloadLength:uint, ch);
      for val in valList {
        sfixed32AppendBase(val, ch);
      }
    }

    proc sfixed32RepeatedConsume(ch: readingChannel) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(int(32));
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = sfixed32ConsumeBase(ch);
        returnList.append(val);
      }
      return returnList; 
    }
    
    proc enumRepeatedAppend(valList, fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;

      tagAppend(fieldNumber, lengthDelimited, ch);
      var initialOffset = ch.offset();
      ch.mark();
      for val in valList {
        enumAppendBase(val:uint, ch);
      }
      var currentOffset = ch.offset();
      ch.revert();
      unsignedVarintAppend((currentOffset-initialOffset):uint, ch);
      for val in valList {
        enumAppendBase(val:uint, ch);
      }
    }

    proc enumRepeatedConsume(ch: readingChannel, type enumType) throws {
      var (payloadLength, _) = unsignedVarintConsume(ch);
      var initialOffset = ch.offset();

      var returnList: list(enumType);
      while true {
        if (ch.offset() - initialOffset) >= payloadLength then break;
        var val = enumConsumeBase(ch);
        returnList.append(val:enumType);
      }
      return returnList;
    }

    proc messageRepeatedAppend(valList, fieldNumber: int, ch: writingChannel) throws {
      if valList.isEmpty() then return;
      for val in valList {
        tagAppend(fieldNumber, lengthDelimited, ch);
        messageAppendBase(val, ch);
      }
    }

    proc messageRepeatedConsume(ch: readingChannel, type messageType) throws {
      var returnList: list(messageType);
      var tmpMem = openmem();
      var memWriter = tmpMem.writer(kind=iokind.little, locking=false);
      var memReader = tmpMem.reader(kind=iokind.little, locking=false);

      var tmpObj: messageType;
      messageConsumeBase(ch, tmpObj, memWriter, memReader);
      returnList.append(tmpObj);
      tmpMem.close();
      return returnList;
    }

  }

}
