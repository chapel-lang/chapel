// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed Parent? to unmanaged Child?
  var allocFrom = new owned Parent();
  var allocTo = new unmanaged Child();
  var a:borrowed Parent? = allocFrom;
  var a_:unmanaged Child? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
