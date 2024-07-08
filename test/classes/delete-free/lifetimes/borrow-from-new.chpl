// This test checks a warning that highlights a behavior
// change between 1.31 and 1.32. These should all produce
// lifetime errors but do not today.

class C { }

proc f(arg) { return arg; }

proc main() {
  var x = (new owned C()).borrow();
  const y = (new C()).borrow();
  const z = f((new C()).borrow());
}
