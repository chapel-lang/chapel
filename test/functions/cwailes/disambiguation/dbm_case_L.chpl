class Base {}
class Derived1 : Base {}
class Derived2 : Derived1 {}

proc foo(a:borrowed Base) {
  writeln("foo1");
}

proc foo(a:borrowed Derived1) {
  writeln("foo2");
}

foo(new borrowed Derived2());
