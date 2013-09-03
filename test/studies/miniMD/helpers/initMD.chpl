use Time;
use thermo;
use MDForce;
use BlockDist;

// used in testing system
config param printOriginal = false;
config param printPerf = true;
config param printCorrect = false;

// Stores user-configurable variables
// See printHelp for more information
// TODO: strings vs enum for units/force?
config var input_file = "in.lj.miniMD";
config var numSteps = 100;
config var size = 32;
config var neigh_bins = -1;
config var units = "lj";
config var force = "lj";
config var data_file = "";
config var yaml_output = 1;
config var yaml_screen = false;

// user/dev utilities
config const debug = false;
config const pHelp = false;

// 3D vector type alias
type v3 = 3*real;
type v3int = 3*int;

// system variables from input files
var problemSize : v3 = (size:real,size:real,size:real);
var initialTemp = 1.44;
var density = .8442;
var dt = .005;
var dtforce = .0025;
var force_cut = 2.5; 
var cutneigh = 2.8;
var neigh_every = 20;
var thermoEvery = 100;

// Used in deterministic 'random' number generator
// Based on Park-Miller
const IA = 16807;
const IM = 2147483647;
const AM = 1.0/IM;
const IQ = 127773;
const IR = 2836;
const MASK = 123459876;

// Dot product between two vectors
inline proc dot(a,b : v3) {
  var c = a * b;
  return c(1) + c(2) + c(3);
}

inline proc dot(a,b : v3int) {
  var c = a*b;
  return c(1) + c(2) + c(3);
}

// TODO: ghosts only need position. Can we split this record up 
// in such a way that reduces the unnecessary variables used in 
// ghost storage?
//
// Idea: pull position out and define based on the fluff domain/perbinspace, 
// then define the rest with 'Real' and perbinspace
record atom {
  // position, velocity, force
  var x, v, f : v3; 

  // define storage for neighbor list, which stores the bin and index 
  // of a neighboring atom
  var nspace : domain(1) = {1..100};
  var neighs : [nspace] (v3int,int);
  var ncount : int = 0;
}

// dimensions of simulation space
var box : v3;
var volume : real;
var boxlo, boxhi : v3;

// atom storage
var numAtoms : int;

// default mass
var mass : real = 1.0; 

// number of bins that define our piece of the continguous space
var numBins : v3int;

// constants from c++ version, used in neighboring
const small = 1.0e-6;
const factor = .999;

// time for binAtoms and buildNeighbors
var buildTime : real;

// time for force computation (inside integration)
var forceTime : real;

// Time for communication
//
// Since chapel abstracts much of this away, sometimes we do communication 
// without knowing (or caring). This value may not be completely accurate, 
// but should reflect the bulk of communication that occurs.
var commTime : real; 

// will halt if true
if pHelp then printHelp();

// entry point - read configuration file if one exists
inputFile();
dtforce = .5 * dt;

// generate our own atoms, or read from a data file?
const generating = data_file == "" || data_file == "none";

const cutneighsq = cutneigh * cutneigh;

// io for reading the data file
var dataFile : file;
var dataReader : channel(false, iokind.dynamic,false);

// no data file, use input file to generate uniform lattice
if generating {
  const lattice : real = (4.0 / density) ** (1.0 / 3.0);
  box = problemSize * (lattice,lattice,lattice);
  volume = box(1) * box(2) * box(3);

  boxhi = box;
  numAtoms = (4 * problemSize(1) * problemSize(2) * problemSize(3)) : int;
	
  for i in 1..3 do 
    numBins(i) = (5.0/6.0 * problemSize(i)) : int;
} else {
  dataFile = open(data_file, iomode.r);
  dataReader = dataFile.reader();

  dataReader.readln(); // skip first line

  // we can only have 1 atom type
  var types = dataReader.readln(int);
  assert(types == 1, "You can only have one type of atom.");

  // sim dimensions
  for i in 1..3 do
    (boxlo(i), boxhi(i)) = dataReader.readln(real,real);
  box = boxhi - boxlo;
  volume = box(1) * box(2) * box(3);

  const masses = dataReader.readln(string);
  assert(masses == "Masses");
  var (mass_type, mass) = dataReader.readln(int,real);
  
  // density overriden if data file provided
  density = numAtoms / (volume);
  const nbs : real = (density * 16) ** (1.0/3.0 : real);
  for i in 1..3 do
    numBins(i) = (box(i) / nbs) : int;
}

initThermo();

// user-defined
if neigh_bins > 0 {
  numBins = (neigh_bins,neigh_bins,neigh_bins);
}

