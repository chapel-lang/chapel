var TableSize: uint(64) = 10;
var TableDomain: domain(1) distributed(blockcyclic) = [0..TableSize);
var Table: [TableDomain] uint(64);

Table = 1;
writeln(Table);


