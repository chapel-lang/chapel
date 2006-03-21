var x = 32;
var y = -1;

fun main(x: int) {
  writeln("In main(x), x is: ", x);
}

fun main() {
  var y = 42;

  writeln("x is: ", x);
  writeln("y is: ", y);  
  main(y);
}

