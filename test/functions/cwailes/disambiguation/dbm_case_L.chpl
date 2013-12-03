class Base {}
class Derived1 : Base {}
class Derived2 : Derived1 {}

proc foo(a:Base) {
  writeln("foo1");
}

proc foo(a:Derived1) {
  writeln("foo2");
}

foo(new Derived2());
