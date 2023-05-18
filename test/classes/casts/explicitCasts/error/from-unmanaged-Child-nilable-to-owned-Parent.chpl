// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from unmanaged Child? to owned Parent
  var alloc = new unmanaged Child();
  var a:unmanaged Child? = alloc;
  var a_ = a:owned Parent;
}
proc main() {
  foo();
}
