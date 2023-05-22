// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned A? to owned A?
proc bar(inout x: owned A?) {
  x = new owned A?();
}
proc foo() {
  var a = new owned A?();
  bar(a);
}
proc main() {
  foo();
}
