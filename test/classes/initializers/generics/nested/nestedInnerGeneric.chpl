class C {
  var x: D(int);

  pragma "use default init"
  class D {
    var y;
  }

  proc init() {
    x = new D(2);
  }
}

var myC = new C();
writeln(myC);
