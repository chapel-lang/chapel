// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared Parent to owned Child
proc bar(const x: owned Child) {}
proc foo() {
  var a = new shared Parent();
  bar(a);
}
proc main() {
  foo();
}
