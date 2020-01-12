use IO;

config const fileName = "test.txt";

var f = open(fileName, iomode.cwr);

var w = f.writer();

assert(!w.isclosed());
w.close();
assert(w.isclosed());

// Try a channel that was not initialized.
{
  var ch:channel(writing=false, kind=iokind.dynamic, locking=true);
  assert(ch.isclosed());
}

