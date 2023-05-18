// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned A? to shared A
  var a = new owned A?();
  var a_:shared A = a;
}
proc main() {
  foo();
}
