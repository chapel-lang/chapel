// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from borrowed A to unmanaged A
  var alloc = new owned A();
  var a:borrowed A = alloc;
  var a_ = a:unmanaged A;
}
proc main() {
  foo();
}
