proc f() {
  var x: [1..10] real = 5.0;
  return x;
}

class C {
  var d = {1..10};
  var x: [d] real = f();
}

var cOwn = new owned C();
var c = cOwn.borrow();

writeln(c.x);
