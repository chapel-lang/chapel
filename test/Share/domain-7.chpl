
var Cliques : domain(1);
var randomnums = (/ 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.10 /);
var cliqueSizes[Cliques] : integer;
cliqueSizes = ceil(maxCliqueSize*randomnums);
Cliques = 1..10;
var AdjDomain : domain [i in Cliques] * (2) =
  let k = cliqueSize(i) in (1..k, 1..k);

var maxParalEdges = 4;
var cliqueAdjMatrix[AdjDomain] =
    reshape(let n = AdjDomain.extent;
                in ceil(maxParalEdges*random_numbers.generate(n)));

[c in Cliques][i in 1..cliqueSize(c)] cliqueAdjMaxtrix(c,i,i) = 0;

var edgeDomain : domain Cliques * (1..maxParalEdges);
var edgeCounts [(c,i) in edgeDomain] =
                      sum ([m in cliqueAdjMatrix(c)] m >=i);
var edgeStarts [edgeDomain] = scan edgeCounts by + ;

numEdgesPlacedInCliques = edgeStarts(edgeDomain.last);

record EndPoints {
  var start : integer;
  var end : integer;
}

var intraEdges [1..numEdgesPlacedInCliques] : EndPpoints;

forall (c,m) in edgeDomain {
     var first = VsInClique(c).first;
     intraEdges[edgeStarts(c,m)..] =
         [(i,j) in AdjDomain(c)]
            (if(cliqueAdjMatrix(c,i,j) >= m)
                 then (start=i+first,end=j+first));
}
