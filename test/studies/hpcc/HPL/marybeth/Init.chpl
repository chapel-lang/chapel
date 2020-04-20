use Random;
private use IO;

enum processmaptype {Row=0, Column};
enum factortype {Left=0, Crout, Right};
enum bcasttype {OneRG=0, OneRM, TwoRG, TwoRM, Lng, LnM};
enum swaptype {BinExchange=0, Long, Mix};
enum transtype {Transpose=0, NonTranspose};
enum equiltype {NoEquilibration=0, Equilibration};

record HPLparams {
  var inFileName = "HPL.dat";
 
  const epsil = 2.0e-16;
  var kpass, kfail: int;

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
     if numProbSizes < 1 then 
        stderr.writeln("HPL ERROR: Number of values of N is less than 1");
        else NDom = {1..numProbSizes};
     for n in N { 
       infile.read(n);
       if n < 0 then 
          stderr.writeln("HPL ERROR: Value of N less than 0");
     }
     infile.readln();

     infile.readln(numBlockSizes);
     if numBlockSizes < 1 then 
        stderr.writeln("HPL ERROR: Number of values of NB is less than 1");
        else NBDom = {1..numBlockSizes};
     for nb in NB { 
       infile.read(nb);
       if nb < 1 then 
          stderr.writeln("HPL ERROR: Value of NB less than 1");
     }
     infile.readln();

     pMap = infile.readln(int):processmaptype;

     infile.readln(numProcessGrids);
     if numProcessGrids < 1 then 
        stderr.writeln("HPL ERROR: Number of values of grids is less than 1");
        else GridDom = {1..numProcessGrids};
     for p in P {
       infile.read(p);
       if p < 1 then 
          stderr.writeln("HPL ERROR: Value of P less than 1");
     }  
     infile.readln();
     for q in Q {
       infile.read(q);
       if q < 1 then 
          stderr.writeln("HPL ERROR: Value of Q less than 1");
     }
     infile.readln();

     infile.readln(Thresh);

     infile.readln(numPanelFacts);
     if numPanelFacts < 1 then 
        stderr.writeln("HPL ERROR: Number of values of PFACT is less than 1");
        else PFactDom = {1..numPanelFacts};
     for pf in PFact do pf = infile.read(int):factortype;
     infile.readln();

     infile.readln(numRecurStops);
     if numRecurStops < 1 then 
        stderr.writeln("HPL ERROR: Number of values of NBMIN is less than 1");
        else NBMinDom = {1..numRecurStops};
     for nbmin in NBMin {
       infile.read(nbmin);
       if nbmin < 1 then 
          stderr.writeln("HPL ERROR: Value of NBMIN less than 1");
     }
     infile.readln();

     infile.readln(numPanels);
     if numPanels < 1 then 
        stderr.writeln("HPL ERROR: Number of values of NDIV is less than 1");
        else NDivDom = {1..numPanels};
     for nd in NDiv {
       infile.read(nd);
       if nd < 2 then 
          stderr.writeln("HPL ERROR: Value of NDIV less than 2");
     }
     infile.readln();

     infile.readln(numRecurPanelFacts);
     if numRecurPanelFacts < 1 then 
        stderr.writeln("HPL ERROR: Number of values of RFACT is less than 1");
        else RFactDom = {1..numRecurPanelFacts};
     for rf in RFact do rf = infile.read(int):factortype; 
     infile.readln();

     infile.readln(numBCasts);
     if numBCasts < 1 then 
        stderr.writeln("HPL ERROR: Number of values of BCAST is less than 1");
        else BCastDom = {1..numBCasts};
     for bc in BCast do bc = infile.read(int):bcasttype;
     infile.readln();
         
     infile.readln(numDepths);
     if numDepths < 1 then 
        stderr.writeln("HPL ERROR: Number of values of DEPTH is less than 1");
        else DepthDom = {1..numDepths};
     for d in Depth {
       d = infile.read(int);
       if d < 0 then 
          stderr.writeln("HPL ERROR: Value of DEPTH less than 0");
     }      
     infile.readln();

     Swap = infile.readln(int):swaptype;
     infile.readln(swapThresh);
     L1 = infile.readln(int):transtype;
     U = infile.readln(int): transtype;
     infile.readln(memAlign);
     infile.close();
  }
}

proc init(A:[?D]) {
//  Rather than initialize A = [Asquare | b] with one call to 
//  fillRandom, Asquare is initialized first, followed by b.
//  This is to match the previous version of benchmark where
//  A and b were stored separately. 
  var n = D.dim(0).size;
  ref Asquare = A(..,1..n);
  ref b = A(..,n+1);
  var rstream = new owned RandomStream(real, seed=1234567891);

  rstream.fillRandom(Asquare);
  rstream.fillRandom(b);
}
