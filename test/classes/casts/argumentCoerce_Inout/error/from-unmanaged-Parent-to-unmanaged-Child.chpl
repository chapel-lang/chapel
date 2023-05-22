// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged Parent to unmanaged Child
proc bar(inout x: unmanaged Child) {
  x = new unmanaged Child();
}
proc foo() {
  var a = new unmanaged Parent();
  bar(a);
}
proc main() {
  foo();
}
