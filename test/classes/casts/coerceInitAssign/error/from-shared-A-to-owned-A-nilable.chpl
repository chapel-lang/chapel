// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared A to owned A?
  var a = new shared A();
  var a_:owned A? = a;
}
proc main() {
  foo();
}
