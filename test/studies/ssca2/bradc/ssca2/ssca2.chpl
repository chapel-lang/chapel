config var ENABLE_K2 = true;
config var ENABLE_K3 = true;
config var ENABLE_K4 = true;
config var DECOR_VERTICES = true;
config var ENABLE_PAUSE = false;
config var ENABLE_PLOTS = false;
config var ENABLE_VERIF = true;
config var ENABLE_MDUMP = false;
config var ENABLE_DEBUG = false;
config var FIGNUM = 0 ;
config var ENABLE_PLOT_ROSE  = false;
config var ENABLE_PLOT_COLOR = false;
config var ENABLE_PLOT_K3    = false;
config var ENABLE_PLOT_K3DB  = false;

union Weight {
  var i : int;
  var s : string;
  proc is_string {
    typeselect (this) {
      when s     return true;
      otherwise  return false;
    }
  }
}

record Numbers {
  var totVertices : int; 
  var maxParallelEdge : int;
  var numIntEdges : int; 
  var numStrEdges : int;
  var maxIntWeight : int;
}

record EndPoints { 
  var start : int;
  var end : int;
}

class Edges { 
  with Numbers;
  var Cliques : domain(1);
  var cliqueSizes : [Cliques] int; 
  var VsInClique : [Cliques] (first:int, last:int);
  var numEdgesPlaced  : int;
  var Edges : domain(1); // 1..numEdgesPlaced
  var edges : [Edges] record { 
                        with EndPoints;
                        var weight :Weight;
                      };
  var numEdgesPlacedInCliques : int;
  var numEdgesPlacedOutside   : int;
}

class Graph {
  with Numbers; 
  var VertexD  : domain(1);  // 1..totVertices
  var ParEdgeD : domain(1) ; // 1..maxParallelEdge

  // separate int and string subgraps that
  // share the above two domains
  var intg = new Subgraph(wtype=int,
                      VertexD=>VertexD,
                      ParEdgeD=>ParEdgeD);
  var strg = new Subgraph(wtype=string,
                      VertexD=>VertexD,
                      ParEdgeD=>ParEdgeD);

  proc copy(s : Graph) {
    return Graph(VerteD  =s.VertexD,
                 ParEdgeD=s.ParEdgeD);
  }
}

class Subgraph {
  type wtype;
  var VertexD : domain(1);
  var ParEdgeD : domain(1); 
  // sparse matrix index by directed vertex pairs
  var AdjD  : domain sparse (VertexD * VertexD) = nil;
  // holds count of edges between vertex pairs
  var weights : [AdjD] seq of wtype;

  constructor EndPoints.EndPoints ( (s,e) : index(AdjD)) {
    start = s;
    end   = e;
  }
  proc adjMatrix(i: index(AdjD)) { return weights(i).size; }
}


