// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed Child to unmanaged Parent
proc bar(out x: unmanaged Parent) {
  x = new unmanaged Parent();
}
proc foo() {
  var alloc = new owned Child();
  var a:borrowed Child = alloc;
  bar(a);
}
proc main() {
  foo();
}
