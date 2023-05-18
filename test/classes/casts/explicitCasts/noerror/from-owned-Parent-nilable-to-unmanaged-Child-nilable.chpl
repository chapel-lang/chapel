// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned Parent? to unmanaged Child?
  var alloc = new owned Parent();
  var a:owned Parent? = alloc;
  var a_ = a:unmanaged Child?;
}
proc main() {
  foo();
}
