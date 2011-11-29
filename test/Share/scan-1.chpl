
var Cliques : domain(1);
var maxCliqueSize = 10;
var cliqueSizes[Cliques] : int;
Cliques = 1..10;
var randomnums = (/ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.10 /);
cliqueSizes = ceil(maxCliqueSize*randomnums);
var vsInClique[Cliques] : (first:int, last:int);
VsInCliques.last = scan cliqueSizes by +;

