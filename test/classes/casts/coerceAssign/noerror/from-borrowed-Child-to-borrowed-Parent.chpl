// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from borrowed Child to borrowed Parent
  var allocFrom = new owned Child();
  var allocTo = new owned Parent();
  var a:borrowed Child = allocFrom;
  var a_:borrowed Parent = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
