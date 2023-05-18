// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned Parent to shared Child?
  var a = new owned Parent();
  var a_ = a:shared Child?;
}
proc main() {
  foo();
}
