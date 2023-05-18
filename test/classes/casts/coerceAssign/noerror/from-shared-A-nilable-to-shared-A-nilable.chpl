// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared A? to shared A?
  var allocFrom = new shared A();
  var allocTo = new shared A();
  var a:shared A? = allocFrom;
  var a_:shared A? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
