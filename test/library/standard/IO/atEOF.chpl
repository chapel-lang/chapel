use IO;
config const doError = 0;

proc main() {
  var f = openMemFile();

  // check empty file
  assert(f.reader(locking=false).atEOF());
  f.reader(locking=false).assertEOF();

  // check file with some data
  const data = "a bunch of stuff";
  f.writer(locking=false).write(data);
  assert(!f.reader(locking=false).atEOF());

  var r = f.reader(locking=false);
  assert(!r.atEOF());
  var b = r.readAll(string);
  writeln(b);

  // check after we are at EOF
  assert(r.atEOF());
  r.assertEOF();
  r.close();

  var s = "";
  r = f.reader(locking=false);
  while !r.atEOF() {
    var b = r.readByte();
    s.appendCodepointValues(b:int);
  }
  writeln(s);
  r.close();

  if doError == 1 {
    r = f.reader(locking=false);
    r.assertEOF(); // error: we have read nothing
    r.close();
  } else if doError == 2 {
    r = f.reader(locking=false);
    r.readByte();
    r.assertEOF(); // error: have read only 1 byte
    r.close();
  } else if doError == 3 {
    r = f.reader(locking=false);
    r.readBytes(data.size - 1);
    r.assertEOF(); // error: have not read the last byte
    r.close();
  }
}
