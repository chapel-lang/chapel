
inline proc foo(x: int) {
  if x > 0 {
    writeln("alive = ", x);
    foo(x-1);
  }
  else
    writeln("dead");
}

foo(5);
