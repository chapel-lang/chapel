use IO;

var f = openTempFile();

{
  var w = f.writer();
  w.writeByte(0x11);
}

{
  var r = f.reader();
  r.readByte(); // should not throw
  try {
    r.readByte(); // should throw
    assert(false);
  } catch e:UnexpectedEofError {
    assert(true);
  } catch { assert(false); }
}
