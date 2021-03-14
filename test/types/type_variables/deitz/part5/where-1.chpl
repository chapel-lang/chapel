proc f(type t, x : ?tt) where tt == t {
  var xx : t = x;
  writeln("like types ", xx);
}

proc f(type t, x : ?tt) where tt != t {
  var xx : t = x;
  writeln("unlike types ", xx);
}

f(t = int, 1);
f(t = real, 2);
