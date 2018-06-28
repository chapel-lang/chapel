class C {
  var x: owned D(int);

  pragma "use default init"
  class D {
    var y;
  }

  proc init() {
    x = new owned D(2);
  }
}

var myC = new owned C();
writeln(myC);
