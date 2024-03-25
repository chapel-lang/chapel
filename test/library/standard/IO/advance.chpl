use IO;

/* Tests a user-submitted bug with the channel.advance() function */
proc main() {
  var f = open('advance.txt', ioMode.r),
      c = f.reader(locking=false);


  writeln(c.read(int));
  c.advance(2);
  writeln(c.read(int));
  try! c.advance(2);
  writeln(c.read(int));
}
