// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from unmanaged Parent? to owned Child?
  var alloc = new unmanaged Parent();
  var a:unmanaged Parent? = alloc;
  var a_ = a:owned Child?;
}
proc main() {
  foo();
}
