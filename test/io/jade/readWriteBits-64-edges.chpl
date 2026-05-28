use IO;

proc checkReadBits(ref reader, expected:uint(64), numBits:int) throws {
  var got:uint(64);
  assert(reader.readBits(got, numBits));
  assert(got == expected,
         "Expected 0x" + expected:string + " but got 0x" + got:string +
         " for read of " + numBits:string + " bits");
}

proc checkReadByte(ref reader, expected:uint(8)) throws {
  var got:uint(8);
  assert(reader.readByte(got));
  assert(got == expected,
         "Expected byte 0x" + expected:string + " but got 0x" + got:string);
}

proc main() throws {
  const aligned64 = 0x0123456789abcdef:uint(64);
  const alignedSpill64 = 0xfedcba9876543210:uint(64);
  const misaligned64 = 0x1122334455667788:uint(64);
  const misalignedSpill64 = 0x8877665544332211:uint(64);

  var f = openTempFile();

  {
    var w = f.writer(serializer=new binarySerializer(), locking=false);

    w.writeBits(aligned64, 64);
    w.writeBits(1:uint(64), 1);
    w.writeBits(alignedSpill64, 64);

    w.writeByte(0x5a:uint(8));
    w.writeByte(0xa5:uint(8));
    w.writeBits(misaligned64, 64);
    w.writeBits(1:uint(64), 1);
    w.writeBits(misalignedSpill64, 64);

    w.close();
  }

  {
    var r = f.reader(deserializer=new binaryDeserializer(), locking=false);

    checkReadBits(r, aligned64, 64);
    checkReadBits(r, 1:uint(64), 1);
    checkReadBits(r, alignedSpill64, 64);

    checkReadByte(r, 0x5a:uint(8));
    checkReadByte(r, 0xa5:uint(8));
    checkReadBits(r, misaligned64, 64);
    checkReadBits(r, 1:uint(64), 1);
    checkReadBits(r, misalignedSpill64, 64);

    r.close();
  }

  f.close();
}
