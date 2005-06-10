function returntwo(): (integer, float) {
  return (1, 2.3);
}

var x: integer;
var y: float;

var t: (integer, float);

t = returntwo();

writeln("t is: ", t);
