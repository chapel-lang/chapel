// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Child to unmanaged Parent?
  var a = new owned Child();
  var a_ = new unmanaged Parent?();
  a_ = a;
}
proc main() {
  foo();
}
