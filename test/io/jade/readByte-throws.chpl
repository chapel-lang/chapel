use IO;

var f = openTempFile();

{
  var w = f.writer();
  w.writeByte(0x11);
}

{
  var r = f.reader();
  var b = r.readByte(); // should not throw
  b = r.readByte(); // should throw
}
