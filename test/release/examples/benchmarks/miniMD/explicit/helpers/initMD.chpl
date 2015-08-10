// defines global variables, initialization methods, and some helper functions

use Time;
use thermo;
use forces;
use neighbor;
use BlockDist;

// used in testing system
config param printOriginal = false;
config param printPerf = true;
config param printCorrect = false;

// Stores user-configurable variables
// See printHelp for more information
config var input_file = "in.lj.miniMD";
config var numSteps = 100;
config var size = 32;
config var num_bins = -1;
config var units = "lj";
config var force = "lj";
config var data_file = "";

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

record atom {
  // velocity, force
  var v, f : v3; 

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

// set dtforce from input file or cmd line
dtforce = .5 * dt;

// generate our own atoms, or read from a data file?
const generating = data_file == "" || data_file == "none";

const cutneighsq = cutneigh * cutneigh;

// io for reading the data file
var dataFile : file;
var dataReader : channel(false, iokind.dynamic,false);

// no data file, use input file to generate uniform lattice
if generating {

  // let the density inform box size
  const lattice : real = (4.0 / density) ** (1.0 / 3.0);
  box = problemSize * (lattice,lattice,lattice);
  volume = box(1) * box(2) * box(3);

  boxhi = box;
  numAtoms = (4 * problemSize(1) * problemSize(2) * problemSize(3)) : int;
  
  // compute the number of bins we need in each direction
  for i in 1..3 do 
    numBins(i) = (5.0/6.0 * problemSize(i)) : int;
} else {
  dataFile = open(data_file, iomode.r);
  dataReader = dataFile.reader();

  dataReader.readln(); // skip first line

  numAtoms = dataReader.readln(int);

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
  var mass_type: int; 
  dataReader.readln(mass_type, mass);
  
  // density overriden if data file provided
  density = numAtoms / (volume);
  const nbs : real = (density * 16) ** (1.0/3.0 : real);
  for i in 1..3 do
    numBins(i) = (box(i) / nbs) : int;
}

initThermo();

// user-defined
if num_bins > 0 {
  numBins = (num_bins,num_bins,num_bins);
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
  var Bins : [Real] [perBinSpace] atom;           // bin and atom storage
  var Pos : [Fluff] [perBinSpace] v3;
  var Count : [Fluff] int(32);                    // # of actual atoms in a bin
  var DestSlice, SrcSlice : [NeighDom] domain(3); // local/remote ghost slices
  var Neighs : [NeighDom] (int,int,int);          // array of locale indices
  var PosOffset : [NeighDom] v3;
}

// This breaks the abstraction layer a bit. hopefully we can push this into a 
// Chapel distribution at some point. 
//
// LocaleGridDom is a set of indices that map into LocaleGrid, which 
// contains 'locale' objects with which we can use the 'on' statement 
// for actual distribution. These values are automatically calculated 
// by the Block distribution, and it was simplest to re-use them in our 
// code.
const LocaleGridDom = DistSpace._value.dist.targetLocDom;
const LocaleGrid = DistSpace._value.dist.targetLocales;

// storage across all locales
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
  dataReader.readln(string); // skip 'Atoms' line

  // read in positions
  var tempPos : [1..numAtoms] v3;
  for x in tempPos {
    var a, b : int;
    dataReader.readln(a, b, x(1),x(2),x(3));
  }

  dataReader.readln(string); // skip 'Velocities' line

  // read velocities and add 
  for x in tempPos {
    var v : v3;
    var a : int;
    dataReader.readln(a, v(1),v(2),v(3));
    addatom(new atom(v), x, coord2bin(x));
  }

  // cleanup
  dataReader.close();
  dataFile.close();
}

