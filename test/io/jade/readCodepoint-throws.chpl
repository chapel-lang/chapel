use IO;

var f = openTempFile();

{
  var w = f.writer();
  w.writeCodepoint(0x11);
}

{
  var r = f.reader();
  var b = r.readCodepoint(); // should not throw
  b = r.readCodepoint(); // should throw
}
