// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared Child to unmanaged Parent
proc bar(in x: unmanaged Parent) {}
proc foo() {
  var a = new shared Child();
  bar(a);
}
proc main() {
  foo();
}
