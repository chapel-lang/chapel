class C {
  var x: int;
}

var c: C;

on Locales(1) {
  c = new C();
}

_debugWriteln(localeID(), " c.locale = ", c.locale);
_debugWriteln(localeID(), " c.x.locale = ", c.x.locale);

def C.foo() {
  _debugWriteln(localeID(), " this.locale = ", this.locale);
}

c.foo();

def C.goo() {
  on Locales(1) do
    foo();
  _debugWriteln(localeID(), " this.locale = ", this.locale);
}

c.goo();

on Locales(1) do
  c.goo();

def bar() {
  var d: C;
  d = c;
  _debugWriteln(localeID(), " d.locale = ", d.locale);
  _debugWriteln(localeID(), " d.x.locale = ", d.x.locale);
}

bar();