proc main() {
  // Scalable Data Generator parameters.
  // Total number of vertices in directed multigraph.
  config var TOT_VERTICES       =  2^8;
  //  Maximum allowed clique size in directed multigraph.
  config var MAX_CLIQUE_SIZE    =   10; 
  // Max num of parallel edges allowed between two vertices. 
  config var MAX_PARAL_EDGES    =    8; 
  // Percentage of int (vs. char string) edge weights.
  config var PERC_INT_WEIGHTS   =  0.6; 
  // Max allowed int value in any int edge weight.
  config var MAX_INT_WEIGHT     =  255;
  // Initial probability of a link between two cliques.
  config var PROB_INTERCL_EDGES =  0.5; 
  // Kernel parameters.
  // Kernel 3: Num of edges in each dim 
  // in directed subgraph. Valid range: 2 and up, 
  // though at some point it will recover entire graph.
  config var SUBGR_EDGE_LENGTH  =    3; 
  // Kernel 4: Clustering search box size.
  config var MAX_CLUSTER_SIZE   =   14; 
  // Kernel 4: cluster search region within MAX_CLUSTER_SIZE.
  config var ALPHA              =  0.5; 

  writeln('\mHPCS SSCA #2 Graph Analysis Executable Specification:');
  writeln('\nRunning...\n\n\n\n');
  var edges, SOUGHT_STRING;  
  writeln('\nScalable Data Generator - genScalData() beginning execution...\n');
  if ENABLE_VERIF then
    (edges, SOUGHT_STRING) = 
              genScalData( TOT_VERTICES, MAX_CLIQUE_SIZE, MAX_PARAL_EDGES, 
                           PERC_INT_WEIGHTS, MAX_INT_WEIGHT, 
                           PROB_INTERCL_EDGES );
  writeln('\n\tgenScalData() completed execution.\n');
  if ENABLE_VERIF then verifyGenData(edges);

  var G : Graph;
  if ENABLE_K1 {
    writeln('\nKernel 1 - computeGraph() beginning execution...\n');
    var startTime = clock;             // Start performance timing.
    G = computeGraph( edges, TOT_VERTICES, MAX_PARAL_EDGES, MAX_INT_WEIGHT );
    writeln('\n\tcomputeGraph() completed execution.\n');
    dispEllapsedTime( startTime );      // End performance timing.
    if ENABLE_VERIF then
      verifComputeGraph( edges, G );
  }

  var startSetInt, maxIntWeight, startSetStr;

  if ENABLE_K2 {
    writeln('\nKernel 2 - sortWeights() beginning execution...\n');
    var startTime = clock; // Start performance timing.
    
    (startSetInt, maxIntWeight, startSetStr) = sortWeights( G, SOUGHT_STRING );

    writeln('\n\tsortWeights() completed execution.\n');
    dispEllapsedTime( startTime ); // End performance timing.
    
    if ENABLE_VERIF then
      verifSortWeights(edges, startSetInt, maxIntWeight,
                       startSetStr, SOUGHT_STRING );
  }

  var subGraphs;
  if ENABLE_K3 {
    writeln('\nKernel 3 - findSubGraphs() beginning execution...\n');
    var startTime = clock; // Start performance timing.
    
    subGraphs = findSubGraphs( G, SUBGR_EDGE_LENGTH, 
                               startSetInt, 
                               startSetStr );
    writeln('\n\tfindSubGraphs() completed execution.\n');
    
    dispEllapsedTime( startTime ); // End performance timing.
    
    if ENABLE_VERIF then
      verifFindSubGraphs( G, subGraphs, startSetInt, startSetStr, 
                          SUBGR_EDGE_LENGTH );
  }

  if ENABLE_K4 {
    writeln('\nKernel 4 - cutClusters() beginning execution...\n');
    
    if ENABLE_VERIF then
      verPrCutClusters( G );
    var startTime = clock; //  Start performance timing.
    
    // Find clusters in the graph.
    var (cutG, intVertexRemap, strVertexRemap) 
          = cutClusters( G, MAX_CLUSTER_SIZE, ALPHA );
    
    writeln('\n\tcutClusters() completed execution.\n');
    
    dispEllapsedTime( startTime ); // End performance timing.
    
    if ENABLE_VERIF then
      verifCutClusters( cutG, intVertexRemap, strVertexRemap );
  }

  writeln('\nHPCS SSCA #2 Graph Analysis Executable Specification:');
  writeln('\nEnd of test.\n\n\n\n') ;
}