// setup/store slices and neighbors so we don't have to recompute them every 
// time. Our neighbor bins won't change, and we'll always need the same slices.
proc setupComms() {
  for ijk in LocaleGridDom {
    on LocaleGrid[ijk] {
      // grab our chunk of the real domain
      const MyLocDom = DistSpace._value.locDoms[ijk].myBlock;
      if debug then writeln(here, " owns ", MyLocDom);

      // expand the maximum distance we might need
      const WithFluff = MyLocDom.expand(numNeed);
      if debug then writeln(here, "'s fluff: ", WithFluff);

      // directions of our nearest neighboring locales
      const NeighDom = {-1..1, -1..1, -1..1};

      var MyChunk = new Chunk(WithFluff, MyLocDom, NeighDom);
      Grid[ijk] = MyChunk;

      // alias to save typing
      var DestSlice => MyChunk.DestSlice;
      var SrcSlice => MyChunk.SrcSlice;
      var Neighs => MyChunk.Neighs;
     
      // Create 3D slices representing an overlap
      for (Src, Dest, neigh) in zip(SrcSlice, DestSlice, NeighDom) {
        // the number of bins (numNeed) we need in a direction (neigh)
        Dest = MyLocDom.exterior(neigh * numNeed);

        // in the case of many locales, most of the time source and destination 
        // will share the same indices. The wrapping case is handled soon.
        Src = Dest;
      }

      // use so we can move atoms around more easily
      BinToLocale[MyLocDom] = ijk;

      // adjust our remote-translated slices and neighbor pointer
      // store for future use
      for (Src, Dest, PosOffset, neigh) in 
        zip(SrcSlice, DestSlice, MyChunk.PosOffset, NeighDom) {
        // no comms with ourself, can't 'continue' inside a forall
        if neigh != (0,0,0) {
          // Get the index of the neighboring Locale
          var neighbor = ijk + neigh;

          // some of our slices will overlap with 'real' cells, but slices
          // on the exterior will need their cells to be offset so we can index 
          // into the neighboring locale's bins.
          var SrcOffset = (0,0,0);

          // Adjust the neighbor index so we're actually referencing an 
          // existing locale. Also compute the offset for source slices, 
          // if necessary.
          for i in 1..3 {
            if neighbor(i) < 0 {
              neighbor(i) = LocaleGridDom.high(i);
              SrcOffset(i) = binSpace.high(i) - Dest.high(i);
            }
            else if neighbor(i) > LocaleGridDom.high(i) {
              neighbor(i) = 0;
              SrcOffset(i) = -binSpace.high(i) + binSpace.low(i) - 1;
            }
          }
          Src = Src.translate(SrcOffset);
          Neighs[neigh] = neighbor;
          
          var mask = (1.0,1.0,1.0);
          for i in 1..3 do 
            if Dest.low(i) == Src.low(i) then mask(i) = 0.0;
          
          // ghost's position offset
          PosOffset = (neigh) * box * mask;

          if debug 
            then writeln(Src, " is the remote neighbor of ", Dest);
          }
      }
    } // end of on statement
  } // end of coforall
}

proc cleanup() {
  for c in Grid do delete c;
  delete fobj;
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
  writeln("Command line Options:");
  writeln("\n  Simulation setup:");
  writeln("\t--input_file <string>:   set input file to be used (default: in.lj.miniMD)");
  writeln("\t--numSteps <int>:        set number of timesteps for simulation");
  writeln("\t--size <int>:            set linear dimension of systembox");
  writeln("\t--num_bins <int>:        set linear dimension of bin grid");
  writeln("\t--units <string>:        set units (lj or metal), see LAMMPS documentation");
  writeln("\t--force <string>:        set interaction model (lj or eam)");
  writeln("\t--data_file <string>:    read configuration from LAMMPS data file"); 
  writeln("\n  Miscellaneous:");
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
  writef("\t# density: %.6dr\n", density);
  writef("\t# Force cutoff: %.6dr\n", force_cut);
  writef("\t# neigh cutoff: %.6dr\n", cutneigh);
  writeln("\t# Neighbor bins: ", numBins(1), " ", numBins(2), " ", numBins(3));
  writeln("\t# neighbor frequency ", neigh_every);
  writef("\t# timestep size: %.6dr\n", dt );
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
        addatom(new atom(v), temp, coord2bin(temp));
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
      const Me = Grid[ijk];
      vtotDist[ijk] = + reduce forall bin in Me.Real do 
      + reduce forall ind in 1..Me.Count[bin] do Me.Bins[bin][ind].v;
    }
  }
  vtot = + reduce vtotDist;

  // get the average
  vtot /= (numAtoms,numAtoms,numAtoms);

  // adjust using average
  coforall (ijk) in LocaleGridDom {
    on LocaleGrid[ijk] {
      const Me = Grid[ijk];
      forall (bin,c) in zip(Me.Bins[Me.Real], Me.Count[Me.Real]) {
        for a in bin[1..c] {
          a.v -= vtot;
        }
      }
    }
  }

  const factor = sqrt(initialTemp / temperature());

  coforall (ijk) in LocaleGridDom {
    on LocaleGrid[ijk] {
      const fac = (factor,factor,factor);
      const Me = Grid[ijk];
      forall (bin,c) in zip(Me.Bins[Me.Real], Me.Count[Me.Real]) {
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
