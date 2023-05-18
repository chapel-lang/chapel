// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned Parent? to owned Child
  var a = new owned Parent?();
  var a_ = a:owned Child;
}
proc main() {
  foo();
}
