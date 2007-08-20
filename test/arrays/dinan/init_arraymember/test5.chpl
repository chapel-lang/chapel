def f(d): [d] real {
  var x: [d] real = 5.0;
  return x;
}

class C {
  var d = [1..10];
  var x: [d] real = f(d);
}

var c = C();

writeln(c.x);
