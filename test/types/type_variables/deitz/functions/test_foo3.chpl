proc foo(type t) {
  var x : t;
  writeln(x);
  if x > 0 then
    foo(real);
}

foo(int);
