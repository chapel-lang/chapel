// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Child to unmanaged Parent?
  var allocFrom = new unmanaged Child();
  var allocTo = new unmanaged Parent();
  var a:unmanaged Child = allocFrom;
  var a_:unmanaged Parent? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
