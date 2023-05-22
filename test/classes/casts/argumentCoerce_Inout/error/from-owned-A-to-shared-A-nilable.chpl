// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned A to shared A?
proc bar(inout x: shared A?) {
  x = new shared A?();
}
proc foo() {
  var a = new owned A();
  bar(a);
}
proc main() {
  foo();
}
