
var Cliques : domain(1);
var vsInClique[Cliques] : (first:integer, last:integer);
Cliques = 1..10;
vsInClique[4].first = 1;
Cliques = 2..9;
writeln("expect 8 ", Cliques.length); -- is this legal?  is this a sequence conversion?

