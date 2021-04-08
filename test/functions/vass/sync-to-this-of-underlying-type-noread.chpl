// Should the value of a sync/single variable be read
// when it is the receiver calling a method on the underlying type?
// This test historically required "yes". The compiler has a special
// case that says "no".  With the change to require explicit read/write
// methods on sync, we have to add them explicitly.

use Time;

proc int.showme() { writeln("showme ", this); }

var intSync: sync int;
var intSingle: single int;

cobegin {
  intSync.showme();
  intSingle.showme();
  { sleep(3); exit(0); }  // quit after 3 seconds
}
