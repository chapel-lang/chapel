
var Cliques : domain(1);
var maxCliqueSize = 10;
var cliqueSizes[Cliques] : integer;
Cliques = 1..10;
var randomnums = (/ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.10 /);
cliqueSizes = ceil(maxCliqueSize*randomnums);
var VsInClique[Cliques] : (first:integer, last:integer);
[i : 1..10] VsInClique(i) = (first = 1, last = 2);
var totCliques = 10;
var totVertices = 100;
VsInCliques.last(totCliques) = totVertices;
VsInClique.first = 1 # VsInCliques(1..totCliques-1).last + 1;

