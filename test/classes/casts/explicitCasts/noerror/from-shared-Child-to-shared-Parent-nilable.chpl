// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared Child to shared Parent?
  var a = new shared Child();
  var a_ = a:shared Parent?;
}
proc main() {
  foo();
}
