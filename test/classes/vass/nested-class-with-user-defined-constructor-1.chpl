class enclosing {

  class inner {
    proc inner() {
      writeln("in inner constructor");
    }
  } // class inner

  proc new_inner() {
    return new inner();
  }

} // class outer

var e = new enclosing();
var i = e.new_inner();
writeln(i.outer == e);