// physical size of a bin
const binsize = box / numBins;

// 1/binsize
const bininv = (1.0,1.0,1.0) / binsize;

// number of bins we need for neighboring
var numNeed : v3int;
for i in 1..3 { 
  numNeed(i) = (cutneigh * bininv(i)) : int;
  if numNeed(i) * binsize(i) < factor * cutneigh then numNeed(i) += 1;
}

// stencil that defines our bin neighbor offsets
// for example, (0,1,0) will represent the bin above us
const stencil = {-numNeed(1) .. numNeed(1),
                  -numNeed(2) .. numNeed(2),
                  -numNeed(3) .. numNeed(3)};

// Defines the problem space
const binSpace = {1..numBins(1), 1..numBins(2), 1..numBins(3)};

// The 'special sauce'. Will distribute stuff across locales
const DistSpace = binSpace dmapped Block(binSpace);

// bin storage. we can likely assume that each bin will store 
// about the same number of atoms
//
// TODO: can we deal with this on a per-locale basis? How do we deal with 
// the case of not enough space when copying inside communication?
var perBinSpace : domain(1) = {1..8};

// locale's chunk of 'bins' (consists of whole bins)
class Chunk {
  var Fluff : domain(3);                          // real and ghost bins
  var Real : domain(3);                           // real bins
  var NeighDom : domain(3);                       // nearest neighbors
  var Arr : [Fluff] [perBinSpace] atom;           // bin and atom storage
  var Count : [Fluff] int(32);                    // # of actual atoms in a bin
  var DestSlice, SrcSlice : [NeighDom] domain(3); // local/remote ghost slices
  var Neighs : [NeighDom] (int,int,int);          // array of locale indices
}

// This breaks the abstraction layer a bit. hopefully we can push this into a 
// Chapel distribution at some point. 
//
// LocaleGridDom is a set of indices that map into LocaleGrid, which 
// contains 'locale' objects with which we can use the 'on' statement 
// for actual distribution. These values are automatically calculated 
// by the Block distribution, and it was simplest to re-use them in our 
// code.
//
// TODO: Discuss implementing an interface for grabbing such useful values.
const LocaleGridDom = DistSpace._value.dist.targetLocDom;
const LocaleGrid = DistSpace._value.dist.targetLocales;

// storage across all locales
// TODO: rename to something a bit less ambiguous
var Grid : [LocaleGridDom] Chunk;

// map of bin (3*int) to LocaleGridDom index (3*int)
var BinToLocale : [DistSpace] (int,int,int);

setupComms();

var fobj : Force;

if force == "lj" {
  fobj = new ForceLJ(force_cut);
} else {
  fobj = new ForceEAM(force_cut);
  mass = fobj.mass;
}

if printOriginal then writeln("# Create System:");

// if there's no data file to read from, generate a lattice of atoms
if generating {
  create_atoms();
  create_velocity();
} else {
  dataReader.readln(); // skip 'Atoms' line

  // read in positions
  var tempPos : [1..numAtoms] v3;
  for x in tempPos {
    dataReader.read(int,int);
    dataReader.readln(x(1),x(2),x(3));
  }

  // read velocities and add 
  for x in tempPos {
    var v : v3;
    dataReader.read(int,int);
    dataReader.readln(v(1),v(2),v(3));
    addatom(new atom(x,v), coord2bin(x));
  }

  // cleanup
  dataReader.close();
  dataFile.close();
}

