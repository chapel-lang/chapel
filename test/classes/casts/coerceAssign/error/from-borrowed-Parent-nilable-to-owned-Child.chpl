// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed Parent? to owned Child
  var allocFrom = new owned Parent?();
  var a:borrowed Parent? = allocFrom;
  var a_ = new owned Child();
  a_ = a;
}
proc main() {
  foo();
}
