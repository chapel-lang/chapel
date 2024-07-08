record R {
  var x;
  var y: integral;
}

type t1 = R(x=real, y=int); // error: can't type construct with named argument for a value field
