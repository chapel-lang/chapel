use IO;

proc main() {
  var f = openMemFile();

  // check empty file
  assert(f.reader(locking=false).atEOF()); 
  f.reader(locking=false).assertEOF();

  // check file with some data
  f.writer(locking=false).write("a bunch of stuff");
  assert(!f.reader(locking=false).atEOF()); 

  var r = f.reader(locking=false);
  var b = r.readAll(bytes);

  // check after we are at EOF
  assert(r.atEOF());
  r.assertEOF();
}
