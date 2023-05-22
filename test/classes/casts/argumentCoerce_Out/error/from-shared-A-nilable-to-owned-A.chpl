// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared A? to owned A
proc bar(out x: owned A) {
  x = new owned A();
}
proc foo() {
  var a = new shared A?();
  bar(a);
}
proc main() {
  foo();
}
