// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed Child? to unmanaged Parent?
  var alloc = new owned Child();
  var a:borrowed Child? = alloc;
  var a_:unmanaged Parent? = a;
}
proc main() {
  foo();
}
