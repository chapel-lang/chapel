// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared A? to unmanaged A?
  var allocFrom = new shared A();
  var allocTo = new unmanaged A();
  var a:shared A? = allocFrom;
  var a_:unmanaged A? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
