// the snippet is from Paul Cassella's bug report #14783
record R {
  var x, y: int;
}

var small = new R(1, 1);
var mid1 = new R(1, 2);
var mid2 = new R(2, 1);
var big = new R(2, 2);


var Rs = [ small, mid1, mid2, big ];

for r1 in Rs {
  for r2 in Rs {
    writeln(r1, " == ", r2, ": ", r1 == r2);
    writeln(r1, " < ", r2, ": ", r1 < r2);
    writeln(r1, " <= ", r2, ": ", r1 <= r2);
    writeln(r1, " > ", r2, ": ", r1 >
        r2);
    writeln(r1, " >= ", r2, ": ",
        r1 >= r2);
    writeln(r1, " != ", r2,
        ": ", r1 != r2);
    writeln();
  }
}
