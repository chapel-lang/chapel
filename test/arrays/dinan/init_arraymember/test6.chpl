def f() {
  var x: [1..10] real = 5.0;
  return x;
}

class C {
  var d = [1..10];
  var x: [d] real;
  
  def initialize() {
    x = f();
  }
}

var c = new C();

writeln(c.x);
