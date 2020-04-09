use printHelp;

var d : domain(string);
d.add("two");
d.add("seven");

var a : [d] real;

a("two") = 7.2;
a("seven") = 2.7;

writelnSortedByDom(a);

writeln(a("two"));
writeln(a("seven"));
