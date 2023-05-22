// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned Parent? to owned Child?
proc bar(const in x: owned Child?) {}
proc foo() {
  var a = new owned Parent?();
  bar(a);
}
proc main() {
  foo();
}
