
use LinkedLists;

config var filename="graph.dat";

/*
 * A simple vertex class
 */
enum colors { WHITE, GRAY, BLACK };
enum edgeKind { GRAPH, TREE, CROSS, BACK, SELF };

class position{
  var x: int;
  var y: int;
}

class Node {
  var id: int;
  var name: string;
  var pos: unmanaged position = new unmanaged position(-1,-1);
  var color : colors = colors.WHITE;
  var pred : unmanaged Node? = nil;
  var disc,fini : int = -1;

  proc writeThis(w){
    w.write("{",name,"}");
  }

  proc writeFancy(w){
    w.write("{",name," : ",color);
    if(pred != nil) then w.write("^",pred.name);
    if(disc >= 0) then w.write(" ",disc);
    if(fini >= 0) then w.write(",",fini);
    w.write("}");
  }
}

proc <(x: borrowed Node, y: borrowed Node) {
  return x.name < y.name;
}

proc <(x: borrowed Node?, y: borrowed Node?) {
  return x!.name < y!.name;
}

class Edge {
  var id: int;
  var src: unmanaged Node;
  var dst: unmanaged Node;
  var reversed: bool = false;
  var kind: edgeKind = edgeKind.GRAPH;
  proc writeThis(w){
    var ec:string;
    select(kind) {
    when edgeKind.GRAPH do ec = "--->";
    when edgeKind.TREE do ec = "===>";
    when edgeKind.CROSS do ec = "+++>";
    when edgeKind.SELF do ec = "self";
    when edgeKind.BACK {
      if(reversed){
	ec = "<---";
      } else {
	  ec = "--->";
	}
      }
      otherwise halt("invalid edge kind",kind);
    }
    w.write(src,ec,dst);
  }
  iter these() {
    if( src != nil) then yield src;
    if( dst != nil) then yield dst;
  }
}

proc <(x: borrowed Edge, y: borrowed Edge) {
  if (x.src.name == y.src.name) {
    return (x.dst.name < y.dst.name);
  } else {
    return (x.src.name < y.src.name);
  }
}

proc <(x: borrowed Edge?, y: borrowed Edge?) {
  return x! < y!;
}

class MultiMap {
  type keyType;
  type eltType;

  const keyD : domain(keyType);
  var keySpD : sparse subdomain(keyD);

  var bins : [keySpD] LinkedList(eltType);

  proc add( key: keyType, d: eltType ) : bool {
    var list_head=nil;
    if(keySpD.contains(key) == false){
      keySpD.add(key);
      list_head = LinkedList(eltType);
    } else {
      list_head = bins[key];
      for el in list_head do {
	  if (el == d) then return false;
      }
    }
    list_head.append(d);
    return true;
  }

  /* list of elements mapping to
   * the specific key
   */
  proc get( key: keyType ) : LinkedList(eltType) {
    var list_head = LinkedList(eltType);

    if(keySpD.contains(key)){
      list_head = bins[key];
      return list_head;
    }
  }
}

/*
proc file.read(inout val: Edge){
  var s,a,d: string;
  this.read(s,a,d);

  if( a != "->" ){
     halt("Expecting directed graphs!");
  }
  val = new Edge(new Node(s),new Node(d));
  return val;
}

proc Edge.read(infile: file){
  var s,a,d: string;
  file.read(s,a,d);

  if( a != "->" ){
     halt("Expecting directed graphs!");
  }
  return Edge(new Node(s),new Node(d));
}
*/

class UndirectedEdge : Edge {
  proc writeThis(w){
     w.write(src," -- ",dst);
  }
}

class Graph {

  proc init(nd:domain(1),ed:domain(1), ns, es){
    NodeDom = nd;
    EdgeDom = ed;
    nodes = ns;
    edges = es;
    this.complete();
    writeln("Graph constructor");
    preprocess();
  }

  const NodeDom, EdgeDom : domain(1); //= [1..NumNodes];
  //  const EdgeDom : domain(1) = [1..NumEdges];
  var nodes : [NodeDom] unmanaged Node?;
  var edges : [EdgeDom] unmanaged Edge?;

  var inEdges : [NodeDom] LinkedList(unmanaged Edge);
  proc inEdges(n:borrowed Node) ref { return inEdges[n.id]; }
  proc inEdges(i:index(NodeDom)) ref { return inEdges[i]; }

  var outEdges : [NodeDom] LinkedList(unmanaged Edge);
  proc outEdges(n:borrowed Node) ref { return outEdges[n.id]; }
  proc outEdges(i:index(NodeDom)) ref { return outEdges[i]; }



