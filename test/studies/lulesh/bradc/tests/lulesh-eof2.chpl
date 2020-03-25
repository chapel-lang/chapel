/*

  Livermore Unstructured Lagrangian Explicit Shock Hydrodynamics (LULESH)
  https://computation.llnl.gov/casc/ShockHydro/

  Version 1.0.0

  Originally ported by Brandon Holt (8/2011)

  Notes on the Initial Implementation
  -----------------------------------
   
  This implementation is complete in that it produces nearly identical
  results to the C++ implementation. Verifying this was done by
  putting identical printouts of various arrays each time step and
  diff'ing the outputs. Some minor variations were observed (for
  extremely tiny values on the order of 10^-40, or in the last digit
  of a floating point value).  These variations are one-off and most
  often are gone the next iteration and do not appear to propagate or
  accumulate, so they are assumed to be variations in the order that
  operations are carried out.

  This implementation was designed to mirror the overall structure of
  the C++ Lulesh but use Chapel constructs where they can help make
  the code more readable, easier to maintain, or more
  'elegant'. Function names are preserved for the most part, with some
  additional helper functions, and original comments from the C++ code
  are interspersed approximately where they belong to give an idea of
  how the two codes line up. One major difference for this Chapel
  version is the use of a number of module-level variables and
  constants.

  Notes on Performance Improvements
  ---------------------------------
  [sungeun@cray.com 12/2011]

  - Replaced many consts with params because they are compile time
    constants.  In many cases the compiler should be able to detect
    this, but currently does not.

  - Replaced Chapel reductions over small tuples with explicit fused
    param loops (~6x performance improvement).  It's possible that
    using --dataParMinGranularity would have helped with performance
    here.

  - Similarly, replaced whole array operations with explicitly fused
    forall loops.

  - As much as possible, under-specified formal types (domain) of
    array formal arguments.  This avoids an expensive reindexing
    operation.

  - Replaced zippered foralls using elemToNodes() and a small static
    domain with a non-zippered forall using an iterator that returns a
    tuple.

  - Removed 'in' intent since it is currently more expensive than
    const.

  - Removed unnecessary locks (sync vars).

  - Inlined some functions.

 */


// TODO: Found a lot of loops that were computing reductions in a race-y
// way (i.e., by accumulating into unprotected scalars).  This could be
// the cause of the numerical instability in the nightly tests.  We 
// really should be using a reduction for these.

use Time, IO;

/* Compile-time constants */

config const showProgress = false;
config const debug = false;
config const doTiming = false;  // TODO: reset to true
config const printCoords = false;

param XI_M        = 0x003;
param XI_M_SYMM   = 0x001;
param XI_M_FREE   = 0x002;

param XI_P        = 0x00c;
param XI_P_SYMM   = 0x004;
param XI_P_FREE   = 0x008;

param ETA_M       = 0x030;
param ETA_M_SYMM  = 0x010;
param ETA_M_FREE  = 0x020;

param ETA_P       = 0x0c0;
param ETA_P_SYMM  = 0x040;
param ETA_P_FREE  = 0x080;

param ZETA_M      = 0x300;
param ZETA_M_SYMM = 0x100;
param ZETA_M_FREE = 0x200;

param ZETA_P      = 0xc00;
param ZETA_P_SYMM = 0x400;
param ZETA_P_FREE = 0x800;

config const filename = "sedov15oct.lmesh";
var infile = open(filename, iomode.r);
var reader = infile.reader();

if debug then writeln("Reading problem size...");
const (numElems, numNodes) = reader.read(int, int);

// TODO: Do we want to keep this, or promote it across all reads?
config const debugIO = true;

if (debugIO) then
  writeln("Using ", numElems, " elements, and ", numNodes, " nodes");

if debug then
  writeln((numElems, numNodes));

/* Setup Problem Domain*/

//ranges
const Elems = 0..#numElems;
const Nodes = 0..#numNodes;

//domains
const ElemSpace = {Elems};
const NodeSpace = {Nodes};


//distributions
use BlockDist;
config param useBlockDist = false;


// STYLE: I don't really like these names (ElemDist, NodeDist)

const ElemDist = if useBlockDist then ElemSpace dmapped Block(ElemSpace)
                                 else ElemSpace;
const NodeDist = if useBlockDist then NodeSpace dmapped Block(NodeSpace)
                                 else NodeSpace;

                           
                                 
// STYLE: It'd be nice to replace groups of three arrays with some
// sort of tuple or otherwise parameterizable data structure

// coordinates
var
   x, y, z: [NodeDist] real; //coordinates


// TODO: Support comments in file between major sections for clarity
                                                                         
if debug then writeln("reading coordinates");
for (locX,locY,locZ) in zip(x,y,z) do reader.read(locX, locY, locZ);

param nodesPerElem = 8;

                                 
// STYLE: In some places we're using an 8* tuple and in other places
// an 8-ary array, which seems inconsistent and could raise questions
// about how to know when to use which.  Today, tuples are used in
// contexts like this primarily for performance purposes.  Ultimately,
// the two should perform the same for these simple cases.

                                                                         
// Could name this... we chose not to...
//
// const elemNeighbors = 0..#nodesPerElem;

//
// STYLE: Wouldn't it be cool to just stream the reader into this
// as an initializer to fill it all up?  Maybe we can and I just
// don't know how to?  Or else, we can write our own iterator...
//
var elemToNode: [ElemDist] [0..#nodesPerElem] index(NodeDist);


                                 
                                 
if debug then writeln("reading elemToNode mapping");
//
// OR, at the very least, we should be able to write read(elemToNode);
for e in elemToNode do 
  for n in e do
    reader.read(n);

                                                                         


var lxim, lxip, letam, letap, lzetam, lzetap: [ElemDist] index(ElemDist);

if debug then writeln("reading greek stuff");

for (xm,xp,em,ep,zm,zp) in zip(lxim, lxip, letam, letap, lzetam, lzetap) do
  reader.read(xm,xp,em,ep,zm,zp);


if debug then writeln("reading symmetric cell locations");

// NOTE: The integers returned by readNodeSet below are not actually
// used currently because Chapel prefers iterating over arrays directly
// (i.e. 'forall x in XSym' rather than 'forall i in 0..#numSymX ... XSym[i]').
//
// Moreover, an array's size can also be queried directly
// (i.e., 'const numSymX = XSym.size')
//
// We used the style below to demonstrate a commonly used idiom in
// current unstructured codes.

var (numSymX, XSym) = readNodeset(reader);
var (numSymY, YSym) = readNodeset(reader);
var (numSymZ, ZSym) = readNodeset(reader);

if debugIO {
  writeln("XSym:\n", XSym);
  writeln("YSym:\n", YSym);
  writeln("ZSym:\n", ZSym);
}


if debug then writeln("reading free surfaces");

var (numFreeSurf, freeSurface) = readNodeset(reader);

if debugIO {
  writeln("freeSurface:\n", freeSurface);
}


// It'd be nice to have something like the following built-in.  It
// could use a different name/interface as far as I'm concerned.

writeln("Doing EOF check");

reader.assertEOF();

writeln("Made it past EOF check");

proc readNodeset(reader) {
  const arrSize = reader.read(int);
  //  var A: [0..#arrSize] index(NodeDist);
  var A: [0..#arrSize] int;

  for a in A do
    reader.read(a);

  return (arrSize, A);
}
