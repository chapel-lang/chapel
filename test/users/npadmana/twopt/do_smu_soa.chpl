use Histogram;
use Time;

// Test flags
config const isTest=false;

// Input/Output filenames
config const fn1 = "test.dat";
config const fn2 = "test.dat";
config const pairfn = "test-DD.dat";

// Dimension constanst
param NDIM=3;
param NEXT=2; // w, r2
param NTOT=NDIM+NEXT;
const W=NDIM;
const R2=NDIM+1;
const ParticleAttrib = 0.. #NTOT;
const DimSpace = 0.. #NDIM;

// Tree parameters
config const minpart=500;

// Histogram parameters
config const smax=200.0;
config const smax2=smax**2;
config const nmubins=5;
config const nsbins=5;

// Global variables
var gtime1 : Timer;

proc main() {
  if isTest {
    testPairs();
  } else {
    doPairs();
  }
}


class Particle3D {
  var npart : int = 0;
  var Darr : domain(2);
  var Dpart : domain(1);
  var arr : [Darr] real;

  var _tmp : [Dpart] real;
  var _n1, _ndx : [Dpart] int;

  proc Particle3D(npart1 : int) {
    npart = npart1;
    Darr = {ParticleAttrib, 0.. #npart};
    Dpart = {0.. #npart};
  }

  proc this(ii : int) : NTOT*real {
    return (arr[0,ii],arr[1,ii],arr[2,ii],arr[3,ii],arr[4,ii]);
  }

  
}


proc countLines(fn : string) : int {
  var ff = open(fn, iomode.r);
  var ipart = 0;
  for iff in ff.lines() do ipart +=1;
  ff.close();
  return ipart;
}

proc readFile(fn : string) : Particle3D  {
  var npart = countLines(fn);
  var pp = new Particle3D(npart);

  var ff = openreader(fn);
  var ipart = 0;
  var x,y,z,w,r2 : real;
  while (ff.read(x,y,z,w)) {
    r2 = x**2 + y**2 + z**2;
    pp.arr[..,ipart] = [x,y,z,w,r2];
    ipart += 1;
  }

  return pp;
}

proc splitOn(pp : Particle3D, idom : domain(1), splitDim : int, xsplit : real) : int {
  // Setup for a prefix scan
  forall ii in idom {
    if (pp.arr[splitDim,ii] < xsplit) {
      pp._n1[ii] = 1;
    } else {
      pp._n1[ii] = 0;
    }
  }

  var lnpart : int = 0;
  for ii in idom do lnpart += pp._n1[ii];
  var li = idom.low;
  var ri = li+lnpart;
  for ii in idom {
    if (pp._n1[ii] == 1) {
      pp._ndx[ii] = li;
      li += 1;
    } else {
      pp._ndx[ii] = ri;
      ri += 1;
    }
  }

  for kk in ParticleAttrib {
    forall ii in idom {
      pp._tmp[pp._ndx[ii]] = pp.arr[kk,ii];
    }
    forall ii in idom {
      pp.arr[kk,ii] = pp._tmp[ii];
    }
  }

  return lnpart;
}
      
class KDNode {
  var lo, hi,npart,id : int;
  var dom : domain(1);
  var xcen : [DimSpace]real;
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


proc BuildTree(pp : Particle3D, lo : int, hi : int, id : int) : KDNode  {
  var me : KDNode = new KDNode();
  me.lo = lo;
  me.hi = hi;
  me.dom = {lo..hi};
  me.id = id;
  me.npart = (me.hi-me.lo)+1;

  //  work out xcen and vantage point radius
  var pmin, pmax : [DimSpace] real;
  for idim in DimSpace {
    pmin[idim] = min reduce pp.arr[idim,lo..hi];
    pmax[idim] = max reduce pp.arr[idim,lo..hi];
  }
  me.xcen = (pmax+pmin)/2.0;
  me.rcell = 0.0;
  var r1 : real;
  for ii in lo..hi {
    r1 = 0.0;
    for idim in DimSpace {
      r1 += (pp.arr[idim,ii]-me.xcen[idim])**2;
    }
    r1 = sqrt(r1);
    if (r1 > me.rcell) then me.rcell = r1;
  }

  // Continue to split
  if (me.npart <= minpart) then return me; // Don't split further.

  // Find dimension to split on
  var dx : [DimSpace] real;
  dx = pmax - pmin; 
  var splitDim = 0;
  for idim in DimSpace {
    if (dx[idim] > dx[splitDim]) then splitDim=idim;
  }

  // Split
  var lnpart = splitOn(pp,me.dom,splitDim, me.xcen[splitDim]);
  me.left  = BuildTree(pp, lo, lo+lnpart-1, 2*id+1);
  me.right = BuildTree(pp, lo+lnpart, hi,2*id+2);
  return me;
}

proc TreeAccumulate(hh : UniformBins, p1, p2 : Particle3D, node1, node2 : KDNode) {
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

  

// The basic pair counter
proc smuAccumulate(hh : UniformBins, p1,p2 : Particle3D, d1,d2 : domain(1), scale : real) {
  forall ii in d1 { // Loop over first set of particles
   
    var x1,y1,z1,w1,r2 : real;
    var sl, s2, l1, s1, l2, mu, wprod : real;
    (x1,y1,z1,w1,r2) = p1[ii];

    for jj in d2 { // Second set of particles
      mu=2*(p2.arr[0,jj]*x1 + p2.arr[1,jj]*y1 + p2.arr[2,jj]*z1);
      sl = r2 - p2.arr[R2,jj];
      l1 = r2 + p2.arr[R2,jj];
      s2 = l1 - mu;
      l2 = l1 + mu;
      if ((s2 >= smax2) || (s2 < 1.0e-20)) then continue;
      wprod = scale * w1 * p2.arr[W,jj];
      s1 = sqrt(s2);
      mu = sl/(s1*sqrt(l2));
      if (mu < 0) then mu = -mu;
      
      hh.add((s1,mu),wprod);
    }
  }
}


proc testPairs() {
  var pp = readFile("test.dat");

  // Set up the histogram
  var hh = new UniformBins(2,(nsbins,nmubins), ((0.0,smax),(0.0,1.0+1.e-10)));

  // Do the paircounts with a tree
  smuAccumulate(hh,pp,pp,pp.Dpart,pp.Dpart,1.0);
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

  // Build the tree
  var root1 = BuildTree(pp, 0, pp.npart-1, 0); 
  hh.reset();
  sync TreeAccumulate(hh,pp,pp,root1,root1);
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
    

proc doPairs() {
  var tt : Timer;

  // Read in the file
  tt.clear(); tt.start();
  var pp1 = readFile(fn1);
  var pp2 = readFile(fn2);
  tt.stop();
  writef("Read in %i lines from file %s \n", pp1.npart, fn1);
  writef("Read in %i lines from file %s \n", pp2.npart, fn2);
  writef("Time to read : %r \n", tt.elapsed());

  // Build the tree
  tt.clear(); tt.start(); gtime1.clear();
  var root1 = BuildTree(pp1, 0, pp1.npart-1, 0);
  var root2 = BuildTree(pp2, 0, pp2.npart-1, 0);
  tt.stop();
  writef("Time to build trees : %r \n", tt.elapsed());
  //writef("Time in splitOn : %r \n", gtime1.elapsed());
  


  // Set up the histogram
  var hh = new UniformBins(2,(nsbins,nmubins), ((0.0,smax),(0.0,1.0+1.e-10)));

  // Do the paircounts with a tree
  hh.reset();
  tt.clear(); tt.start();
  sync TreeAccumulate(hh, pp1,pp2, root1, root2);
  tt.stop();
  writef("Time to tree paircount : %r \n", tt.elapsed());
  //writeHist("%s.tree".format(pairfn),hh);
}

