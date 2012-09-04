class C {
  var x: int;
}

var c: C;

on rootLocale.getLocale(1) {
  c = new C();
}

writeln(here.id, " c.locale.id = ", c.locale.id);
writeln(here.id, " c.x.locale.id = ", c.x.locale.id);

proc C.foo() {
  writeln(here.id, " this.locale.id = ", this.locale.id);
}

c.foo();

proc C.goo() {
  on rootLocale.getLocale(1) do
    foo();
  writeln(here.id, " this.locale.id = ", this.locale.id);
}

c.goo();

on rootLocale.getLocale(1) do
  c.goo();

proc bar() {
  var d: C;
  d = c;
  writeln(here.id, " d.locale.id = ", d.locale.id);
  writeln(here.id, " d.x.locale.id = ", d.x.locale.id);
}

bar();
