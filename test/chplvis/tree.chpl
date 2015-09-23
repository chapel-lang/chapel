// explore the communication and execution patters
//   Trees
//   Hypercube tree patters
//
//   Done both with iterators and procs.
//
//   Also, using VisualDebug to show comm patters of
//   these structures

use VisualDebug;
use BlockDist;

iter treeiter ( n: int, id: int=0) : int {
   if (id < n) {
      yield id;

      // recursive?
      for z in treeiter( n, 2*id + 1) do yield z;
      for z in treeiter( n, 2*id + 2) do yield z;
   }
}

iter treeiter ( param tag: iterKind, n: int, id: int=0) : int
      where tag == iterKind.standalone {
   if (id < n) {
     yield id;

     // recursive ... but still have to yeild here
     cobegin {
       if 2*id+1 < n then on Locales[2*id+1] do
         for z in treeiter(n=n, id=2*id + 1, tag=iterKind.standalone) do yield z;
       if 2*id+2 < n then on Locales[2*id+2] do
        for z in treeiter(n=n, id=2*id + 2, tag=iterKind.standalone) do yield z;
     }
   }
}


startVdebug ("TREEvis");

const space = { 0..#numLocales };

tagVdebug ("decl 1");

const Bspace = space dmapped Block(boundingBox=space);

tagVdebug ("decl 2");

var a: [Bspace] int;
var b: [Bspace] int;
var c: [Bspace] int;
var d: [Bspace] int;
var e: [Bspace] int;

tagVdebug ("simple forall");

forall i in Bspace do b[i] = here.id;

tagVdebug ("writeln 0");

writeln("B is: ", b);

tagVdebug ("treeIter");

forall i in treeiter(numLocales) do b[i] = here.id;

tagVdebug ("writeln 1");

writeln("B is: ", b);

// Tree procedure ... defined after a
proc SetAPar (id: int = 0) {
   var child = id * 2 + 1;

   cobegin {
      if child < numLocales then
        on Locales[child] do SetAPar (child);
      if child+1 < numLocales then
        on Locales[child+1] do SetAPar (child+1);
      a[id] = here.id;
   }
}

tagVdebug ("treeProc");

SetAPar();

tagVdebug ("writeln 2");

writeln("A is: ", a);

tagVdebug ("hcubeProc");

// Hypercube style tree

iter id2com ( id: int, off: int ) {
     var offset = off;
     var ix = 1;
     while (offset > 0) {
        yield (id+offset,ix);
        offset = offset >> 1;
        ix = ix + 1;
     }
}

proc SetCHcube (n: int = numLocales, id: int = 0, off: int = -1) {
  var offset = 1;
  if (off < 0) then
    while (offset*2 + id < n) do offset = offset << 1;
  else
    offset = off;

  coforall (rid, shift) in id2com (id, offset) do
    if (rid < n) then
      on Locales[rid] do SetCHcube (n, rid, offset >> shift);
  c[id] = here.id;
}

SetCHcube();

tagVdebug ("writeln 3");

writeln("C is: ", c);

pauseVdebug ();

iter cubeiter ( n: int, id: int=0, off: int = -1) : int {
   var offset = 1;
   if (off < 0) then
      while (offset*2+id < n) do offset = offset << 1;
   else
      offset = off;

   yield id;

   if (id + offset < n ) then
     while (offset > 0) {
        for z in cubeiter(n, id+offset, offset >> 1) do yield z;
        offset = offset >> 1;
     }
}

iter cubeiter ( param tag: iterKind, n: int, id: int=0, off: int = -1) : int
    where tag == iterKind.standalone {
   // if (id == 0) then writeln ("Running standalone parallel treeiter.");
   var offset = 1;
   if (off < 0) then
      while (offset*2+id < n) do offset = offset << 1;
   else
      offset = off;

   yield id;

   while (offset > 0) {
     if (id + offset < n ) then
        on Locales[id+offset] do
           for z in cubeiter(n, id+offset, offset >> 1, tag=iterKind.standalone)
              do yield z;
        offset = offset >> 1;
     }
}

write("cubeiter: ");
for z in cubeiter(numLocales) do write(" ", z);
writeln();

resumeVdebug("cubeiter");

forall z in cubeiter(numLocales) do  d[z] = here.id;

tagVdebug("writeln 4");

writeln ("D is ", d);

iter cubeiter2 ( n: int, id: int=0, off: int = -1) : int {
   var offset = 1;
   if (off < 0) then
      while (offset*2+id < n) do offset = offset << 1;
   else
      offset = off;

   yield id;

   if (id + offset < n ) then
     while (offset > 0) {
        for z in cubeiter(n, id+offset, offset >> 1) do yield z;
        offset = offset >> 1;
     }
}

iter cubeiter2 ( param tag: iterKind, n: int, id: int=0, off: int = -1) : int
       where tag == iterKind.standalone {
   var offset = 1;
   if (off < 0) then
      while (offset*2+id < n) do offset = offset << 1;
   else
      offset = off;

   yield id;

   coforall (ccid, shift) in id2com(id,offset) do
     if ccid < n then 
       on Locales[ccid] do
          for z in cubeiter2(n, ccid, offset >> shift, tag=iterKind.standalone)
             do yield z;
}

pauseVdebug();

// Tests not enabled
// writeln ("id2com tests: ");
// for a in id2com(0,8) do write (a, " ");
// writeln ();

resumeVdebug("cubeiter2");

forall x in cubeiter2(numLocales) do e[x] = here.id;

stopVdebug();

writeln ("E=", e);
