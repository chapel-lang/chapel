fun foo(x) where x == int || x == float {
  writeln("int or float = ", x);
}

fun foo(x) where x == complex || x == bool {
  writeln("complex or bool = ", x);
}

foo(1);
foo(2.0);
foo(3.0+2.0i);
foo(false);
