var People: domain(opaque);

var Names: [People] string;

var aperson : index(People) = People.create();
var another : index(People);

Names(aperson) = "Brad";
Names(another) = "Steve";

writeln(Names(aperson));
writeln(Names(another));
