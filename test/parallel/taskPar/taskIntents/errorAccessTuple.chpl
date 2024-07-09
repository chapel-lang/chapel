record R {
  var x: 2*int;
}

var r: R;
var a = (1,2);

// need to toggle between the cases, as the error messages won't all appear at once
config param test = 1;

begin {
  if test == 1 {
    r.x(1) += 1;
    r.x = (1,1);
    a = (2,3);
  }
  else {
    a(1) = 1;
  }
}
