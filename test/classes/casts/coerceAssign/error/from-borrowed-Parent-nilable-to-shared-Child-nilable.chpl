// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed Parent? to shared Child?
  var allocFrom = new owned Parent?();
  var a:borrowed Parent? = allocFrom;
  var a_ = new shared Child?();
  a_ = a;
}
proc main() {
  foo();
}
