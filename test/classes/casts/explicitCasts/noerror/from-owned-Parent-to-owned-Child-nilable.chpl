// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from owned Parent to owned Child?
  var alloc = new owned Parent();
  var a:owned Parent = alloc;
  var a_ = a:owned Child?;
}
proc main() {
  foo();
}
