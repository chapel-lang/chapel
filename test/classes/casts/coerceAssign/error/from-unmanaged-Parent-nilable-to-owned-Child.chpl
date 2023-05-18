// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Parent? to owned Child
  var a = new unmanaged Parent?();
  var a_ = new owned Child();
  a_ = a;
}
proc main() {
  foo();
}
