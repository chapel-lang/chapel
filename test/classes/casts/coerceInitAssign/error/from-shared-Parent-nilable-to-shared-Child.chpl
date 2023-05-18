// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Parent? to shared Child
  var a = new shared Parent?();
  var a_:shared Child = a;
}
proc main() {
  foo();
}
