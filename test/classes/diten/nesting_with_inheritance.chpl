class baseclass {
  var x: int;

  proc foo() {
    x = 3;
  }
}

class myouter {
  var y: int;
  var x: int;
  class nested : baseclass {
    proc bar() {
      writeln("x should be 0. x is: ", x);
      foo();
      writeln("x should be 3. x is: ", x);
      writeln("outer.x should be 0. outer.x is: ", outer.x);
    }
  }
  proc runit() {
    var nes = new nested();
    nes.bar();
  }
}

var outervar = new myouter();
outervar.runit();
