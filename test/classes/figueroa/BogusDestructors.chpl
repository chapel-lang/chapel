// See also: test/classes/deinitializers/deinitAndDestruct

class C {
  proc ~C () {writeln("inside ~C");}
}

proc C.~C () {writeln("inside C.~C");}

var c: C;
delete c; // not intended to be executed
