// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged Child to unmanaged Parent
proc bar(const in x: unmanaged Parent) {}
proc foo() {
  var a = new unmanaged Child();
  bar(a);
}
proc main() {
  foo();
}
