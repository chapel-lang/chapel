var D = {"a", "b", "c"};
var DD: domain([D] real);

var A1: [D] real;
A1["a"] = 1.2;
A1["b"] = 3.4;
A1["c"] = 5.6;

DD.add(A1);

writeln(DD);

var E: [DD] real;

E[A1] = 9.9;

writeln(E);

var A2 = ["a" => 10.0, "b" => 20.0, "c" => 30.0];
DD.add(A2);
E[A2] = 100.0;

DD.remove(A1);

writeln(E);
