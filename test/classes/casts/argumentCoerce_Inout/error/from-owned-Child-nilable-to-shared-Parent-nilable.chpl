// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned Child? to shared Parent?
proc bar(inout x: shared Parent?) {
  x = new shared Parent?();
}
proc foo() {
  var a = new owned Child?();
  bar(a);
}
proc main() {
  foo();
}
