function f(type t, x : ?tt) where tt == t {
  var xx : t = x;
  writeln(xx);
}
f(t = integer, 1);
