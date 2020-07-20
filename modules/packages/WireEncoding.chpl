/* Documentation for WireEncoding */
module WireEncoding {

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

  proc writeToOutputFileHelper(ref message, ch) throws {
    ch.lock();
    defer { ch.unlock(); }
    var binCh: channel(writing=true, kind=iokind.little, locking=false) = ch;
    message._writeToOutputFile(binCh);
  }

  proc parseFromInputFileHelper(ref message, ch) throws {
    ch.lock();
    defer { ch.unlock(); }
    var binCh: channel(writing=false, kind=iokind.little, locking=false) = ch;
    message._parseFromInputFile(binCh);
  }

}
