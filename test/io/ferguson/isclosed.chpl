use IO;

config const fileName = "test.txt";

var f = open(fileName, ioMode.cwr);

var w = f.writer();

assert(!w.isClosed());
w.close();
assert(w.isClosed());

// Try a channel that was not initialized.
{
  var ch:fileReader(locking=true);
  assert(ch.isClosed());
}

