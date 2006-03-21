fun returntwo(): (int, float) {
  return (1, 2.3);
}

var x: int;
var y: float;

var t: (int, float);

t = returntwo();

writeln("t is: ", t);
