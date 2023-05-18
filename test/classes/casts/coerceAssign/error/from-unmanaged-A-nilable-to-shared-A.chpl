// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged A? to shared A
  var allocFrom = new unmanaged A();
  var allocTo = new shared A();
  var a:unmanaged A? = allocFrom;
  var a_:shared A = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
