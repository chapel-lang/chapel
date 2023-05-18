// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from borrowed Child to owned Parent
  var alloc = new owned Child();
  var a:borrowed Child = alloc;
  var a_ = a:owned Parent;
}
proc main() {
  foo();
}
