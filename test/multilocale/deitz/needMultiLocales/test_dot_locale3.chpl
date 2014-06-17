class C {
  var data: int;
  var next: C;
}

var c1, c2: C;

on Locales(1) {
  c1 = new C();
  c2 = new C();
}

c1.next = c2;

writeln(c1.locale.id);
writeln(c1.data.locale.id);
writeln(c2.locale.id);
writeln(c2.data.locale.id);
writeln(c1.next.locale.id);
writeln(c1.next.data.locale.id);
