var a: [1..3] int;

pragma "no copy"
pragma "no auto destroy"
ref b = a;
writeln(b);

