// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared Child to owned Parent
proc bar(out x: owned Parent) {
  x = new owned Parent();
}
proc foo() {
  var a = new shared Child();
  bar(a);
}
proc main() {
  foo();
}