  var adjacent : [NodeDom] LinkedList(int);
  var undir_adjacent: [NodeDom] LinkedList(int);

  var __slack : [EdgeDom] int = -1;
  proc slack(e:borrowed Edge) ref { return slack[e.id]; }
  proc slack(i:index(EdgeDom)) ref { return __slack[i]; }

  var __length: [EdgeDom] int = 1;
  proc length(e:borrowed Edge) ref { return length[e.id]; }
  proc length(i:index(EdgeDom)) ref { return __length[i]; }

  var __treeEdges : [EdgeDom] int = 0;
  proc treeEdges(e:borrowed Edge) ref { return treeEdges[e.id]; }
  proc treeEdges(i:index(EdgeDom)) ref { return __treeEdges[i]; }

  var __curRank : [NodeDom] int = -1;
  proc curRank(n:borrowed Node) ref { return curRank[n.id]; }
  proc curRank(i:index(NodeDom)) ref { return __curRank[i]; }

  var __treeNodes : [NodeDom] int = 0;
  proc treeNodes(n:borrowed Node) ref { return treeNodes[n.id]; }
  proc treeNodes(i:index(NodeDom)) ref { return __treeNodes[i]; }


  // Graph variables
  var nodeSep : int = 1;
  var rankSep : int = 1;

  var weights : [EdgeDom] int = 1;

  var time : int = 1;

  /* Populate index structures in parallel */
  proc preprocess(){
    forall n in nodes do {
      preprocess(n!);
    }

    writeln("preprocess done!");
    for n in nodes do {
	writeln(n!.name," :in[",inEdges[n!.id],"] :out[",outEdges[n!.id],"]");
      }
  }

  proc preprocess(n : borrowed Node){
    for edge in edges do {
      var e = edge!;
      if( e.src == n ){
	outEdges[n.id].append(e);
	adjacent[n.id].append(e.dst.id);
      }
      if( e.dst == n ){
	inEdges[n.id].append(e);
	undir_adjacent[n.id].append(e.src.id);
      }
    }
  }


  proc init_rank(){
    writeln("**************");
    writeln("* init_rank  *");
    writeln("**************");
    // for all nodes, if not ranked, rank it 1
    for node in nodes do {
      var u = node!; 
      var uId = u.id;
      var r = curRank(uId);

      if (u.pred == nil) {
        curRank(uId) = 1;
        r = 1;
        writeln("Init rank for ",u," to ",curRank(uId));
        propagate_rank(u);
      }
    }
  }

  proc propagate_rank(u:borrowed Node) {
    var r = curRank(u.id);
    for edge in inEdges[u.id] do {
      var e = edge!;
      var v = e.src.id;
      if(v != u.id){
	if(curRank(v) == -1){
	  curRank(v) = r - 1; // it points to me
    var nd = nodes(v)!;
	  writeln("\t",nd.name," of rank ",r - 1,"\n\t propagate_rank(",nd.name,")");
	  propagate_rank(nd);
	} else if(curRank(v) > r){
	  halt("Edge from ",e.src," to ",u," but ranks are ",curRank(v)," -> ",r);
	}
      }
    }
    for e in outEdges[u.id] do {
      var v = e.dst.id;
      if(v != u.id){
	if(curRank(v) == -1){
	  curRank(v) = r + 1; // it points to me
    var nd = nodes(v)!;
	  writeln("\t",nd.name," of rank ",r+1,"\n\t propagate_rank(",nd.name,")");
	  propagate_rank(nd);
	} else if(curRank(v) < r){
	  halt("Edge from ",u," to ",e.dst," but ranks are ",r," -> ",curRank(v));
	}
      }
    }
  }

  proc recompute_slack(){
    forall edge in edges do{
      var e = edge!;
      if(e.kind == edgeKind.BACK){
	slack(e.id) = curRank(e.src.id) - curRank(e.dst.id);
      } else {
	slack(e.id) = curRank(e.dst.id) - curRank(e.src.id);
      }
    }
  }

  // Network Simplex for Rank assignment
  proc rank(){
    /*     feasible_tree();
     var edge: edgeType = leave_tree();
     for e in leave_edge() do {
       var f = enter_edge(e);
       exchange(e,f);
     }
     normalize();
     balance();
    */
  }

  iter non_tree_inEdges(id:index(NodeDom)) : unmanaged Edge {
    for edge in inEdges[id] do {
	if ((slack[edge] == length[edge]) && treeEdges[edge] == 0){
	  yield edge;
	}
      }
  }

