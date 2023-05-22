// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed Child to owned Parent
proc bar(inout x: owned Parent) {
  x = new owned Parent();
}
proc foo() {
  var alloc = new owned Child();
  var a:borrowed Child = alloc;
  bar(a);
}
proc main() {
  foo();
}
