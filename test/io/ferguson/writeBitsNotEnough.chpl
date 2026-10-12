use IO;

var f = openMemFile();

{
  var w = f.writer(serializer=new binarySerializer(), locking=false);

  w.writeBits(0b011, 3);
  w.writeBits(0b011, 2);
  w.writeBits(0b011, 1); // we have more bits, only write 1 of them
  w.writeBits((-1):uint(64), 2);
}

{
  var r = f.reader(deserializer=new binaryDeserializer(), locking=false);

  var tmp:uint(8);
  r.readBits(tmp, 3);
  assert(tmp == 0b011);
  r.readBits(tmp, 2);
  assert(tmp == 0b11);
  r.readBits(tmp, 1);
  assert(tmp == 0b1);
  r.readBits(tmp, 2);
  assert(tmp == 0b11);
}
