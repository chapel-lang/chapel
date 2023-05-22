// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned Child to owned Parent
proc bar(const in x: owned Parent) {}
proc foo() {
  var a = new owned Child();
  bar(a);
}
proc main() {
  foo();
}
