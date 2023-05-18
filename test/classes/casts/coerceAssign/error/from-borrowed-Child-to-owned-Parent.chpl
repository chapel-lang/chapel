// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed Child to owned Parent
  var allocFrom = new owned Child();
  var a:borrowed Child = allocFrom;
  var a_ = new owned Parent();
  a_ = a;
}
proc main() {
  foo();
}
