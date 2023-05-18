// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned A? to unmanaged A?
  var allocFrom = new owned A();
  var allocTo = new unmanaged A();
  var a:owned A? = allocFrom;
  var a_:unmanaged A? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
