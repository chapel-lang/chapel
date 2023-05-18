// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared Parent to shared Child?
  var a = new shared Parent();
  var a_ = a:shared Child?;
}
proc main() {
  foo();
}
