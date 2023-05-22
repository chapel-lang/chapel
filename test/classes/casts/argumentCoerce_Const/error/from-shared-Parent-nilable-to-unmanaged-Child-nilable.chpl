// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared Parent? to unmanaged Child?
proc bar(const x: unmanaged Child?) {}
proc foo() {
  var a = new shared Parent?();
  bar(a);
}
proc main() {
  foo();
}
