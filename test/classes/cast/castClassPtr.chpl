use CTypes;

class Func {
  var x: int;

  proc postinit() {
    writeln("created");
  }

  proc this() const {
    writeln("invoked");
    return x;
  }
}

proc main() {
  var c = new unmanaged Func(42);
  writeln(c);

  var p: c_ptr(void) = c_ptrTo(c);
  var c2: unmanaged Func = (p: unmanaged Func?)!;
  writeln(c2);
  writeln(c2());
  assert(c_ptrTo(c) == c_ptrTo(c2));
  writeln("Addresses match!");

  delete c;
}
