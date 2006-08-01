fun f(type t, x : ?tt) where tt == t {
  var xx : t = x;
  writeln("like types ", xx);
}

fun f(type t, x : ?tt) where tt != t {
  var xx : t = x;
  writeln("unlike types ", xx);
}

f(t = int, 1);
f(t = float, 2);
