// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from unmanaged A? to borrowed A?
  var a = new unmanaged A?();
  var a_ = a:borrowed A?;
}
proc main() {
  foo();
}
