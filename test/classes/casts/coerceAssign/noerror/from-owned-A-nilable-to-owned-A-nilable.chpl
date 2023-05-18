// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned A? to owned A?
  var a = new owned A?();
  var a_ = new owned A?();
  a_ = a;
}
proc main() {
  foo();
}
