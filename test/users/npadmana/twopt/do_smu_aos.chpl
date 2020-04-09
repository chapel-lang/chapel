use Regexp;
use Histogram;
use Time;
use Random;
use IO;

// Use the test/twopt code
config const isTest=false;
config const isPerf=false;
config const doBrute=false;

// Performance test parameters
config const nParticles=10000;

config const fn1 = "test.dat";
config const fn2 = "test.dat";
config const pairfn = "test-DD.dat";

param NDIM  = 3;
const Ddim = {1.. #NDIM};

config const spaces=compile("\\s+");
config const bufsize=10000;
config const minpart=500;

config const smax=200.0;
config const smax2=smax**2;
config const nmubins=5;
config const nsbins=5;

// Global variables
var gtime1 : Timer;

proc main() {
  doPairs();
}

record WeightedParticle3D {
  var x : NDIM*real;
  var w : real;
  var r2 : real;
}

proc generateRandom(pp : []WeightedParticle3D) {
  var x,y,z : real;
  var rng = new RandomStream(eltType=real);
  for ip in pp {
    x = rng.getNext()*1000.0; y = rng.getNext()*1000.0; z=rng.getNext()*1000.0;
    ip.x = (x,y,z);
    ip.w = 1.0;
    ip.r2 = x**2 + y**2 + z**2;
  }
}


proc countLines(fn : string) : int {
  var ff = open(fn, iomode.r);
  var ipart = 0;
  for iff in ff.lines() do ipart +=1;
  ff.close();
  return ipart;
}

proc readFile(fn : string, pp : []WeightedParticle3D)  {
  const maxcols=25;

  var ff = open(fn, iomode.r);
  var cols : [1.. #maxcols] real;
  var icol=1;
  var ipart = 0;
  for iff in ff.lines() {
   icol = 1; 
   for col1 in iff.split(spaces) {
     if (col1.size==0) then continue;
     cols[icol] = col1 : real;
     icol += 1;
   }
   if (icol < 4) then assert(false,"malformed line...");
   for jj in Ddim do pp[ipart].x(jj-1) = cols[jj];
   pp[ipart].w = cols[4];
   pp[ipart].r2 = + reduce (pp[ipart].x**2);
   ipart += 1;
  }
}

proc smuAccumulate(hh : UniformBins, p1, p2 : []WeightedParticle3D, d1,d2 : domain(1), scale : real) {
  forall ii in d1 { // Loop over first set of particles
   
    var x1,y1,z1,w1,r2 : real;
    var sl, s2, l1, s1, l2, mu, wprod : real;
    x1 = p1[ii].x(0); y1 = p1[ii].x(1); z1 = p1[ii].x(2); w1 = p1[ii].w; r2 = p1[ii].r2;

    for jj in d2 { // Second set of particles
      mu=2*(p2[jj].x(0)*x1 + p2[jj].x(1)*y1 + p2[jj].x(2)*z1);
      sl = r2 - p2[jj].r2;
      l1 = r2 + p2[jj].r2;
      s2 = l1 - mu;
      l2 = l1 + mu;
      if ((s2 >= smax2) || (s2 < 1.0e-20)) then continue;
      wprod = scale * w1 * p2[jj].w;
      s1 = sqrt(s2);
      mu = sl/(s1*sqrt(l2));
      if (mu < 0) then mu = -mu;
      
      hh.add((s1,mu),wprod);
    }
  }
}



proc splitOn(pp : []WeightedParticle3D, scr : []WeightedParticle3D, splitDim : int, xsplit : real) : int {
  var npart, lnpart : int;
  var lo = pp.domain.low;
  var hi = pp.domain.high;
  npart = pp.domain.size;
  lnpart = 0;
  for ipp in pp {
    if (ipp.x(splitDim-1) < xsplit) then lnpart+=1;
  }
  var li, ri : int;
  li = lo; ri = (lo+lnpart);
  for ipp in pp {
    if (ipp.x(splitDim-1) < xsplit) {
      scr[li] = ipp;
      li+=1;
    } else {
      scr[ri] = ipp;
      ri+=1;
    }
  }
  assert(li==(lo+lnpart), "splitOn : li != lo+npart");
  assert(ri==(hi+1), "splitOn : ri != hi+1");
  pp = scr;

  return lnpart;
}


class KDNode {
  var lo, hi,npart,id : int;
  var dom : domain(1);
  var xcen : NDIM*real;
  var rcell : real;
  var left, right : owned KDNode?;

  proc isLeaf() : bool {
    return (left==nil) && (right==nil);
  }
}


proc BuildTree(pp : []WeightedParticle3D, scr : []WeightedParticle3D, id : int) : owned KDNode {
  gtime1.start();
  var me = new owned KDNode();
  var dom = pp.domain;
  me.lo = dom.low;
  me.hi = dom.high;
  me.dom = {me.lo..me.hi};
  me.id = id;
  me.npart = (me.hi-me.lo)+1;

  //  work out xcen and vantage point radius
  var pmin = pp[me.lo].x;
  var pmax = pp[me.lo].x;
  for ipp in pp {
    for idim in Ddim {
      if (ipp.x(idim-1) < pmin(idim-1)) then pmin(idim-1) = ipp.x(idim-1);
      if (ipp.x(idim-1) > pmax(idim-1)) then pmax(idim-1) = ipp.x(idim-1);
    }
  }
  me.xcen = (pmax+pmin)/2.0;
  var dx : NDIM*real;
  me.rcell = 0.0;
  var r1 : real;
  for ipp in pp {
    dx = (ipp.x - me.xcen);
    r1 = sqrt(+ reduce dx**2);
    if (r1 > me.rcell) then me.rcell = r1;
  }

  // Continue to split
  gtime1.stop();
  if (me.npart <= minpart) then return me; // Don't split further.
  gtime1.start();

  // Find dimension to split on
  dx = pmax - pmin; 
  var splitDim = 1;
  for idim in Ddim {
    if (dx(idim-1) > dx(splitDim)) then splitDim=idim;
  }

  // Split
  var lnpart = splitOn(pp, scr,splitDim, me.xcen(splitDim-1));
  var ldom = {me.lo..(me.lo+lnpart-1)};
  var rdom = {(me.lo+lnpart)..me.hi};
  gtime1.stop();
  me.left = BuildTree(pp[ldom],scr[ldom],2*id+1);
  me.right = BuildTree(pp[rdom],scr[rdom],2*id+2);
  return me;
}

proc TreeAccumulate(hh : UniformBins, p1, p2 : []WeightedParticle3D, node1, node2 : KDNode) {
  // Compute the distance between node1 and node2
  var rr = sqrt (+ reduce(node1.xcen - node2.xcen)**2);
  var rmin = rr - (node1.rcell+node2.rcell);

  // If distance is greater than all cases
  if (rmin > smax) then return;

  // If both nodes are leaves
  if (node1.isLeaf() & node2.isLeaf()) {
    begin smuAccumulate(hh, p1, p2, node1.dom, node2.dom,1);
    return;
  }

  // If one node is a leaf 
  if (node1.isLeaf()) {
    TreeAccumulate(hh, p1, p2, node1, node2.left!);
    TreeAccumulate(hh, p1, p2, node1, node2.right!);
    return;
  }
  if (node2.isLeaf()) {
    TreeAccumulate(hh, p1, p2, node1.left!, node2);
    TreeAccumulate(hh, p1, p2, node1.right!, node2);
    return;
  }

  // Split the larger case;
  if (node1.npart > node2.npart) {
    TreeAccumulate(hh, p1, p2, node1.left!, node2);
    TreeAccumulate(hh, p1, p2, node1.right!, node2);
    return;
  } else {
    TreeAccumulate(hh, p1, p2, node1, node2.left!);
    TreeAccumulate(hh, p1, p2, node1, node2.right!);
    return;
  }

}

// The main code 
proc doPairs() {
  var tt : Timer;

  // Read in the file
  var nlines1, nlines2 : int;
  var Dpart1 = {0.. #0};
  var Dpart2 = {0.. #0};
  var pp1 : [Dpart1] WeightedParticle3D;
  var pp2 : [Dpart2] WeightedParticle3D;
  tt.clear(); tt.start();
  if isPerf {
    nlines1 = nParticles;
    Dpart1 = {0.. #nlines1};
    generateRandom(pp1);
    nlines2 = nParticles;
    Dpart2 = {0.. #nlines2};
    generateRandom(pp2);
  } else {
    nlines1 = countLines(fn1);
    Dpart1 = {0.. #nlines1};
    readFile(fn1,pp1);
    nlines2 = countLines(fn2);
    Dpart2 = {0.. #nlines2};
    readFile(fn1,pp2);
    if (!isTest) {
      writef("Read in %i lines from file %s \n", pp1.size, fn1);
      writef("Read in %i lines from file %s \n", pp2.size, fn2);
    }
  }
  tt.stop();
  if !isTest then writef("Time to read : %r \n", tt.elapsed());

  // Build the tree
  tt.clear(); tt.start(); gtime1.clear();
  var scr1 : [Dpart1] WeightedParticle3D;
  var root1 = BuildTree(pp1,scr1,0);
  var scr2 : [Dpart2] WeightedParticle3D;
  var root2 = BuildTree(pp2,scr2,0);
  tt.stop();
  if (!isTest) {
    writef("Time to build trees : %r \n", tt.elapsed());
    writef("Time in splitOn : %r \n", gtime1.elapsed());
  }
  


  // Set up the histogram
  var hh = new UniformBins(2,(nsbins,nmubins), ((0.0,smax),(0.0,1.0+1.e-10)));

  /*
  // Brute force paircounts
  // This is only here for comparisons... and should be turned off in general
  if (doBrute && !isTest) {
    tt.clear(); tt.start();
    smuAccumulate(hh, soa1,soa2, soa1.Dpart, soa2.Dpart, 1.0);
    tt.stop();
    writef("Time to brute force paircount : %r \n", tt.elapsed());
    var ff1 = openwriter("%s.brute".format(pairfn));
    writeHist(ff1,hh);
    ff1.close();
  }
  */

   // Do the paircounts with a tree
  hh.reset();
  tt.clear(); tt.start();
  sync TreeAccumulate(hh, pp1, pp2, root1, root2);
  tt.stop();
  if (!isTest) {
    writef("Time to tree paircount : %r \n", tt.elapsed());
    if !isPerf {
      var ff = openwriter("%s.tree".format(pairfn));
      writeHist(ff,hh);
      ff.close();
    }
  } else {
    hh.set((0,0),0.0);
    writeHist(stdout,hh,"%20.5er ");
  }
}

