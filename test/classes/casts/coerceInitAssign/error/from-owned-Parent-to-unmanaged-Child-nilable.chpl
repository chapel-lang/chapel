// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Parent to unmanaged Child?
  var a = new owned Parent();
  var a_:unmanaged Child? = a;
}
proc main() {
  foo();
}
