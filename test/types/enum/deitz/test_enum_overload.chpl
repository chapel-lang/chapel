enum month {jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec};

function foo(m : month) {
  writeln("It's a month");
}

function foo(i : int) {
  writeln("It's an int");
}

var m : month = jan;
var i : int = m;

writeln("The first month is ", m, " or, as an int, ", i);
foo(m);
foo(i);
