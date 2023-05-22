// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged Child to owned Parent
proc bar(ref x: owned Parent) {}
proc foo() {
  var a = new unmanaged Child();
  bar(a);
}
proc main() {
  foo();
}
