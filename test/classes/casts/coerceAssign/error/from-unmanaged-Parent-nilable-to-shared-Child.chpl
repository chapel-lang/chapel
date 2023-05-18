// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Parent? to shared Child
  var a = new unmanaged Parent?();
  var a_ = new shared Child();
  a_ = a;
}
proc main() {
  foo();
}
