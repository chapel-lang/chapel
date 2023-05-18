// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // casting from shared Child to borrowed Parent
  var alloc = new shared Child();
  var a:shared Child = alloc;
  var a_ = a:borrowed Parent;
}
proc main() {
  foo();
}
