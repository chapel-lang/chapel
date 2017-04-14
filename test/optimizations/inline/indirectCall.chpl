
inline proc bar(x: int) {
  if x > 0 then
    foo(x-1);
  else
    writeln("bar dead");
}

inline proc foo(x: int) {
  if x > 0 then
    bar(x-1);
  else
    writeln("foo dead");
}

foo(10);
