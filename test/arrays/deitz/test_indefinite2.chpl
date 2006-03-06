var d : domain(string);
d.add("two");
d.add("seven");

var a : [d] float;

a("two") = 7.2;
a("seven") = 2.7;

writeln(a);

writeln(a("two"));
writeln(a("seven"));
