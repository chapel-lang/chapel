// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned Parent to unmanaged Child
proc bar(out x: unmanaged Child) {
  x = new unmanaged Child();
}
proc foo() {
  var a = new owned Parent();
  bar(a);
}
proc main() {
  foo();
}
