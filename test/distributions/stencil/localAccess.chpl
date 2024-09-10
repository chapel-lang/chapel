use StencilDist;

const D = {1..10} dmapped new stencilDist({1..10}, fluff=(1,));
var A: [D] real = [i in D] i;

A.updateFluff();

// on a 2-locale run, these accesses would be considered out-of-bounds
// with respect to the indices that the locale is the primary owner
// of, but not with respect to the fluff values that it stores.  This
// test ensures that we don't get an OOB access check on such local
// accesses to fluff values.

local { writeln(A.localAccess(5));    // this is properly owned by me
        writeln(A.localAccess(6)); }  // this is part of my fluff

on Locales.last { local {
  writeln(A.localAccess(6));  // this is properly owned by me
  writeln(A.localAccess(5));  // this is part of my fluff
  writeln(A.localAccess(4));  // this is not local
} }
