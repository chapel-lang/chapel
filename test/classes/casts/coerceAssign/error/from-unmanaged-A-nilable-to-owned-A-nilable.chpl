// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged A? to owned A?
  var allocFrom = new unmanaged A();
  var allocTo = new owned A();
  var a:unmanaged A? = allocFrom;
  var a_:owned A? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