proc genScalData(totVertices, maxCliqueSize, maxParalEdges,
                     percentIntWeights, 
                     maxIntWeightP, probInterclEdges) {

  use Random only RandomNumbers;
  var random_numbers = RandomNumbers();
  var fedges = new Edges();
  use fedges;
  maxIntWeight = maxIntWeightP;

  // Estimate number of cliques needed and pad by 50%.
  var estTotCliques = 
        ceil( (1.5*totVertices) / ((1.0 + maxCliqueSize) / 2 ));
  Cliques = 1..estTotCliques;

  // Generate random clique sizes.
  cliqueSizes = 
    ceil(maxCliqueSize*random_numbers.generate(estTotCliques));

  // Sum up vertices in each clique.
  VsInCliques.last = sum scan cliqueSizes;

  // Find where this is greater than totVertices.
  var totCliques = binsearch(VsInCliquess.last, totVertices);

  // Truncate cliques to get the right number 
  Cliques = 1..totCliques;

  // Fix the size of the last clique.  
  cliqueSizes(totCliques) = totVertices - VsInCliques(totCliques-1).last;

  // Compute new start and end vertices in cliques.
  VsInCliques.last(totCliques)  = totVertices;
  VsInCliques.first = 1 # VsInCliques(1..totCliques-1).last + 1;


  // create the edges within the cliques
  var AdjDomain : domain [i in Cliques] * (2) =  
                    let k = cliqueSize(i) in (1..k, 1..k);

  // fill matrix with random numbers
  var cliqueAdjMatrix: [AdjDomain] = 
    reshape(let n = AdjDomain.extent; 
                in ceil(maxParalEdges*random_numbers.generate(n)));

  // zero out the diagonals 
  [c in Cliques][i in 1..cliqueSize(c)] cliqueAdjMaxtrix(c,i,i) = 0;

  // build cumulative sum of edge counts by clique and level
  var edgeDomain : domain Cliques * (1..maxParalEdges);
  var edgeCounts: [(c,i) in edgeDomain] =  
        sum ([m in cliqueAdjMatrix(c)] m >=i);
  var edgeStarts: [edgeDomain] = sum scan edgeCounts;

  numEdgesPlacedInCliques = edgeStarts(edgeDomain.last);

  // Initialize vertex arrays.
  var intraEdges : [1..numEdgesPlacedInCliques] EndPpoints;

  // now build tine intra-clique edges
  forall (c,m) in edgeDomain {
    var first = VsInClique(c).first;
    intraEdges[edgeStarts(c,m)..] = 
      [(i,j) in AdjDomain(c)] 
        (if (cliqueAdjMatrix(c,i,j) >= m)
         then (start=i+first, end=j+first));
  }

  // connect the cliques
  // build a map from vertex number to the clique it is in
  var toClique: [1..totalVertices];
  forall c in Cliques do
    toClique(VsInClique(c).first..VsInClique(c).last) = c;

  // the probability of an edge between two cliques
  // which is related to distance.
  var log2Dist = ceil(log(totVertices,2))-1;

  // foreach probability level, there are 'up' and 'down' edges
  var bitsDomain : domain(1..totalVertices, 1..log2Dist, -1..1 by 2);
  var bits : [bitsDomain] boole;
  forall ((ix, d, dir), r) in (bitsDomain ,
                               reshape(random_numbers.generate(bitsDomain.extent),
                                       bitDomain)) {
    if (r <= probInterclEdges**d) {
      var jx = (ix-1 + dir*2**d % totVertices) + 1;
      bits(ix,d,dir) = toClique(ix) != toClique(jx);
    } else bits(ix,d,dir) = false;
  }

  var offset: [bitsDomain] = sum scan (if bits then 1 else 0);
  numPlacedOutside = offset(bitsDomain.last);
  var interEdges: [1..numPlacedOutside] EndPoints;
  forall (ix, d, dir) in bitsDomain do
    if (bits(ix,d, dir)) {
      var jx = (ix-1 + dir*2**d % totVertices) + 1;
      interEdges(offset(ix,d,dir)) = (start=ix, end=jx);
    }

  // Compute the final number of edges.
  numEdgesPlaced = numPlacedInCliques + numPlacedOutside;
  Edges = 1..numEdgesPlaced;

  edges:EndPoints = intraEdges # interEdges;


  var sought_string : string;

  var strAlphabet: [1..26**2] = 
        reshape([i in 'A'..'Z'][j in 'A'..'Z'] i+j);
  var maxLenAlphabet = strAlphabet.extent;
  var is_str : [Edges] boole;
  forall (e, r1, r2) in (Edges,
                         random_numbers.generate(Edges.extent),
                         random_numbers.generate(Edges.extent)) {
                      
    if (r1 <= percentIntWeights) {
      edges(e).weight.i = ceil(maxIntWeight*r2); 
      is_str(e) = false;
    } else {
      edges(e).weight.s = strAlphabet(ceil(maxLenAlphabet*r2));
      is_str(e) = true;
    }
  }

  var i  = ceil(count(is_str) * random_numbers.next);
  if (i == 0) 
    then halt("no strings");
  var j = ([e in Edges] (if is_str(e) then e)) (i);
  var sought_string;
  typeselect (w = edges(j).weight) {
    when s    sought_string = w;
  }

  return (fedges, sought_string);
}


proc binsearch(x : [?lo..?hi] , y]) {
  if (hi < lo  ) then return lo;
  if (x(hi) > y) then return hi;
  if (y <= x(lo)) then return lo;

  while (lo+1 < hi) {
    assert(x(lo) < y && y <= x(hi));

    var mid = (hi+lo)/2;
    if (x(mid) < y) then
      lo = mid;
    else
      hi = mid;
  }
  return hi;
}

proc computeGraph(edges , totVertices, maxParalEdges, 
                      maxIntWeight ) : Graph {
  var G = new Graph();
  G:Numbers = edges;

  use G only VertexD, ParEdgeD, intg, strg;

  VertexD = 1..totVertices;
  ParEdgeD = 1..maxParalEdge;
  intg.AdjD = [e in edges.edges] (if !e.weight.is_string?
                                  then (e.start, e.end));
  strg.AdjD = [e in edges.edges] (if e.weight.is_string?
                                  then (e.start, e.end));
  forall e in edges.edges do
    typeselect (s = e.weight) {
      when i atomic intg.weights(e.start,e.end) #= s;
      when s atomic strg.weights(e.start,e.end) #= s;
    }

  return G;
}


proc sortWeights( G : Graph, soughtString : string ) {

  proc Subgraph.choose(value) {
    return [e in AdjD] (if (weights(e) == value) then EndPoints(e));
  }
  use G only intg, strg;
  var maxWeight = max(intg.weights);
  return (intg.choose(maxWeight), maxWeight, strg.choose(soughtString));
}


