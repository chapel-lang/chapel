// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from borrowed A to shared A?
proc bar(inout x: shared A?) {
  x = new shared A?();
}
proc foo() {
  var alloc = new owned A();
  var a:borrowed A = alloc;
  bar(a);
}
proc main() {
  foo();
}
