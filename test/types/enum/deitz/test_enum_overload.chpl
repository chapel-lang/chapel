enum month {jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec};

proc foo(m : month) {
  writeln("It's a month");
}

proc foo(i : int) {
  writeln("It's an int");
}

var m : month = month.jan;
var i = m: int;

writeln("The first month is ", m, " or, as an int, ", i);
foo(m);
foo(i);
