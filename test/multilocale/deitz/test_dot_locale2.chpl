class C {
  var x: int;
}

var c: C;

on Locales(1) {
  c = new C();
}

_debugWriteln(here.id, " c.locale.id = ", c.locale.id);
_debugWriteln(here.id, " c.x.locale.id = ", c.x.locale.id);

def C.foo() {
  _debugWriteln(here.id, " this.locale.id = ", this.locale.id);
}

c.foo();

def C.goo() {
  on Locales(1) do
    foo();
  _debugWriteln(here.id, " this.locale.id = ", this.locale.id);
}

c.goo();

on Locales(1) do
  c.goo();

def bar() {
  var d: C;
  d = c;
  _debugWriteln(here.id, " d.locale.id = ", d.locale.id);
  _debugWriteln(here.id, " d.x.locale.id = ", d.x.locale.id);
}

bar();
