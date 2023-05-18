// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Parent? to borrowed Child?
  var a = new shared Parent?();
  var allocTo = new owned Child?();
  var a_:borrowed Child? = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
