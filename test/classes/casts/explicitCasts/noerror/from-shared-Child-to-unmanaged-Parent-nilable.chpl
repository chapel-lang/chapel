// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared Child to unmanaged Parent?
  var alloc = new shared Child();
  var a:shared Child = alloc;
  var a_ = a:unmanaged Parent?;
}
proc main() {
  foo();
}
