/* Documentation for Fields */
module Fields {

  use IO;
  use WireEncoding;

  proc uint64Append(val: uint(64), fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, varint, ch);
    uint64AppendBase(val, ch);
  }

  proc uint64Consume(ch: readingChannel): uint(64) throws {
    return uint64ConsumeBase(ch);
  }

  proc uint32Append(val: uint(32), fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, varint, ch);
    uint32AppendBase(val, ch);
  }

  proc uint32Consume(ch: readingChannel): uint(32) throws {
    return uint32ConsumeBase(ch);
  }
  
  proc int64Append(val: int(64), fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, varint, ch);
    int64AppendBase(val, ch);
  }

  proc int64Consume(ch: readingChannel): int throws {
    return int64ConsumeBase(ch);
  }

  proc int32Append(val: int(32), fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, varint, ch);
    int32AppendBase(val, ch);
  }

  proc int32Consume(ch: readingChannel): int(32) throws {
    return int32ConsumeBase(ch);
  }

  proc boolAppend(val: bool, fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, varint, ch);
    boolAppendBase(val, ch);
  }

  proc boolConsume(ch: readingChannel): bool throws {
    return boolConsumeBase(ch);
  }

  proc sint64Append(val: int(64), fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, varint, ch);
    sint64AppendBase(val, ch);
  }

  proc sint64Consume(ch: readingChannel): int(64) throws {
    return sint64ConsumeBase(ch);
  }

  proc sint32Append(val: int(64), fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, varint, ch);
    sint32AppendBase(val, ch);
  }

  proc sint32Consume(ch: readingChannel): int(32) throws {
    return sint32ConsumeBase(ch);
  }

  proc bytesAppend(val: bytes, fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, lengthDelimited, ch);
    bytesAppendBase(val, ch);
  }

  proc bytesConsume(ch: readingChannel): bytes throws {
    return bytesConsumeBase(ch);
  }

  proc stringAppend(val: string, fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, lengthDelimited, ch);
    stringAppendBase(val, ch);
  }

  proc stringConsume(ch: readingChannel): string throws {
    return stringConsumeBase(ch);
  }

  proc fixed32Append(val: uint(32), fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, fixed32Type, ch);
    fixed32AppendBase(val, ch);
  }

  proc fixed32Consume(ch: readingChannel): uint(32) throws {
    return fixed32ConsumeBase(ch);
  }

  proc fixed64Append(val: uint(64), fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, fixed64Type, ch);
    fixed64AppendBase(val, ch);
  }

  proc fixed64Consume(ch: readingChannel): uint(64) throws {
    return fixed64ConsumeBase(ch);
  }

  proc floatAppend(val: real(32), fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, fixed32Type, ch);
    floatAppendBase(val, ch);
  }
  
  proc floatConsume(ch: readingChannel): real(32) throws {
    return floatConsumeBase(ch);
  }

  proc doubleAppend(val: real(64), fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, fixed64Type, ch);
    doubleAppendBase(val, ch);
  }
  
  proc doubleConsume(ch: readingChannel): real(64) throws {
    return doubleConsumeBase(ch);
  }

  proc sfixed64Append(val: int(64), fieldNumber: int, ch: writingChannel) throws {
    tagAppend(fieldNumber, fixed64Type, ch);
    sfixed64AppendBase(val, ch);
  }

  proc sfixed64Consume(ch: readingChannel): int(64) throws {
    return sfixed64ConsumeBase(ch);
  }

  proc sfixed32Append(val: int(32), fieldNumber: int, ch: writingChannel) throws {
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
