var TableDomain = {0..10};
var Table: [TableDomain] uint(64);
var i:int;

[i in TableDomain with (ref Table)] Table(i) = i;
writeln(Table);

