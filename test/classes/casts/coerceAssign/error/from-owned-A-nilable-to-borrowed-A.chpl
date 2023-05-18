// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned A? to borrowed A
  var allocFrom = new owned A();
  var allocTo = new owned A();
  var a:owned A? = allocFrom;
  var a_:borrowed A = allocTo;
  a_ = a;
}
proc main() {
  foo();
}
