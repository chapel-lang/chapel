class Base {}
class Derived1 : Base {}
class Derived2 : Derived1 {}

proc foo(a:Derived1) {
  writeln("foo1");
  delete a;
}

proc foo(a:Base) {
  writeln("foo2");
  delete a;
}

foo(new Derived2());

