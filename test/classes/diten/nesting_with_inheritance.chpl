class baseclass {
  var x: int;

  def foo() {
    x = 3;
  }
}

class outer {
  var y: int;
  var x: int;
  class nested : baseclass {
    def bar() {
      writeln("x should be 0. x is: ", x);
      foo();
      writeln("x should be 3. x is: ", x);
      writeln("outer.x should be 0. outer.x is: ", outer.x);
    }
  }
  def runit() {
    var nes = new nested();
    nes.bar();
  }
}

var outervar = new outer();
outervar.runit();
