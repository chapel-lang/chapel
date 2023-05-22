// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared Parent? to shared Child
proc bar(ref x: shared Child) {}
proc foo() {
  var a = new shared Parent?();
  bar(a);
}
proc main() {
  foo();
}
