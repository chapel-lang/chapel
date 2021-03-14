// When 'this' has in-intent, it should snapshot the actual.

var GLOB = 5;
proc inc() { GLOB += 1; }

inline proc int.testme() {
  writeln(this);
  inc();
  writeln(this);
}

GLOB.testme();