// setup/store slices and neighbors so we don't have to recompute them every 
// time. Our neighbor bins won't change, and we'll always need the same slices.
proc setupComms() {
  coforall (i,j,k) in LocaleGridDom {
    on LocaleGrid(i,j,k) {
      // grab our chunk of the real domain
      const MyLocDom = DistSpace._value.locDoms[i,j,k].myBlock;
      if debug then writeln(here, " owns ", MyLocDom);

      // expand the maximum distance we might need
      const WithFluff = MyLocDom.expand(numNeed);
      if debug then writeln(here, "'s fluff: ", WithFluff);

      // directions of our nearest neighboring locales
      const NeighDom = {-1..1, -1..1, -1..1};

      var MyChunk = new Chunk(Fluff=WithFluff, Real=MyLocDom, NeighDom=NeighDom);
      Grid[i,j,k] = MyChunk;

      // alias to save typing
      var Data => MyChunk.Arr;
      var DestSlice => MyChunk.DestSlice;
      var SrcSlice => MyChunk.SrcSlice;
      var Neighs => MyChunk.Neighs;
     
      // Create 3D slices representing an overlap
      forall xyz in NeighDom {
        // the number of bins (numNeed) we need in a direction (xyz)
        DestSlice[xyz] = MyLocDom.exterior(xyz * numNeed);

        // in the case of many locales, most of the time source and destination 
        // will share the same indices. The wrapping case is handled soon.
        SrcSlice[xyz] = DestSlice[xyz];
      }

      // use so we can move atoms around more easily
      BinToLocale[MyLocDom] = (i,j,k);

      // adjust our remote-translated slices and neighbor pointer
      // store for future use
      forall xyz in NeighDom {
        // no comms with ourself, can't 'continue' inside a forall
        if xyz != (0,0,0) {
          // Get the index of the neighboring Locale
          var neighbor = (i,j,k) + xyz;

          // some of our slices will overlap with 'real' cells, but slices
          // on the exterior will need their cells to be offset so we can index 
          // into the neighboring locale's bins.
          var SrcOffset = (0,0,0);

          // Adjust the neighbor index so we're actually referencing an 
          // existing locale. Also compute the offset for source slices, 
          // if necessary.
          //
          // TODO: assign wslice inside for code readablility?
          for i in 1..3 {
            if neighbor(i) < 0 {
              neighbor(i) = LocaleGridDom.high(i);
              SrcOffset(i) = binSpace.high(i)-DestSlice[xyz].high(i);
            }
            else if neighbor(i) > LocaleGridDom.high(i) {
              neighbor(i) = 0;
              SrcOffset(i) = -binSpace.high(i)+binSpace.low(i)-1;
            }
          }
          SrcSlice[xyz] = SrcSlice[xyz].translate(SrcOffset);
          Neighs[xyz] = neighbor;

          if debug 
            then writeln(SrcSlice[xyz], " is the remote neighbor of ", DestSlice[xyz]);
          }
      }
    } // end of on statement
  } // end of coforall
}

// Reads an input file
proc inputFile() {
  var err : syserr;
  var fchan = open(err, input_file, iomode.r);

  if err then {
    input_file = "none";
    return;
  }

  var r = fchan.reader();

  // skip first line
  r.readln();

  // get unit type ('lj' vs 'metal')
  units = r.readln(string);

  // use data file?
  data_file = r.readln(string);

  // get force type : 'lj' vs 'eam'
  force = r.readln(string);

  // 'size of problem'
  problemSize = r.readln(int,int,int);
  
  // # iterations
  numSteps = r.readln(int);

  // simulated time step size
  dt = r.readln(real);

  // starting temperature
  initialTemp = r.readln(real);

  // start density (overriden if data file), used in EAM
  density = r.readln(real);

  // recompute neighbors every N iterations
  neigh_every = r.readln(int);

  // force cutoff: used to determine significance of force computation
  // neighbor cutoff: used to determine if a nearby atom may be significant
  (force_cut,cutneigh) = r.readln(real, real);

  // do thermo compute every N iterations
  thermoEvery = r.readln(int);

  // cutneigh = skin + force_cutoff, used in neighboring algorithm
  cutneigh += force_cut;

  // cleanup
  r.close();
  fchan.close();
}

proc printHelp() {
  writeln("Commandline Options:");
  writeln("\n  Chapel help:");
  writeln("\t--chapel:                Show Chapel's command line options");
  writeln("\n  Execution configuration:");
  writeln("\n  Simulation setup:");
  writeln("\t--input_file <string>:   set input file to be used (default: in.lj.miniMD)");
  writeln("\t--numSteps <int>:        set number of timesteps for simulation");
  writeln("\t--size <int>:            set linear dimension of systembox");
  writeln("\t--neigh_bins <int>:      set linear dimension of neighbor bin grid");
  writeln("\t--units <string>:        set units (lj or metal), see LAMMPS documentation");
  writeln("\t--force <string>:        set interaction model (lj or eam)");
  writeln("\t--data_file <string>:    read configuration from LAMMPS data file"); 
  writeln("\n  Miscelaneous:");
  writeln("\t                                within rcut_neighbor (outer force cutoff)");
  writeln("\t--yaml_output <int>:     level of yaml output (default 1)");
  writeln("\t--yaml_screen:                write yaml output also to screen");
  writeln("\t--pHelp:                  display this help message");
  writeln("\t--------------------------------------------------"); 
  exit(0);
}

