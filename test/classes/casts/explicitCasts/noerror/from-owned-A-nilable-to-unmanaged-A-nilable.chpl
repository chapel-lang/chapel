// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned A? to unmanaged A?
  var a = new owned A?();
  var a_ = a:unmanaged A?;
}
proc main() {
  foo();
}
