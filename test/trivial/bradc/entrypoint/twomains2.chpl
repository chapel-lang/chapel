var x = 32;
var y = -1;

def main(x: int) {
  writeln("In main(x), x is: ", x);
}

def main() {
  var y = 42;

  writeln("x is: ", x);
  writeln("y is: ", y);  
  main(y);
}

