class R {
  proc foo() { return 1; }
}

class Wrapper {
  var r: owned R?;
  proc init() {
    this.r = new owned R?();
  }
  forwarding this.getR()!;
  proc getR() ref : owned R? {
    return r;
  }
}

var x = new Wrapper();
writeln(x.foo());
