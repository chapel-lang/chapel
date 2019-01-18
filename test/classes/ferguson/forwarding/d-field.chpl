
record Wrapper {
  var instance; // e.g. some class
  forwarding instance;
  proc deinit() {
    delete instance;
  }
}

class C {
  var field:int;
}

var r = new Wrapper(new unmanaged C());
r.field = 11;
writeln(r.field);


