// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Parent to unmanaged Child?
  var allocFrom = new unmanaged Parent();
  var allocTo = new unmanaged Child();
  var a:unmanaged Parent = allocFrom;
  var a_:unmanaged Child? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
