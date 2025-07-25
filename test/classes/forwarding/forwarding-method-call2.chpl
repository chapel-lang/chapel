class R {
  proc foo() { return 1; }
}

class Wrapper {
  var r: owned R;
  proc init() {
    this.r = new owned R();
  }
  forwarding something();
}

proc Wrapper.something() { return r.borrow(); }

var x = new Wrapper();
writeln(x.foo());
