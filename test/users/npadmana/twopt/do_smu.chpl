use Regexp;
use Histogram;
use Time;

// Use the test/twopt code
config const isTest=false;
config const doBrute=false;

config const fn1 = "test.dat";
config const fn2 = "test.dat";
config const pairfn = "test-DD.dat";

param NDIM  = 3;
const Ddim = {0.. #NDIM};

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
  if isTest then testPairs(); else doPairs();
}

record WeightedParticle3D {
  var x : [Ddim] real;
  var w : real;
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
  var cols : [0.. #maxcols] real;
  var icol=0;
  var ipart = 0;
  for iff in ff.lines() {
   icol = 0; 
   for col1 in iff.split(spaces) {
     if (col1.length==0) then continue;
     cols[icol] = col1 : real;
     icol += 1;
   }
   if (icol < 4) then assert(false,"malformed line...");
   pp[ipart].x = cols[Ddim];
   pp[ipart].w = cols[3];
   ipart += 1;
  }
}

    



class SOA_WeightedParticle3D {
  var npart : int;
  var Dpart : domain(1);
  var x,y,z,w,r2 : [Dpart] real;


  proc SOA_WeightedParticle3D(pp : []WeightedParticle3D) {
    npart = pp.size;
    Dpart = 0.. #npart;

    forall ii in Dpart {
      x[ii] = pp[ii].x[0];
      y[ii] = pp[ii].x[1];
      z[ii] = pp[ii].x[2];
      w[ii] = pp[ii].w;
      r2[ii] = + reduce (pp[ii].x**2);
    }
  }
}


proc smuAccumulate(hh : UniformBins, p1,p2 : SOA_WeightedParticle3D, d1,d2 : domain(1), scale : real) {
  forall ii in d1 { // Loop over first set of particles
   
    var x1,y1,z1,w1,r2 : real;
    var sl, s2, l1, s1, l2, mu, wprod : real;
    x1 = p1.x[ii]; y1 = p1.y[ii]; z1 = p1.z[ii]; w1 = p1.w[ii]; r2 = p1.r2[ii];

    for jj in d2 { // Second set of particles
      mu=2*(p2.x[jj]*x1 + p2.y[jj]*y1 + p2.z[jj]*z1);
      sl = r2 - p2.r2[jj];
      l1 = r2 + p2.r2[jj];
      s2 = l1 - mu;
      l2 = l1 + mu;
      if ((s2 >= smax2) || (s2 < 1.0e-20)) then continue;
      wprod = scale * w1 * p2.w[jj];
      s1 = sqrt(s2);
      mu = sl/(s1*sqrt(l2));
      if (mu < 0) then mu = -mu;
      
      hh.add((s1,mu),wprod);
    }
  }
}

proc splitOn(pp : []WeightedParticle3D, scr : []WeightedParticle3D, splitDim : int, rr : []real) : int {
  var npart, lnpart : int;
  var lo = pp.domain.low;
  var hi = pp.domain.high;
  npart = pp.domain.size;
  lnpart = 0;
  for ipp in pp {
    if (ipp.x[splitDim] < rr[splitDim]) then lnpart+=1;
  }
//  var part : [pp.domain] WeightedParticle3D;
  var li, ri : int;
  li = lo; ri = (lo+lnpart);
  for ipp in pp {
    if (ipp.x[splitDim] < rr[splitDim]) {
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
  var xcen : [Ddim]real;
  var rcell : real;
  var left, right : KDNode;

  proc isLeaf() : bool {
    return (left==nil) && (right==nil);
  }

  proc ~KDNode() {
    if left then delete left;
    if right then delete right;
  }

}


proc BuildTree(pp : []WeightedParticle3D, scr : []WeightedParticle3D, id : int) : KDNode{
  gtime1.start();
  var me = new KDNode();
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
      if (ipp.x[idim] < pmin[idim]) then pmin[idim] = ipp.x[idim];
      if (ipp.x[idim] > pmax[idim]) then pmax[idim] = ipp.x[idim];
    }
  }
  me.xcen = (pmax+pmin)/2.0;
  var dx : [Ddim]real;
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
  var splitDim = 0;
  for idim in Ddim {
    if (dx[idim] > dx[splitDim]) then splitDim=idim;
  }

  // Split
  var lnpart = splitOn(pp, scr,splitDim, me.xcen);
  var ldom = {me.lo..(me.lo+lnpart-1)};
  var rdom = {(me.lo+lnpart)..me.hi};
  gtime1.stop();
  me.left = BuildTree(pp[ldom],scr[ldom],2*id+1);
  me.right = BuildTree(pp[rdom],scr[rdom],2*id+2);
  return me;
}

proc TreeAccumulate(hh : UniformBins, p1, p2 : SOA_WeightedParticle3D, node1, node2 : KDNode) {
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
    TreeAccumulate(hh, p1, p2, node1, node2.left);
    TreeAccumulate(hh, p1, p2, node1, node2.right);
    return;
  }
  if (node2.isLeaf()) {
    TreeAccumulate(hh, p1, p2, node1.left, node2);
    TreeAccumulate(hh, p1, p2, node1.right, node2);
    return;
  }

