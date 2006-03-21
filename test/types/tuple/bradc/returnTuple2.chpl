type mytup: (int, float);

fun returntwo(): mytup {
  return (1, 2.3);
}

var x: int;
var y: float;

var t: mytup;

t = returntwo();

writeln("t is: ", t);