// Print simulation settings
proc printSim() {
  if !printOriginal then return;
  writeln("# Done .... ");
  writeln("# miniMD-Reference 0.1 (Chapel) output ...");
  writeln("# Systemparameters: ");
  writeln("\t# input_file: ", input_file);
  if data_file != "" then 
    writeln("\t# datafile: ", data_file);
  else
    writeln("\t# datafile: none");
  writeln("\t# force: ", force);
  write("\t# units: ");
  if units == "metal" then writeln("METAL");
  else writeln("LJ");
  writeln("\t# atoms: ", numAtoms);
  write("\t# System size: ");
  writef("%.2dr %.2dr %.2dr (unit cells: %i %i %i)\n", 
      box(1), box(2), box(3), problemSize(1), problemSize(2), problemSize(3));
  writef("\t%# density: %.6dr\n", density);
  writef("\t%# Force cutoff: %.6dr\n", force_cut);
  writef("\t%# neigh cutoff: %.6dr\n", cutneigh);
  writeln("\t# Neighbor bins: ", numBins(1), " ", numBins(2), " ", numBins(3));
  writeln("\t# neighbor frequency ", neigh_every);
  writef("\t%# timestep size: %.6dr\n", dt );
  writeln("\t# thermo frequency: ", thermoEvery);
}

proc create_atoms() {
  const lattice : real = (4.0 / density) ** (1.0 / 3.0);

  var lo, hi : v3int;

  for i in 1..3 {
    lo(i) = (boxlo(i) / (.5 * lattice)) : int;
    hi(i) = (boxhi(i) / (.5 * lattice)) : int;
  }

  for i in 1..3 {
    lo(i) = max(lo(i), 0);
    hi(i) = min(hi(i), 2 * problemSize(i) - 1) : int;
  }

  var temp, v : v3;
  var s, o : v3int;
  var m, n : int;
  var curCoord : v3int;
  var subboxdim = 8;
  var flag = false;

  while o(3) * subboxdim <= hi(3) {
    curCoord = (o * (subboxdim,subboxdim,subboxdim) + s);

    if flag then continue;

    var withinBounds = true;
    for i in 1..3 {
      withinBounds = withinBounds && curCoord(i) >= lo(i) && curCoord(i) <= hi(i);
    }

    if ((curCoord(1) + curCoord(2) + curCoord(3)) % 2 == 0) && withinBounds {
      for i in 1..3 do
        temp(i) = .5 * lattice * curCoord(i);

      withinBounds = true;
      for i in 1..3 {
        withinBounds = withinBounds && temp(i) >= boxlo(i) && temp(i) < boxhi(i);
      }
                          
      if withinBounds {
        n = (curCoord(3) * (2 * problemSize(2)) * (2*problemSize(1)) + curCoord(2) * (2 * problemSize(1)) + curCoord(1) + 1) : int;
        for i in 1..3 {
          for m in 1..5 { pmrand(n); }
          v(i) = pmrand(n);
        }
        addatom(new atom(temp,v), coord2bin(temp));
      }
    }

    s(1) += 1;

    if s(1) == subboxdim {
      s(1) = 0;
      s(2) += 1;
    }

    if s(2) == subboxdim {
      s(2) = 0;
      s(3) += 1;
    }

    if s(3) == subboxdim {
      s(3) = 0;
      o(1) += 1;
    }

    if o(1) * subboxdim > hi(1) {
      o(1) = 0;
      o(2) += 1;
    }

    if o(2) * subboxdim > hi(2) {
      o(2) = 0;
      o(3) += 1;
    }
  }
}

proc create_velocity() {
  var vtot : v3;

  // find the total velocity
  var vtotDist : [LocaleGridDom] v3;
  coforall (ijk) in LocaleGridDom {
    on LocaleGrid[ijk] {
      vtotDist[ijk] = + reduce forall bin in Grid[ijk].Real do 
      + reduce forall ind in 1..Grid[ijk].Count[bin] do Grid[ijk].Arr[bin][ind].v;
    }
  }
  vtot = + reduce vtotDist;

  // get the average
  vtot /= (numAtoms,numAtoms,numAtoms);

  // adjust using average
  coforall (ijk) in LocaleGridDom {
    on LocaleGrid[ijk] {
      var Real = Grid[ijk].Real;
      forall (bin,c) in zip(Grid[ijk].Arr[Real], Grid[ijk].Count[Real]) {
        for a in bin[1..c] {
          a.v -= vtot;
        }
      }
    }
  }

  const factor = sqrt(initialTemp / temperature());

  coforall (ijk) in LocaleGridDom {
    on LocaleGrid[ijk] {
      var fac = (factor,factor,factor);
      var Real = Grid[ijk].Real;
      forall (bin,c) in zip(Grid[ijk].Arr[Real], Grid[ijk].Count[Real]) {
        for a in bin[1..c] {
          a.v *= fac;
        }
      }
    }
  }
}

