use Map;

class C {
  var i: int;
}

var m: map(int, shared C);
m.add(1, new shared C(1));
writeln(m);
writeln(m[1]);
writeln(m[1].type:string);

m[1] = new shared C(2);
writeln(m);
writeln(m[1]);
writeln(m[1].type:string);

var m2 = new map(int, owned C);
m2.add(1, new C(1));
writeln(m2);
writeln(m2[1]);
writeln(m2[1].type:string);

m2[1] = new owned C(2);
writeln(m2);
writeln(m2[1]);
writeln(m2[1].type:string);
