/*
The definition and use example of Block.changeBoundingBox(),
which allows delayed setting of the bounding box
on an instance of a Block distribution.
*/

use BlockDist;

const dummyBB: domain(1) = {1..1}; // or whatever bounding box type you want eventually

// Create a distribution without knowing its bounding box dimensions.
const dist = new unmanaged Block(dummyBB);

// Any computations here, should not involve 'dist'. //

//
// Any of the following should set the bounding box, once ready:
//
dist.changeBoundingBox(1..8);    // range - only for 1-d
dist.changeBoundingBox((1..8,)); // rank-tuple of ranges
dist.changeBoundingBox({1..8});  // domain of a matching rank

// Show the result.
for idx in 0..9 do writeln("A:  ", idx, " maps to ",
                           dist.dsiIndexToLocale(idx));

// This domain will be distributed according to dist's current bounding box.
var D: domain(1) dmapped new dmap(dist);

// Show the distribution in action.
D = {0..9};
var A: [D] real;
for idx in D do writeln("B:  ", idx, "  maps to ", A[idx].locale);


/*
WARNING: currently, the following will not work when compiled with --local:

  var dist = new Block(...);
  var dm   = new dmap(dist);
  dist.changeBoundingBox(...);

This is because "new dmap(ARG)" retains a copy of ARG
and destroys the original. We have a .future on this.

For now, do this instead:
*/

const distTemp = new unmanaged Block(dummyBB);
const DM = new dmap(distTemp);

DM.changeBoundingBox(1..4);
for idx in 0..6 do writeln("C:  ", idx, " maps to ",
                           DM.idxToLocale(idx));

// Show this one in action, too.
var Dbis: domain(1) dmapped DM;
Dbis = {0..6};
var Abis: [Dbis] real;
for idx in Dbis do writeln("D:  ", idx, "  maps to ", Abis[idx].locale);


/* Implementation of changeBoundingBox().

Change the bounding box of an existing Block distribution to 'newBB'.
'newBB' can be: a domain, a tuple of ranges, or a single range (if 1-D).

If any domain(s) have been dmapped using 'this' distribution,
changeBoundingBox() will halt. If the halt is commented out,
calling changeBoundingBox() will affect only future domains/arrays
created using 'this' - it will not redistribute already-existing
domains/arrays.
*/
proc Block.changeBoundingBox(newBB) {
  // Comment out this check if desired. NB Block-distributed domains created
  // using 'this' will not be re-distributed upon changeBoundingBox().
  if _doms.size != 0 then
    halt("changeBoundingBox: the distribution already has declared domain(s)");

  // From Block.dsiAssign, with mods.
  boundingBox = newBB;
  coforall locid in targetLocDom do
    on targetLocales(locid) {
      delete locDist(locid);
      locDist(locid) = new unmanaged LocBlock(rank, idxType, locid, boundingBox,
                                              targetLocDom);
    }
  // NB at this point privatized copies of 'this' on other locales, if any,
  // have stale pointers in their locDist arrays.

  // Replicate across locales, if needed.
  if pid >= 0 then
    _reprivatize(_to_unmanaged(this));
}
