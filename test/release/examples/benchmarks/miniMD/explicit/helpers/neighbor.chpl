// contains functions to move atoms and build neighbor lists

use initMD;

// update ghost information
proc updateFluff() {
  for ijk in LocaleGridDom {
    on LocaleGrid[ijk] {
      const Me = Grid[ijk];
      for (Src, Dest, Neighbor, Offset, neigh) in 
        zip(Me.SrcSlice, Me.DestSlice, Grid[Me.Neighs], Me.PosOffset, Me.NeighDom) {
        // don't look at ourself
        if neigh != (0,0,0) {
          // copy over the bins' counts
          Me.Count[Dest] = Neighbor.Count[Src];
          
          // copy over fluff atoms' positions
          Me.Pos[Dest] = Neighbor.Pos[Src];

          // offset atoms if necessary
          if Offset != (0.0,0.0,0.0) {
            for (p,c) in zip(Me.Pos[Dest], Me.Count[Dest]) {
              p[1..c] += Offset;
            }
          }
        }
      }
    }
  }
}

// if atoms moved outside the box, wrap them around 
proc pbc() {
  coforall ijk in LocaleGridDom {
    on LocaleGrid[ijk] {
      const Me = Grid[ijk];
      const b = box;
      forall (p,c) in zip(Me.Pos[Me.Real],Me.Count[Me.Real]) {
        for x in p[1..c] {
          for i in 1..3 {
            if x(i) < 0 then x(i) += b(i);
            else if x(i) >= b(i) then x(i) -= b(i);
          }
        }
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

  coforall ijk in LocaleGridDom {
    on LocaleGrid[ijk] {
      const Me = Grid[ijk];
      forall (bin,posBin,r,c) in zip(Me.Bins, Me.Pos[Me.Real],Me.Real, Me.Count[Me.Real]) {
        const existing = 1..c;
        for (a,pos,i) in zip(bin[existing],posBin[existing],existing) {
          // reset neighbor list
          a.ncount = 0;

          // for each surrounding bin
          for s in stencil {
            const o = r + s;
            const existing = 1..Me.Count[o];
            for (npos,x) in zip(Me.Pos[o][existing],existing) {
              // if we're looking at ourself
              if r == o && x == i then continue; 

              // are we within range?
              const del = pos - npos;
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
    }
  }

  buildTime += neighTimer.elapsed();

  if debug then writeln("building done...");
} // end of buildNeighbors

// add an atom 'a' to bin 'b'
// resize if necessary
inline proc addatom(a : atom, x : v3, b : v3int) {
  const Target = Grid[BinToLocale[b]];
 
  // increment bin's # of atoms
  Target.Count[b] += 1;
  const end = Target.Count[b];
  
  // resize bin storage if needed
  if end >= perBinSpace.high {
    if debug then writeln("PERBIN RESIZE");
    var h = (perBinSpace.high * 1.3) : int;
    perBinSpace = {1..h};
  }
 
  // add to the end of the bin
  Target.Bins[b][end] = a;
  Target.Pos[b][end] = x;
}

// helper class for moving atoms around
class Mov {
  var Space : domain(1) = {1..50};
  var List : [Space] (atom, v3, v3int);
  var Count : int;
}

// TODO: reduce communication here
proc binAtoms() {
  var Moved : [LocaleGridDom] Mov;
  coforall ijk in LocaleGridDom {
    on LocaleGrid[ijk] {
      var ToMove = new Mov();
      Moved[ijk] = ToMove;
      const Me = Grid[ijk];
      for (bin, pos, r, c) in zip(Me.Bins, Me.Pos[Me.Real] ,Me.Real ,Me.Count[Me.Real]) {
        var cur = 1;

        // for each atom, check if moved
        // because we move from the end, this setup allows us to examine 
        // the atom pulled from the end
        while(cur <= c) {
          const destBin = coord2bin(pos[cur]);

          // atom moved
          if destBin != r { 
            ToMove.Count += 1;

            // resize storage as needed
            if ToMove.Count >= ToMove.Space.high then 
              ToMove.Space = {1..ToMove.Space.high * 2};

            ToMove.List[ToMove.Count] = (bin[cur], pos[cur], destBin);

            // replace with atom at end of list, if one exists
            if cur < c {
              bin[cur] = bin[c]; 
              pos[cur] = pos[c];
            }

            // correct bin count
            c -= 1; 
          } else cur += 1;
        }
      }
    }
  }

  // actually move the atoms
  for ijk in LocaleGridDom {
    on LocaleGrid[ijk] { 
      for (a, x, b) in Moved[ijk].List[1..Moved[ijk].Count] {
        addatom(a,x,b);
      }
      delete Moved[ijk];
    }
  }
}

// compute atom's correct bin based on its physical position
proc coord2bin(x : v3){
  var cur : v3int;

  // create a simple mask on a per-dimension basis
  var mask : v3int;
  for i in 1..3 do 
    mask(i) = if x(i) >= box(i) then 1 else 0;

  // if the position has drifted outside the physical space, 
  // adjust. Divide position by the size of a bin, and add 
  // (1,1,1) so we're starting at the lowest possible bin
  const temp = (x - box*mask) * bininv + (1,1,1);

  // can't cast from 3*real to 3*int (yet?)
  for i in 1..3 do 
    cur(i) = temp(i) : int;

  return cur;
}
