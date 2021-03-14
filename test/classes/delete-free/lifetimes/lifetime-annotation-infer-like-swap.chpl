class C { var x: int; }

var globOwn = new owned C(1);
var globly = globOwn.borrow();

proc swapit(ref lhs: borrowed C?, ref rhs: borrowed C?) lifetime lhs=rhs, rhs=lhs
{
  var tmp = lhs;
  lhs = rhs;
  rhs = tmp;
}

proc main() {
  var b: borrowed C?;
  {
    var own = new owned C(2);
    var bb: borrowed C? = own.borrow();
    var copy: borrowed C?;
    swapit(copy, bb); // Does this propagate lifetime?
    b = copy; // expecting an error here
  }
  writeln(b!.x);
}
