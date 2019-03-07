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
    }
  }
  proc runit() {
    var nes = new unmanaged nested();

    nes.bar();

    delete nes;
  }
}

var outervar = new unmanaged myouter();

outervar.runit();

delete outervar;

