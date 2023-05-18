// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from unmanaged Parent? to shared Child
  var a = new unmanaged Parent?();
  var a_ = a:shared Child;
}
proc main() {
  foo();
}
