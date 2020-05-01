class C {
  var x: int;
}

var c: unmanaged C?;

on Locales(1) {
  c = new unmanaged C();
}

writeln(here.id, " c.locale.id = ", c!.locale.id);
writeln(here.id, " c.x.locale.id = ", c!.x.locale.id);

proc C.foo() {
  writeln(here.id, " this.locale.id = ", this.locale.id);
}

c!.foo();

proc C.goo() {
  on Locales(1) do
    foo();
  writeln(here.id, " this.locale.id = ", this.locale.id);
}

c!.goo();

on Locales(1) do
  c!.goo();

proc bar() {
  var d: unmanaged C = c!;
  writeln(here.id, " d.locale.id = ", d.locale.id);
  writeln(here.id, " d.x.locale.id = ", d.x.locale.id);
}

bar();

delete c;
