// Modified from
// test/classes/vass/nested-class-with-user-defined-constructor-1.chpl
class enclosing {

  class inner {
    proc init() {
      writeln("in inner initializer");
    }
  } // class inner

  proc new_inner() {
    return new inner();
  }

} // class outer

var e = new enclosing();
var i = e.new_inner();
writeln(i.outer == e);

delete i;
delete e;
