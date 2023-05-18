// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Child to owned Parent?
  var a = new unmanaged Child();
  var a_:owned Parent? = a;
}
proc main() {
  foo();
}