  iter non_tree_outEdges(id:index(NodeDom)) : unmanaged Edge {
    for edge in outEdges[id] do {
	if ((slack[edge] == length[edge]) && treeEdges[edge] == 0){
	  yield edge;
	}
      }
  }

  iter tight_inEdges(id:index(NodeDom)): unmanaged Edge {
    for edge in inEdges[id] do {
	if(slack[edge] == length[edge]){
	  yield edge;
	}
      }
  }

  iter tight_outEdges(id:index(NodeDom)): unmanaged Edge {
    for edge in outEdges[id] do {
	if(slack[edge] == length[edge]){
	  yield edge;
	}
      }
  }

  proc tight_tree(nID:index(NodeDom)=1):int{
    __treeEdges = 0;
    __treeNodes = 0;
    treeNodes(nID) = 1;
    writeln("Finding max tight-tree containing ",nodes(nID)!.name);
    var cnt = find_tight_tree(nID);
    //    writeln("tight_tree ",nodes(nID)," has ",cnt," edges ");
    return + reduce [i in NodeDom] treeNodes(i);
  }

  proc find_tight_tree(nID:index(NodeDom)):int {
    var cnt = 0;
    var node: unmanaged Node = nodes(nID)!;
    treeNodes(nID) = 1;

    for edge in non_tree_inEdges(nID) do {
	const srcID = edge.src.id;
	treeEdges[edge] = 1;
	//	writeln("Adding tight edges ",edge," to tight tree ");
	cnt += 1; // we found the src node
	if(treeNodes(srcID) == 0){
	  cnt += find_tight_tree(srcID);
	}
      }
    for edge in non_tree_outEdges(nID) do {
	const dstID = edge.dst.id;
	treeEdges(edge) = 1;
	//	writeln("Adding tight edges ",edge," to tight tree ");
	cnt += 1; // we found the dst node
	if(treeNodes(dstID) == 0){
	  cnt += find_tight_tree(dstID);
	}
      }
    if(cnt == 0){
      // This node is a leave in the feasible tree, so mark that
      cnt = 1;
      treeNodes(nID) = 2;
    }
    //    writeln("find_tight_tree(",nodes(nID),") = ",cnt);
    return cnt;
  }

  iter incident_non_tree_edge() : unmanaged Edge {
    for edge in edges do {
      var e = edge!;
	if((treeEdges[e.id] == 0) &&
	   ((treeNodes(e.src.id) == 0 && treeNodes(e.dst.id) != 0) ||
	    (treeNodes(e.dst.id)) == 0 && treeNodes(e.src.id) != 0)){
	  writeln("Edge ",e," is non-tree, but incident on the tree ");
	  yield e;
	}
      }
  }

  // Compute a feasible tree for the graph
  proc feasible_tree(){
    writeln("*******************");
    writeln("* feasible_tree() *");
    writeln("*******************");

    init_rank();
    recompute_slack();

    writeln(" Prepare to find a tight tree ");
    __treeEdges = 0;
    __treeNodes = false;
    var treeSize = tight_tree();
    writeln("treeSize == ",treeSize);
    while( treeSize < NodeDom.size ) do {
	/* tight_tree() will set booleans in treeEdges and treeNodes
	 * From the non-treeEdges, we must find one with minimum slack
	 */
	var delta = 256;
	var minEdge: unmanaged Edge? = nil;
	for e in incident_non_tree_edge() do {
	    if(slack(e.id) < delta){
	      delta = slack(e.id);
	      minEdge = e;
	    }
	  }
	if(minEdge == nil){
	  halt("Couldn't find an edge to add!!");
	}

	writeln("In Loop: delta = ",delta," edge to add = ",minEdge);
	/* if the head is the incident node, then reverse the slack */
	if (treeNodes(minEdge!.src.id)) then delta = -delta;


	/* Try to promote across treeNodes */
	for v in NodeDom do {
	    curRank(v) += (delta * treeNodes(v));
	  }
	recompute_slack();
	for edge in edges do {
    var e = edge!;
	    writeln(e.src," @R[[",curRank(e.src.id),"]] --> ",e.dst,"@R[[",curRank(e.dst.id),"]] == ",slack(e.id));
	  }
	/* Try and tighten from the incident edge */
	treeSize = tight_tree(minEdge!.src.id);
      }
    writeln("Feasible tree computed!");
    //    init_cutvalues();
  }


