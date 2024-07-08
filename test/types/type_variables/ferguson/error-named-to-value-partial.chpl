record R {
  var x;
  var y: integral;
}

type t2 = R(y=int(8), ?); // error: can't type construct with named argument for a value field
