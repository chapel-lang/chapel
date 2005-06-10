type mytup: (integer, float);

function returntwo(): mytup {
  return (1, 2.3);
}

var x: integer;
var y: float;

var t: mytup;

t = returntwo();

writeln("t is: ", t);