proc Graph.findSubGraphs(SUBGR_EDGE_LENGTH : int,
                             startSetIntVPairs : seq of EndPoints,
                             startSetStrVPairs : seq of EndPoints) 
                            : seq of Graph {
    
  proc Subgraph.expandSubGraphs(start, complete:subgraph) {
    var frontier like AdjD = (start.start, start.end);
    AdjD = start;
    for k in 2..SUBGR_EDGE_LENGTH {
      frontier = [(_,e) in frontier] complete.AdjD(e,*);
      frontier -= AdjD;
      AdjD += frontier;
    }
    [e in AdjD] weights(e) =  complete.weights(e);
  }

  var subgraphs: [1..(startSetIntVPairs.size+startSetStrVPairs.size)];
  // Loop over vertex pairs in the int starting set.
  cobegin {
    forall (i,v) in (1.., startSetIntVPairs) {
      subgraphs(i) = copy(this);
      subgraphs(i).intg.expand_subgraph(v, intg);
    }
    forall (i,v) in (startSetIntVPairs+1.., startSetStrVPairs); {
      subgraphs(i) = copy(this);
      subgraphs(i).strg.expand_subgraph(v, strg);
    }
  }
  return subgraphs;
}

proc cutClusters(G, cutBoxSize, alpha) {

  proc cutClustersCommon( adjMatrix : Subgraph,
                              cutBoxSize, alpha) {
    if cutBoxSize < 1
      then halt('cutBoxSize must be a least one.');
    if alpha < 0 || alpha > 1
      then halt('alpha must be between 0 and 1 inclusive.');
    var startSearch = ceil(alpha * cutBoxSize); 

    use AdjMatrix only VertexD, AdjD;
    var adjCounts: [v in VertexD] = length(AdjD(v,*) # AdjD(*,v));
     type Set : domain sparse VertexD = nil;
    type Seq : seq of index of VertexD = nil;

    var setG : Set = VertexD;
    var setClusters : Seq;     // set of nodes in clusters.
    var setN2 : Seq;           // set of cut nodes between clusters.

    while (setG.size > 0) {
      var setIter :Seq;   // candidate verticies in cluster
      var iCut = 0;       // index of best cutting point so far
      var iAdj :Set;      // snapshot of setAdj at iCut

      {
        var setAdj :Set;    // vertices adjacent to nodes in setIetr
        var vMin = setG(minloc(adjCounts(setG)));
        var cnCut = totVertices+1;   // minimum contour number so far.
        for i in 1..cutBoxSize {
          setIter #= vMin;         // add vMin to the iterating set.
    
          // Find the sets of vertices that are adjacent to vMin.
          // Form setAdj by excluding the already accounted for vertices.
          setAdj += AdjD(vMin,*) # AdjD(*,vMin);
          setAdj -= setIter # setN2;

          // Evaluate this vertex as a cutting point.
          var cn = setAdj.extent;
          if i >= startSearch && cnCut >= cn {
            cnCut = cn;
            iCut = i;
            iAdj = setAdj;
          }
          if (cn == 0) then
            break;            // The cluster is complete.

          // Pick the next vertex to be processed from among the adjacent 
          // vertices, the one which minimizes the adjacency count.
          var count: [ setAdj ] ;
          forall v in setAdj {
            // Find the sets of vertices that are adjacent to v.
            var vAdj like setAdj = setAdj # AdjD(v,*) # AdjD(*,v);
            vAdj -= setIter # setN2;
            count(v) = vAdj.extent;
          }
          vMin = minloc(count);
        }

        if iCut == 0 {                      // If no cutting point,
          iCut = cutBoxSize;              // use the entire cutBox.
          iAdj = setAdj;
        }
      }

      setClusters #= setIter(1..iCut);
      setN2       #= iAdj;
      setG  -= setIter # iAdj;
    }
    return setClusters # setN2;
  }

  var intVertexRemap = cutClustersCommon( G.intg, cutBoxSize, alpha );
  var strVertexRemap = cutClustersCommon( G.strg, cutBoxSize, alpha );
  var cutG = new Graph.copy(G);

  proc remap(oldg, newg, vertexRemap) {
    var map: [G.VertexD];
    forall (new, old) in (1.., vertexRemap) do 
      map(old) = new;
    newg.AdjD = [ (i,j) in oldg.Adj ] (map(i), map(j));
    
    forall (i,j) in newg.AdjD do
      newg.weights(i,j) = oldg.weights(vertexRemap(i), vertexRemap(j));
  }
  remap(G.intg, cutG.intg, intVertexRemap);
  remap(G.strg, cutG.strg, strVertexRemap);

  return cutG;
}
