// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed A to shared A?
  var allocFrom = new owned A();
  var allocTo = new shared A();
  var a:borrowed A = allocFrom;
  var a_:shared A? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
