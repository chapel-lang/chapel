
fun foo(x: int) {
  writeln("Got an int");
}

fun foo(x: float) {
  writeln("Got a float");
}

fun foo(x: string) {
  writeln("Got a string");
}

fun unrollme(x ...?numargs) {
  for param i in 1..numargs {
    foo(x(i));
  }
  writeln();
}

unrollme(1,2,3,4);

unrollme(1, 2.3, "4.5.6", 78);


