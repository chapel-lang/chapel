class Par { var a, b: int; proc writea() { writeln("a = ", a); } }
class Sub: Par { var c, d: int; }

var x: Par;
var y: Sub = new Sub(c = 3, d = 4);

atomic {
  x = new Par(1,2);  
  x.a = 5;
  delete x;
}

writeln ("Update x: ", x.a, " ", y.a, " ", y.c);

atomic {
  y.a = 1;
  y.c = 2;
}

writeln ("Update y: ", y.a, " ", y.c);