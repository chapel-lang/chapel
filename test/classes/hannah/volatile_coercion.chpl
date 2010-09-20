def main() {
  var x     : volatile int = 1;
  var y     : volatile uint = 2;
  var z     : volatile real = 3.14;

  writeln("x = ", x);  
  writeln("y = ", y);  
  writeln("z = ", z);  

  x = 1.0;
  y = 2.0;
  z = 3;

  writeln("x = ", x);  
  writeln("y = ", y);  
  writeln("z = ", z);  

}

