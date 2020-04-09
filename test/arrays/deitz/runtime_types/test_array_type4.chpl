proc g(x) {
  writeln(x.type:string, ": ", x);
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
f([     1..3] ( minloc reduce ([j in 1..3] (j,j)) ).type);
