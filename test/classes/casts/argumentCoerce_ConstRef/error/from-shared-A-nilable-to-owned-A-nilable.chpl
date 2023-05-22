// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared A? to owned A?
proc bar(const ref x: owned A?) {}
proc foo() {
  var a = new shared A?();
  bar(a);
}
proc main() {
  foo();
}
