// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from shared Child? to borrowed Parent
  var allocFrom = new shared Child();
  var allocTo = new owned Parent();
  var a:shared Child? = allocFrom;
  var a_:borrowed Parent = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
