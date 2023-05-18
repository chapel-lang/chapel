// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned A? to unmanaged A
  var alloc = new owned A();
  var a:owned A? = alloc;
  var a_ = a:unmanaged A;
}
proc main() {
  foo();
}
