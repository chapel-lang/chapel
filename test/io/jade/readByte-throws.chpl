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
    writeln("Incorrectly did not throw");
  } catch e:UnexpectedEofError {
    writeln("Correctly threw UnexpectedEofError");
  } catch { 
    writeln("Incorrectly threw some other error");
  }
}
