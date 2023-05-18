// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Parent to owned Child?
  var a = new unmanaged Parent();
  var a_:owned Child? = a;
}
proc main() {
  foo();
}
