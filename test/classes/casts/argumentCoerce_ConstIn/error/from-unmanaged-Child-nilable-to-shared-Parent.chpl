// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from unmanaged Child? to shared Parent
proc bar(const in x: shared Parent) {}
proc foo() {
  var a = new unmanaged Child?();
  bar(a);
}
proc main() {
  foo();
}
