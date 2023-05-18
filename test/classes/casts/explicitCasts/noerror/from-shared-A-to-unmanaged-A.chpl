// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared A to unmanaged A
  var alloc = new shared A();
  var a:shared A = alloc;
  var a_ = a:unmanaged A;
}
proc main() {
  foo();
}
