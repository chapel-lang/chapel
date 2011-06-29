proc main() {
  var i     : volatile int = 3;
  var ui    : volatile uint = 2;
  var r     : volatile real = 3.14;

  writeln("i  = ", i);  
  writeln("ui = ", ui);  
  writeln("r  = ", r);  

  i  = true;
  ui = false;
  r  = 3;

  writeln("i  = ", i);  
  writeln("ui = ", ui);  
  writeln("r  = ", r);  
}

