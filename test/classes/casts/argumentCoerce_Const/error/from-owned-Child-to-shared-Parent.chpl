// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned Child to shared Parent
proc bar(const x: shared Parent) {}
proc foo() {
  var a = new owned Child();
  bar(a);
}
proc main() {
  foo();
}
