// Should the value of a sync/single variable be read
// when it is the receiver calling a method on the underlying type?
// This test requires "yes". The compiler has a special case that says "no".

use Time;

proc int.showme() { writeln("showme ", this); }

var intSync: sync int;
var intSingle: single int;

cobegin {
  intSync.showme();
  intSingle.showme();
  { sleep(3); exit(0); }  // quit after 3 seconds
}