  // Split the larger case;
  if (node1.npart > node2.npart) {
    TreeAccumulate(hh, p1, p2, node1.left, node2);
    TreeAccumulate(hh, p1, p2, node1.right, node2);
    return;
  } else {
    TreeAccumulate(hh, p1, p2, node1, node2.left);
    TreeAccumulate(hh, p1, p2, node1, node2.right);
    return;
  }

}

// A simple testHarness
proc testPairs() {
  
  // Read in the file
  var nlines = countLines(fn1);
  var pp1 : [0.. #nlines] WeightedParticle3D;
  readFile("test.dat",pp1);

  // Build the tree
  var scr : [0.. #nlines] WeightedParticle3D;
  var root1 = BuildTree(pp1,scr,0);

  // Set up the histogram
  var hh = new UniformBins(2,(nsbins,nmubins), ((0.0,smax),(0.0,1.0+1.e-10)));

  // AOS -> SOA
  var soa1 = new SOA_WeightedParticle3D(pp1);

  // Do the paircounts with a tree
  sync TreeAccumulate(hh, soa1, soa1, root1, root1);
  for xx in hh.bins(1) do writef("%12.4dr",xx); 
  writeln();
  for xx in hh.bins(2) do writef("%12.4dr",xx); 
  writeln("\n##");
  for ii in hh.Dhist.dim(1) {
    for jj in hh.Dhist.dim(2) {
      writef("%20.5er ",hh[(ii,jj)]);
    }
    writeln();
  }
}


// The main code 
proc doPairs() {
  var tt : Timer;

  // Read in the file
  tt.clear(); tt.start();
  var nlines1 = countLines(fn1);
  var pp1 : [0.. #nlines1] WeightedParticle3D;
  readFile(fn1,pp1);
  var nlines2 = countLines(fn2);
  var pp2 : [0.. #nlines2] WeightedParticle3D;
  readFile(fn1,pp2);
  tt.stop();
  writef("Read in %i lines from file %s \n", pp1.size, fn1);
  writef("Read in %i lines from file %s \n", pp2.size, fn2);
  writef("Time to read : %r \n", tt.elapsed());

  // Build the tree
  tt.clear(); tt.start(); gtime1.clear();
  var scr1 : [0.. #nlines1] WeightedParticle3D;
  var root1 = BuildTree(pp1,scr1,0);
  var scr2 : [0.. #nlines2] WeightedParticle3D;
  var root2 = BuildTree(pp2,scr2,0);
  tt.stop();
  writef("Time to build trees : %r \n", tt.elapsed());
  writef("Time in splitOn : %r \n", gtime1.elapsed());
  


  // Set up the histogram
  var hh = new UniformBins(2,(nsbins,nmubins), ((0.0,smax),(0.0,1.0+1.e-10)));

  // AOS -> SOA
  tt.clear(); tt.start();
  var soa1 = new SOA_WeightedParticle3D(pp1);
  var soa2 = new SOA_WeightedParticle3D(pp2);
  tt.stop();
  writef("Time to SOA : %r \n", tt.elapsed());

  // Brute force paircounts
  if doBrute {
    tt.clear(); tt.start();
    smuAccumulate(hh, soa1,soa2, soa1.Dpart, soa2.Dpart, 1.0);
    tt.stop();
    writef("Time to brute force paircount : %r \n", tt.elapsed());
    writeHist("%s.brute".format(pairfn),hh);
  }

   // Do the paircounts with a tree
  hh.reset();
  tt.clear(); tt.start();
  sync TreeAccumulate(hh, soa1, soa2, root1, root2);
  tt.stop();
  writef("Time to tree paircount : %r \n", tt.elapsed());
  writeHist("%s.tree".format(pairfn),hh);
}

