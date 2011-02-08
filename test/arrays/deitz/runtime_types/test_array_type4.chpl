proc g(x) {
  writeln(typeToString(x.type), ": ", x);
}

proc f(e) {
  var x = e;
  g(x);
}

proc f(type t) {
  var x : t;
  g(x);
}

f([i in 1..3] + reduce ([j in 1..3] j * i));
f([     1..3] (+ reduce ([j in 1..3] j * 1)).type);
