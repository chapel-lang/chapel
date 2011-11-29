class myclass {
  var x: int;
  var y: real;
}

class myotherclass {
  var x: real;
  var y: int;
}

proc foo(a) {
  writeln("a is: (", a.x, ", ", a.y, ") // b is: (", b.x, ", ", b.y, ")");
}

var class1: myclass;
var class2: myotherclass;

foo(class1);
foo(class2);
