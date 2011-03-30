proc main() {
  var b     : volatile bool = false;
  var i     : volatile int = 1;
  var ui    : volatile uint = 2;
  var r     : volatile real = 3.14;

  writeln("b  = ", b);
  writeln("i  = ", i);  
  writeln("ui = ", ui);  
  writeln("r  = ", r);  

  b  = 1;
  i  = 1.0;
  ui = 2.0;
  r  = 3;

  writeln("b  = ", b);
  writeln("i  = ", i);  
  writeln("ui = ", ui);  
  writeln("r  = ", r);  
}

