// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Child? to owned Parent
  var a = new owned Child?();
  var a_ = new owned Parent();
  a_ = a;
}
proc main() {
  foo();
}
