// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Child? to unmanaged Parent?
  var a = new shared Child?();
  var a_:unmanaged Parent? = a;
}
proc main() {
  foo();
}
