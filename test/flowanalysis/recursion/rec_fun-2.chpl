
fun g(x) {
 if (x > 19)
   return x;
  return g(x + 1);
}

fun f(x) {
  return g(x);
}


writeln(f(1));
writeln(f(1.0));
