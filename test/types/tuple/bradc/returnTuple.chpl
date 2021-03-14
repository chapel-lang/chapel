proc returntwo(): (int, real) {
  return (1, 2.3);
}

var x: int;
var y: real;

var t: (int, real);

t = returntwo();

writeln("t is: ", t);
