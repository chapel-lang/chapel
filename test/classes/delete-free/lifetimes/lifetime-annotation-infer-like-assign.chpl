class C { var x: int; }

proc setit(ref lhs: borrowed C?, rhs: borrowed C) lifetime lhs=rhs
{
  lhs = rhs;
}

proc main() {
  var b: borrowed C?;
  {
    var own = new owned C(2);
    var bb = own.borrow();
    var copy: borrowed C?;
    setit(copy, bb); // Does this propagate lifetime?
    b = copy; // expecting an error here
  }
  writeln(b!.x);
}
