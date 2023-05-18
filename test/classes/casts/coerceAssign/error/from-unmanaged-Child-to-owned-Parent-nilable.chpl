// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Child to owned Parent?
  var allocFrom = new unmanaged Child();
  var allocTo = new owned Parent();
  var a:unmanaged Child = allocFrom;
  var a_:owned Parent? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
