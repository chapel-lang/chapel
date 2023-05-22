// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned Parent to shared Child?
proc bar(ref x: shared Child?) {}
proc foo() {
  var a = new owned Parent();
  bar(a);
}
proc main() {
  foo();
}
