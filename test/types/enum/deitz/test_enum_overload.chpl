enum month {jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec};

function foo(m : month) {
  writeln("It's a month");
}

function foo(i : integer) {
  writeln("It's an integer");
}

var m : month = jan;
var i : integer = m;

writeln("The first month is ", m, " or, as an integer, ", i);
foo(m);
foo(i);
