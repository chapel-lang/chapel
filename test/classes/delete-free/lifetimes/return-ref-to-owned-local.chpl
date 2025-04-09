// Note: I was expecting a lifetime error for this code
// but it compiles without error.

class C { var x: int; }

proc getRefToVal() ref {
  // allocate a class instance on the heap containing an integer field
  var instance = new owned C(0);

  // create a reference the field 'x' on the heap
  ref refToVal = instance.x;

  // return the reference
  // note: the class instance is freed here before return!
  return refToVal;
}

proc main() {
  // create a reference the field 'x' on the heap
  ref refToVal = getRefToVal();

  refToVal = 42;

  // do other heap operations to make heap corruption more visible
  {
    var x = new owned C(2);
    var y = new owned C(3);
  }

  writeln(refToVal);
}
