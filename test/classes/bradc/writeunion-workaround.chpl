union myunion {
  var x: integer;
  var y: float;
}

var a: myunion;

writeln("a is: ", a);

a.x = 1;

writeln("a is: ", a);

a.y = 2.3;

writeln("a is: ", a);

