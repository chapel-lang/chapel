class myclass {
  var x: integer;
  var y: float;
}

class myotherclass {
  var x: float;
  var y: integer;
}

function foo(a) {
  writeln("a is: (", a.x, ", ", a.y, ") -- b is: (", b.x, ", ", b.y, ")");
}

var class1: myclass;
var class2: myotherclass;

foo(class1);
foo(class2);
