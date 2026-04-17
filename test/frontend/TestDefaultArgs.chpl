
use Print;

proc a(x = 0) {
  println(x);
}

proc b(x: int, y = x*2) {
  println(x);
  println(y);
}

proc c(x = 3, y = x*2) {
  println(x);
  println(y);
}

proc main() {
  a();
  a(1);

  b(2);
  b(2, 5);

  c();
  c(10);
  c(10, 100);
}
