enum color { red = foo(), green, blue };

config var x = 3;

proc foo() {
  return x;
}

writeln(color.red:int);
