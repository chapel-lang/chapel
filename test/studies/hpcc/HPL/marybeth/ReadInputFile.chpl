enum processmaptype {Row=0, Column};
enum factortype {Left=0, Crout, Right};
enum bcasttype {OneRG=0, OneRM, TwoRG, TwoRM, Lng, LnM};
enum swaptype {BinExchange=0, Long, Mix};
enum transtype {Transpose=0, NonTranspose};
enum equiltype {NoEquilibration=0, Equilibration};

record HPLparams {
  var inFileName = "HPL.dat";
 
  var outFileName: string,
        deviceOut: int;

  var numProbSizes: int;
  var NDom: domain(1); 
  var N: [NDom] int;

  var numBlockSizes: int;
  var NBDom: domain(1);
  var NB: [NBDom] int;

  var pMap: processmaptype;
  var numProcessGrids: int;
  var GridDom: domain(1);
  var P: [GridDom] int;
  var Q: [GridDom] int;

  var Thresh: real;

  var numPanelFacts: int;
  var PFactDom: domain(1);
  var PFact: [PFactDom] factortype;

  var numRecurStops: int;
  var NBMinDom: domain(1);
  var NBMin: [NBMinDom] int;

  var numPanels: int;
  var NDivDom: domain(1);
  var NDiv: [NDivDom] int;

  var numRecurPanelFacts: int;
  var RFactDom: domain(1);
  var RFact: [RFactDom] factortype; 

  var numBCasts: int;
  var BCastDom: domain(1);
  var BCast: [BCastDom] bcasttype;

  var numDepths: int;
  var DepthDom: domain(1);
  var Depth: [DepthDom] int;
        
  var Swap: swaptype;
  var swapThresh: int;
  var L1, U: transtype;
  var memAlign: int;

  proc postinit() {
     var infile = open(inFileName, iomode.r).reader();

     infile.readln();
     infile.readln();
     infile.readln(outFileName);
     infile.readln(deviceOut);
     infile.readln(numProbSizes);
     NDom = {1..numProbSizes};
     for n in N do infile.read(n);
     infile.readln();

     infile.readln(numBlockSizes);
     NBDom = {1..numBlockSizes};
     for nb in NB do infile.read(nb);
     infile.readln();

     pMap = infile.readln(int):processmaptype;

     infile.readln(numProcessGrids);
     GridDom = {1..numProcessGrids};
     for p in P do infile.read(p);
     infile.readln();
     for q in Q do infile.read(q);
     infile.readln();

     infile.readln(Thresh);

     infile.readln(numPanelFacts);
     PFactDom = {1..numPanelFacts};
     for pf in PFact do pf = infile.read(int):factortype;
     infile.readln();

     infile.readln(numRecurStops);
     NBMinDom = {1..numRecurStops};
     for nbmin in NBMin do infile.read(nbmin);
     infile.readln();

     infile.readln(numPanels);
     NDivDom = {1..numPanels};
     for nd in NDiv do infile.read(nd);
     infile.readln();

     infile.readln(numRecurPanelFacts);
     RFactDom = {1..numRecurPanelFacts};
     for rf in RFact do rf = infile.read(int):factortype; 
     infile.readln();

     infile.readln(numBCasts);
     BCastDom = {1..numBCasts};
     for bc in BCast do bc = infile.read(int):bcasttype;
     infile.readln();
         
     infile.readln(numDepths);
     DepthDom = {1..numDepths};
     for d in Depth do d = infile.read(int);
     infile.readln();

     Swap = infile.readln(int):swaptype;
     infile.readln(swapThresh);
     L1 = infile.readln(int):transtype;
     U = infile.readln(int): transtype;
     infile.readln(memAlign);
     infile.close();
  }
}

