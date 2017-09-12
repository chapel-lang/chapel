class C {
  type eltType = real;
  var head, tail: atomic int;

  proc init() {
    head.write(0);
    tail.write(0);
    super.init();
  }
}

var myC = new C();
writeln(myC);
