// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from unmanaged Parent? to borrowed Child?
  var a = new unmanaged Parent?();
  var allocTo = new owned Child?();
  var a_:borrowed Child? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
