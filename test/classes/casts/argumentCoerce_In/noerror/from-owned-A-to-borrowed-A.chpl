// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
// coercing from owned A to borrowed A
proc bar(in x: borrowed A) {}
proc foo() {
  var a = new owned A();
  bar(a);
}
proc main() {
  foo();
}
