var x = 32;
var y = -1;

proc main(x: int) {
  writeln("In main(x), x is: ", x);
}

proc main() {
  var y = 42;

  writeln("x is: ", x);
  writeln("y is: ", y);  
  main(y);
}

