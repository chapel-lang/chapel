// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged Child to unmanaged Parent
proc bar(inout x: unmanaged Parent) {
  x = new unmanaged Parent();
}
proc foo() {
  var a = new unmanaged Child();
  bar(a);
}
proc main() {
  foo();
}
