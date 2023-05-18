// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Child to owned Parent
  var alloc = new owned Child();
  var a:owned Child = alloc;
  var a_:owned Parent = a;
}
proc main() {
  foo();
}
