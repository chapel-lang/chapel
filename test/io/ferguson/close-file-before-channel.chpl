//This program is in error; the test
// is just to lock in that we get a reasonable error message.

use IO;

proc test() {
  var file = open("test.txt", iomode.cw);
  var chan = file.writer();
  chan.write("hello");
  file.close();
  chan; // keep it alive to here
}

test();
