// AUTO-GENERATED: Do not edit
class A {}
class Parent {}
class Child : Parent {}
proc foo() {
  // coercing from owned Parent to owned Child?
  var alloc = new owned Parent();
  var a:owned Parent = alloc;
  var a_:owned Child? = a;
}
proc main() {
  foo();
}
