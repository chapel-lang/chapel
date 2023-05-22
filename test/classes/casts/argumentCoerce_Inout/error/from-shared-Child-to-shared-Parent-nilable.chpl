// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared Child to shared Parent?
proc bar(inout x: shared Parent?) {
  x = new shared Parent?();
}
proc foo() {
  var a = new shared Child();
  bar(a);
}
proc main() {
  foo();
}
