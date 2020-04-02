// contains functions to move atoms and build neighbor lists

use initMD;
use Time;

// update ghost information
proc updateFluff() {
  if useStencilDist {
    Pos.updateFluff();
    Count.updateFluff();

    // offset positions
    // boundaries() yields a periodic array element, and the 
    // neighbor panel value
    forall (pos, N) in Pos.boundaries() {
      const PO = PosOffset[N];
      for p in pos do p += PO;
    }
  } else {
    forall (P, D, S) in zip(PosOffset, Dest, Src) {
      Pos[D] = Pos[S];
      Count[D] = Count[S];

      // offset positions
      forall d in D do
        Pos[d][1..Count[d]] += P;
    }
  }
}

// if atoms moved outside the box, wrap them around 
proc pbc() {
  forall (pos, c) in zip(RealPos, RealCount) {
    for x in pos[1..c] {
      for i in 0..2 {
        if x(i) < 0 then x(i) += box(i);
        else if x(i) >= box(i) then x(i) -= box(i);
      }
    }
  }
}

// put atoms into correct bins and rebuild neighbor lists
proc buildNeighbors() {
  if debug then writeln("starting to build...");

  var neighTimer : Timer;
  neighTimer.start();
 
  // enforce boundaries
  pbc();

  commTime += neighTimer.elapsed();
  neighTimer.stop();
  neighTimer.clear();
  neighTimer.start();

  // if any atoms moved outside a bin, relocate
  binAtoms();

  buildTime += neighTimer.elapsed();
  neighTimer.stop();
  neighTimer.clear();
  neighTimer.start();

  if debug then writeln("starting comms...");

  // grab ghost/overlapping atoms
  updateFluff();

  if debug then writeln("comms done...");

  commTime += neighTimer.elapsed();
  neighTimer.stop();
  neighTimer.clear();
  neighTimer.start();

  forall (bin, pos, r, c) in zip(Bins, RealPos, 
                                 binSpace, RealCount) {
    const existing = 1..c;
    for (a, p, i) in zip(bin[existing], pos[existing], existing) {
      a.ncount = 0;

      for s in stencil {
        const o = r + s;
        const existing = 1..Count[o];

        for (n, x) in zip(Pos[o][existing], existing) {
          if r == o && x == i then continue; 

          // are we within range?
          const del = p - n;
          const rsq = dot(del,del);
          if rsq <= cutneighsq {
            a.ncount += 1;

            // resize neighbor list if necessary
            var h = a.nspace.high;
            while a.ncount > h { 
              if debug then writeln("NSPACE RESIZE");
              h = ((a.nspace.high*1.2) : int);
              a.nspace = {1..h};
            }
            // store atom's bin and index
            a.neighs[a.ncount] = (o,x);
          }
        }
      }
    }
  }

  buildTime += neighTimer.elapsed();

  if debug then writeln("building done...");
} // end of buildNeighbors

// add an atom 'a' to bin 'b'
// resize if necessary
// 
// We use the 'ref' intent in order to avoid shallow copy bugs.
// See test/types/records/bharshbarger/remote*Record.chpl for more.
inline proc addatom(ref a : atom, x : v3, b : v3int) {
  // increment bin's # of atoms
  const end = RealCount[b] + 1;
  RealCount[b] = end;
  
  // resize bin storage if needed
  if end >= perBinSpace.high {
    if debug then writeln("PERBIN RESIZE");
    var h = (perBinSpace.high * 1.3) : int;
    perBinSpace = {1..h};
  }
 
  // add to the end of the bin
  Bins[b][end].v = a.v;
  Bins[b][end].f = a.f;
  RealPos[b][end] = x;
}

proc binAtoms() {
  for (bin, pos, c, r) in zip(Bins, RealPos, RealCount, binSpace) {
    var cur = 1;

    // for each atom, check if moved
    // because we move from the end, this setup allows us to examine 
    // the atom pulled from the end
    while(cur <= c) {
      const destBin = coord2bin(pos[cur]);

      // atom moved
      if destBin != r { 
        addatom(bin[cur], pos[cur], destBin);

        // replace with atom at end of list, if one exists
        if cur < c {
          bin[cur].v = bin[c].v;
          bin[cur].f = bin[c].f;
          pos[cur] = pos[c];
        }

        // correct bin count
        c -= 1; 
      } else cur += 1;
    }
  }
}

// compute atom's correct bin based on its physical position
proc coord2bin(x : v3){
  var cur : v3int;

  // create a simple mask on a per-dimension basis
  var mask : v3int;
  for i in 0..2 do
    mask(i) = if x(i) >= box(i) then 1 else 0;

  // if the position has drifted outside the physical space, 
  // adjust. Divide position by the size of a bin, and add 
  // (1,1,1) so we're starting at the lowest possible bin
  const temp = (x - box*mask) * bininv + (1,1,1);

  // can't cast from 3*real to 3*int (yet?)
  for i in 0..2 do
    cur(i) = temp(i) : int;

  return cur;
}
