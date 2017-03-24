// See also: test/classes/deinitializers/deinitAndDestruct

class C {
  proc deinit () {writeln("inside ~C");}
}

proc C.deinit () {writeln("inside C.~C");}

var c: C;
delete c; // not intended to be executed
