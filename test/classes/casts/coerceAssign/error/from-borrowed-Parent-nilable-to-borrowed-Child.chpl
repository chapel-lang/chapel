// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed Parent? to borrowed Child
  var allocFrom = new owned Parent?();
  var a:borrowed Parent? = allocFrom;
  var allocTo = new owned Child();
  var a_:borrowed Child = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
