// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Child to borrowed Parent?
  var alloc = new owned Child();
  var a:owned Child = alloc;
  var a_:borrowed Parent? = a;
}
proc main() {
  foo();
}
