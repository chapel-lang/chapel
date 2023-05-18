// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned Child to unmanaged Parent?
  var alloc = new owned Child();
  var a:owned Child = alloc;
  var a_ = a:unmanaged Parent?;
}
proc main() {
  foo();
}