// Park/Miller RNG w/out MASKING
proc pmrand(ref n : int) : real {
  var k : int;
  var ans : real;

  k = n / IQ;
  n = IA * (n - k * IQ) - IR * k;
  if n < 0 then n += IM;
  ans = AM * n;
  return ans;
}

// TODO: zipper variables relying on NeighDom
proc updateFluff() {
  coforall ijk in LocaleGridDom {
    on LocaleGrid[ijk] {
      for neigh in Grid[ijk].NeighDom {
        // don't look at ourself
        if neigh == (0,0,0) then continue;

        // ease of typing
        var Data => Grid[ijk].Arr;
        var Neighs => Grid[ijk].Neighs;
        var DestSlice => Grid[ijk].DestSlice;
        var SrcSlice => Grid[ijk].SrcSlice;

        // copy over the bins' counts
        Grid[ijk].Count[DestSlice[neigh]] = Grid[Neighs[neigh]].Count[SrcSlice[neigh]];
        
        // if a dimension of the bin exists in adjacent 'real' chunk of bins, 
        // we shouldn't offset
        var dimtest = (1.0,1.0,1.0);
        for i in 1..3 do 
          if DestSlice[neigh].low(i) == SrcSlice[neigh].low(i) then dimtest(i) = 0.0;
        
        // ghost's position offset
        const offset = (neigh) * box * dimtest;

        // for every atom in every bin, clone & wrap the position
        forall (D, S) in zip(DestSlice[neigh], SrcSlice[neigh]) {
          const c = Grid[ijk].Count[D];
          for (loc, rem) in zip(Data[D][1..c], Grid[Neighs[neigh]].Arr[S][1..c]) {
            loc.x = rem.x + (offset);
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
      var Real = Grid[ijk].Real;
      forall (b,c) in zip(Grid[ijk].Arr[Real],Grid[ijk].Count[Real]) {
        for a in b[1..c] {
          for i in 1..3 {
            if a.x(i) < 0 then a.x(i) += box(i);
            else if a.x(i) >= box(i) then a.x(i) -= box(i);
          }
        }
      }
    }
  }
}

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
      var Real = Grid[ijk].Real;
      var Data => Grid[ijk].Arr;
      forall (b,r,c) in zip(Data[Real],Real, Grid[ijk].Count[Real]) {
        for (a,i) in zip(b[1..c],1..c) {
          // reset neighbor list
          a.ncount = 0;

          // for each surrounding bin
          for s in stencil {
            const o = r + s;
            for (n,x) in zip(Data[o][1..Grid[ijk].Count[o]],1..Grid[ijk].Count[o]) {
              // if we're looking at ourself
              if r == o && x == i then continue; 

              // are we within range?
              const del = a.x - n.x;
              const rsq = dot(del,del);
              if rsq <= cutneighsq {
                a.ncount += 1;

                // resize neighbor list if necessary
                var h = a.nspace.high;
                while a.ncount > h { 
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
inline proc addatom(a : atom, b : v3int) {
  const loc = BinToLocale[b];
  Grid[loc].Count[b] += 1;
  if Grid[loc].Count[b] >= perBinSpace.high {
    var h = (perBinSpace.high * 1.5) : int;
    perBinSpace = {1..h};
  }
  Grid[loc].Arr[b][Grid[loc].Count[b]] = a;
}

// helper class for moving bins around
class Mov {
  var Space : domain(1) = {1..50};
  var List : [Space] (atom, v3int);
  var Count : int;
}

// TODO: reduce communication here
proc binAtoms() {
  var Moved : [LocaleGridDom] Mov;
  coforall ijk in LocaleGridDom {
    on LocaleGrid[ijk] {
      var M = new Mov();
      Moved[ijk] = M;
      var Real = Grid[ijk].Real;
      for (b,r,c) in zip(Grid[ijk].Arr[Real] ,Real ,Grid[ijk].Count[Real]) {
        var cur = 1;

        // for each atom, check if moved
        // because we move from the end, this setup allows us to examine 
        // the atom pulled from the end
        while(cur <= c) {
          const destBin = coord2bin(b[cur].x);

          // atom moved
          if destBin != r { 
            M.Count += 1;

            // resize storage as needed
            if M.Count >= M.Space.high then 
              M.Space = {1..M.Space.high * 2};

            M.List[M.Count] = (b[cur], destBin);

            // replace with atom at end of list, if one exists
            if cur < c then b[cur] = b[c]; 

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
      for (a, b) in Moved[ijk].List[1..Moved[ijk].Count] {
        addatom(a,b);
      }
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
