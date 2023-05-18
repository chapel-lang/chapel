// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared Parent to unmanaged Child?
  var a = new shared Parent();
  var a_ = a:unmanaged Child?;
}
proc main() {
  foo();
}
