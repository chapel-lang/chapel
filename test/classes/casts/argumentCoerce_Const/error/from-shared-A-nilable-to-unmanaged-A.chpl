// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared A? to unmanaged A
proc bar(const x: unmanaged A) {}
proc foo() {
  var a = new shared A?();
  bar(a);
}
proc main() {
  foo();
}
