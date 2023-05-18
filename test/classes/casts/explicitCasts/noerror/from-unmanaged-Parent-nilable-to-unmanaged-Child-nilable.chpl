// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from unmanaged Parent? to unmanaged Child?
  var a = new unmanaged Parent?();
  var a_ = a:unmanaged Child?;
}
proc main() {
  foo();
}