  iter nodeEdges(u:unmanaged Node) : unmanaged Edge {
    yield inEdges[u.id];
    yield outEdges[u.id];
  }


  iter adjacentNodes(u:unmanaged Node): unmanaged Node {
    for edge in inEdges[u.id] do {
      if(edge.kind == edgeKind.BACK){
	yield edge.src;
      }
    }
    for edge in outEdges[u.id] do {
	if(edge.kind != edgeKind.BACK){
	  yield edge.dst;
	}
      }
  }

  iter edgesOut(u:unmanaged Node) : unmanaged Edge {
    for edge in edges do {
      var e = edge!;
      if ((e.src == u && !e.reversed) || (e.dst == u && e.reversed)){
	yield e;
      }
    }
  }

  iter whiteNodes() : unmanaged Node {
    for n in nodes do {
      if( n.color == colors.WHITE) then yield n;
    }
  }
}

proc DFS(G){
  G.time = 0;
  for node in G.nodes do {
    var u = node!;
      if( u.color == colors.WHITE ) {
	writeln("Starting at ",u);
	DFS_VISIT(G,u);
      }
    }
}

  proc DFS_VISIT(G:unmanaged Graph, u:unmanaged Node){
    u.color = colors.GRAY;
    G.time += 1;
    u.disc = G.time;
    for e in G.edgesOut(u) do {
	writeln("\t",e);
	var v = e.dst;
	if(v.color == colors.WHITE){
	  v.pred = u;
	  e.kind = edgeKind.TREE;
	  writeln("\tVisiting ",v);
          DFS_VISIT(G,v);
	} else if (v.color == colors.GRAY){
	  e.kind = edgeKind.BACK;
	  e.reversed = true;
	  writeln("Found back edge ", e);
	} else if (v.color == colors.BLACK){
	  e.kind = edgeKind.CROSS;
	  writeln("Found cross edge ", e);
	}
    }
    u.color = colors.BLACK;
    u.fini = G.time;
    G.time += 1;
  }


//
// This function reads a new graph returns
//
proc readGraph(filename) {
  use IO;

  // Create and open  an input file with the specified filename in read (iomode.r) mode
  var infile = open(filename, iomode.r);
  var reader = infile.reader();

  // Read the number of nodes and edges
  //  var n,m:int;
  //  reader.read(n,m);

  var ND: domain(string);
  var ED: domain((string,string));
  var NameMap: [ND] unmanaged Node?;
  var EdgeMap: [ED] unmanaged Edge?;
  //
  // Read in the edge statements
  //
  var i = 0,j = 1;
  var s,arrow,d : string;
  var edges = reader.read(int);

  for i in 1..edges do{
    reader.read(s,arrow,d);

    // Create a Node for the source if needed
    if(!ND.contains(s)){
      writeln("New source node ",s);
      ND.add(s);
      NameMap(s) = new unmanaged Node(j,s);
      j+=1;
    }

    // Create a Node for the dest if needed
    if(!ND.contains(d)){
      writeln("New dest node ",d);
      ND.add(d);
      NameMap(d) = new unmanaged Node(j,d);
      j+=1;
    }

    // Create an Edge if needed
    if(!ED.contains((s,d))){
      ED.add((s,d));
      EdgeMap((s,d)) = new unmanaged Edge(i,NameMap(s)!,NameMap(d)!);
      writeln("Added edge ",s," ",arrow," ",d);
    } else {
      halt("Duplicate edge ",s," ",arrow," ",d);
    }
  }

  // Declare a domain
  var N: domain(1) = {1..ND.size};
  var E: domain(1) = {1..ED.size};

  var X: [N] unmanaged Node? = NameMap.sorted();
  var Y: [E] unmanaged Edge? = EdgeMap.sorted();

  [ i in N ] X(i)!.id = i;
  [ i in E ] Y(i)!.id = i;

  writeln(ND.sorted());
  writeln(NameMap.sorted());
  writeln("Y = ",Y);

  reader.close();
  infile.close();

  // Return the Graph
  return new unmanaged Graph(N,E,X,Y);
}


//
// the main routine builds the tree and then sums its values
//
proc main() {

  writeln("Reading graph from ",filename);

  var G:unmanaged Graph = readGraph(filename);

  G.preprocess();

  writeln("Graph is:");

  for n in G.nodes {
      writeln(n);
  }
  for e in G.edges {
    writeln(e);
  }

  writeln("Running DFS");

  DFS(G);


  for i in G.NodeDom do
    writeln(G.nodes(i)," points to ",G.adjacent(i));

  G.feasible_tree();
}
