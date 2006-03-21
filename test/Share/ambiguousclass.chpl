class myclass {
  var x: int;
  var y: float;
}

class myotherclass {
  var x: float;
  var y: int;
}

function foo(a) {
  writeln("a is: (", a.x, ", ", a.y, ") -- b is: (", b.x, ", ", b.y, ")");
}

var class1: myclass;
var class2: myotherclass;

foo(class1);
foo(class2);
