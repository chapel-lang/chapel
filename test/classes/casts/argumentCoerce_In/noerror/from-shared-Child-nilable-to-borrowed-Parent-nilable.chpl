// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared Child? to borrowed Parent?
proc bar(in x: borrowed Parent?) {}
proc foo() {
  var a = new shared Child?();
  bar(a);
}
proc main() {
  foo();
}
