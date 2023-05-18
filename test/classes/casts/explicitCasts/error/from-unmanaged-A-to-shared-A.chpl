// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from unmanaged A to shared A
  var alloc = new unmanaged A();
  var a:unmanaged A = alloc;
  var a_ = a:shared A;
}
proc main() {
  foo();
}
