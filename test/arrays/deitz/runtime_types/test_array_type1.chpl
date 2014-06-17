proc f(e) {
  var a = e;
  return a;
}

proc f(type t) {
  var a: t;
  for (aa,i) in zip(a,1..) do
    aa = i;
  return a;
}

var F1 = f([1..5] real);
var F2 = f([1..5] 2.0i);

writeln(F1);
writeln(F2);

writeln(typeToString(F1.type));
writeln(typeToString(F2.type));
