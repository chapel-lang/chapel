// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared A to owned A
  var allocFrom = new shared A();
  var allocTo = new owned A();
  var a:shared A = allocFrom;
  var a_:owned A = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
