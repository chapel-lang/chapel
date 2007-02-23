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
      writeln("this0.x should be 0. this0.x is: ", this0.x);
    }
  }
  def runit() {
    var nes = nested();
    nes.bar();
  }
}

var outervar = outer();
outervar.runit();
