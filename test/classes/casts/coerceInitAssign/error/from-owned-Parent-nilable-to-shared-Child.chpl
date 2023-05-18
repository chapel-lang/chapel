// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Parent? to shared Child
  var a = new owned Parent?();
  var a_:shared Child = a;
}
proc main() {
  foo();
}
