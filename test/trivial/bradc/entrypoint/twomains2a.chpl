var x = 32;
var y = -1;

function main() {
  var y = 42;

  writeln("x is: ", x);
  writeln("y is: ", y);  
  main(y);
}

function main(x: integer) {
  writeln("In main(x), x is: ", x);
}

