// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from shared Parent to owned Child?
proc bar(out x: owned Child?) {
  x = new owned Child?();
}
proc foo() {
  var a = new shared Parent();
  bar(a);
}
proc main() {
  foo();
}
