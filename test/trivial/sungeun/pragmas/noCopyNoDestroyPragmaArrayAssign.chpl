var a: [1..3] int;

pragma "no copy"
pragma "no auto destroy"
var b = a;
writeln(b);

