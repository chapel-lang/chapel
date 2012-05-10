inline
proc foo(param i : int) where i % 2 == 0 {
  writeln(i, " divided by two is ", i / 2);
}

inline
proc foo(param i : int) where i % 2 == 1 {
  writeln(i, " is not divisible by two");
}

for param i in 1..5 do
  foo(i);
