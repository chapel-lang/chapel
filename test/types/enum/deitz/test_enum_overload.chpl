enum month {jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec};

def foo(m : month) {
  writeln("It's a month");
}

def foo(i : int) {
  writeln("It's an int");
}

var m : month = month.jan;
var i : int = m;

writeln("The first month is ", m, " or, as an int, ", i);
foo(m);
foo(i);
