use Map;

class C {
  var i: int;
}

var m: map(string, shared C);

m.add("one", new shared C(1));
m.add("two", new shared C(2));

for c in m.items() {
  writeln(c(2).type:string);
  c(2).i *= -1;
}

writeln(m("one").type:string);
writeln(m("one"));

m("two").i = 2;
writeln(m);
