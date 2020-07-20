/* Documentation for RepeatedFields */
module RepeatedFields {
  
  use WireEncoding;
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

}
