// This test checks a warning that highlights a behavior
// change between 1.31. It's not actually a deprecation
// but the warning is transitional and can be removed
// like a deprecation.

class C { }

proc main() {
  var x = (new owned C()).borrow();
  var y = (new C()).borrow();
}
