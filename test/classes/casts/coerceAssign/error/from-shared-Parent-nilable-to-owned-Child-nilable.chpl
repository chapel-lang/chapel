// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Parent? to owned Child?
  var a = new shared Parent?();
  var a_ = new owned Child?();
  a_ = a;
}
proc main() {
  foo();
}
