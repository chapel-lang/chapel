class C {
  var x: int;
}

var c1: C;
var c2: C;
var c3: C;
var c4: C;

on Realms(0).Locales(0) do c1 = new C(1);
on Realms(0).Locales(1) do c2 = new C(2);
on Realms(1).Locales(0) do c3 = new C(3);
on Realms(1).Locales(1) do c4 = new C(4);

writeln("c1.locale.id = ", c1.locale.id);
writeln("c1.locale.id = ", c2.locale.id);
writeln("c3.locale.id = ", c3.locale.id);
writeln("c4.locale.id = ", c4.locale.id);
/*
proc testit(c: C) {
  on c do {
    writeln("here.id = ", here.id);
    writeln("c1.id = ", c
  }
}
*/
