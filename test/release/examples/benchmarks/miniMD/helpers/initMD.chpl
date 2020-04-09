// defines global variables, initialization methods, and some helper functions

use Time;
use thermo;
use forces;
use neighbor;
public use StencilDist;
public use BlockDist;
private use IO;

// used in testing system
config param printOriginal = false;
config param printPerf = true;
config param printCorrect = false;

// if the user wants multilocale, use the Block distribution
config param useStencilDist = false;
config param useBlockDist = (CHPL_COMM != "none" && !useStencilDist);

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
  return c(0) + c(1) + c(2);
}

inline proc dot(a,b : v3int) {
  var c = a*b;
  return c(0) + c(1) + c(2);
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

// number of bins that define our piece of the contiguous space
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

  numAtoms = (4 * problemSize(0) * problemSize(1) * problemSize(2)) : int;

  // let the density inform box size
  const lattice : real = (4.0 / density) ** (1.0 / 3.0);
  box = problemSize * (lattice,lattice,lattice);
  volume = box(0) * box(1) * box(2);

  boxhi = box;

  // compute the number of bins we need in each direction
  for i in 0..2 do
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
  for i in 0..2 do
    (boxlo(i), boxhi(i)) = dataReader.readln(real,real);
  box = boxhi - boxlo;
  volume = box(0) * box(1) * box(2);

  const masses = dataReader.readln(string);
  assert(masses == "Masses");
  var mass_type: int;
  dataReader.readln(mass_type, mass);

  // density overridden if data file provided
  density = numAtoms / (volume);
  const nbs : real = (density * 16) ** (1.0/3.0 : real);
  for i in 0..2 do
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
for i in 0..2 {
  numNeed(i) = (cutneigh * bininv(i)) : int;
  if numNeed(i) * binsize(i) < factor * cutneigh then numNeed(i) += 1;
}

// stencil that defines our bin neighbor offsets
// for example, (0,1,0) will represent the bin above us
const stencil = {-numNeed(0) .. numNeed(0),
                  -numNeed(1) .. numNeed(1),
                  -numNeed(2) .. numNeed(2)};

// Defines the problem space
const binSpace = {1..numBins(0), 1..numBins(1), 1..numBins(2)};
const ghostSpace = binSpace.expand(numNeed);

// Will define the bounds of our arrays and distribute across locales
const DistSpace = if useBlockDist then ghostSpace dmapped Block(ghostSpace)
                  else if useStencilDist then
                   binSpace dmapped Stencil(binSpace, fluff=numNeed, periodic=true)
                  else ghostSpace;

const Space = if useBlockDist then binSpace dmapped Block(binSpace)
              else if useStencilDist then binSpace dmapped Stencil(binSpace)
              else binSpace;

// bin storage. we can likely assume that each bin will store
// about the same number of atoms, and that the size won't
// change much after initialization
var perBinSpace : domain(1) = {1..8};

// points to nearest neighbors
//
// Note: diverges from the C++ version, and may be corrected
// in a later revision of this code
const NeighDom = {-1..1, -1..1, -1..1};

// atom positions
var Pos: [DistSpace] [perBinSpace] v3;
ref RealPos = if useStencilDist then Pos.noFluffView()
              else Pos[binSpace];

// atom velocity, force, and neighbor lists
var Bins: [Space] [perBinSpace] atom;

// bin counts
var Count: [DistSpace] int(32);
ref RealCount = if useStencilDist then Count.noFluffView()
                else Count[binSpace];

// offsets used to wrap ghosts
var PosOffset: [NeighDom] v3;

// for ease of copying ghosts around
var Dest, Src: [NeighDom] domain(3);

setupComms();

var fobj : owned Force =
  if force == "lj" then new ForceLJ(force_cut): owned Force
                   else new ForceEAM(force_cut);

if force != "lj" then
  mass = fobj.mass;

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
    dataReader.readln(a ,b, x(0),x(1),x(2));
  }

  dataReader.readln(string); // skip 'Velocities' line

  // read velocities and add
  for x in tempPos {
    var v : v3;
    var a : int;
    dataReader.readln(a, v(0),v(1),v(2));
    var ta = new atom(v);
    addatom(ta, x, coord2bin(x));
  }

  // cleanup
  dataReader.close();
  dataFile.close();
}

// setup/store slices and neighbors so we don't have to recompute them every
// time. Our neighbor bins won't change, and we'll always need the same slices.
proc setupComms() {
  forall (P, D, S, N) in zip(PosOffset, Dest, Src, NeighDom) {
    P = N * box;

    if !useStencilDist && N != (0,0,0) {
      D = binSpace.exterior(N * numNeed); // section of ghosts
      S = D.translate(-N * numBins); // map to binSpace

      if debug then writeln("Destination ", D, " maps to source ", S);
    }
  }
}

// Reads an input file
proc inputFile() {
  var err : syserr;
  var fchan: file;
  try {
    fchan = open(input_file, iomode.r);
  } catch {
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

  // start density (overridden if data file), used in EAM
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
      box(0), box(1), box(2), problemSize(0), problemSize(1), problemSize(2));
  writef("\t# density: %.6dr\n", density);
  writef("\t# Force cutoff: %.6dr\n", force_cut);
  writef("\t# neigh cutoff: %.6dr\n", cutneigh);
  writeln("\t# Neighbor bins: ", numBins(0), " ", numBins(1), " ", numBins(2));
  writeln("\t# neighbor frequency ", neigh_every);
  writef("\t# timestep size: %.6dr\n", dt );
  writeln("\t# thermo frequency: ", thermoEvery);
}

proc create_atoms() {
  const lattice : real = (4.0 / density) ** (1.0 / 3.0);

  var lo, hi : v3int;

  for i in 0..2 {
    lo(i) = (boxlo(i) / (.5 * lattice)) : int;
    hi(i) = (boxhi(i) / (.5 * lattice)) : int;
  }

  for i in 0..2 {
    lo(i) = max(lo(i), 0);
    hi(i) = min(hi(i), 2 * problemSize(i) - 1) : int;
  }

  var temp, v : v3;
  var s, o : v3int;
  var m, n : int;
  var curCoord : v3int;
  var subboxdim = 8;
  var flag = false;

  while o(2) * subboxdim <= hi(2) {
    curCoord = (o * (subboxdim,subboxdim,subboxdim) + s);

    if flag then continue;

    var withinBounds = true;
    for i in 0..2 {
      withinBounds = withinBounds && curCoord(i) >= lo(i) && curCoord(i) <= hi(i);
    }

    if ((curCoord(0) + curCoord(1) + curCoord(2)) % 2 == 0) && withinBounds {
      for i in 0..2 do
        temp(i) = .5 * lattice * curCoord(i);

      withinBounds = true;
      for i in 0..2 {
        withinBounds = withinBounds && temp(i) >= boxlo(i) && temp(i) < boxhi(i);
      }

      if withinBounds {
        n = (curCoord(2) * (2 * problemSize(1)) * (2*problemSize(0)) + curCoord(1) * (2 * problemSize(0)) + curCoord(0) + 1) : int;
        for i in 0..2 {
          for m in 1..5 { pmrand(n); }
          v(i) = pmrand(n);
        }
        var ta = new atom(v);
        addatom(ta, temp, coord2bin(temp));
      }
    }

    s(0) += 1;

    if s(0) == subboxdim {
      s(0) = 0;
      s(1) += 1;
    }

    if s(1) == subboxdim {
      s(1) = 0;
      s(2) += 1;
    }

    if s(2) == subboxdim {
      s(2) = 0;
      o(0) += 1;
    }

    if o(0) * subboxdim > hi(0) {
      o(0) = 0;
      o(1) += 1;
    }

    if o(1) * subboxdim > hi(1) {
      o(1) = 0;
      o(2) += 1;
    }
  }
  if useStencilDist then Count.updateFluff();
}

proc create_velocity() {
  var vtot : v3;

  // find the total velocity
  vtot = + reduce forall (bin, c) in zip(Bins, RealCount) do
    + reduce forall a in bin[1..c] do a.v;

  // get the average
  vtot /= numAtoms;

  // adjust using average
  forall (bin, c) in zip(Bins, RealCount) do
    for a in bin[1..c] do a.v -= vtot;

  const factor = sqrt(initialTemp / temperature());

  forall (bin, c) in zip(Bins, RealCount) do
    for a in bin[1..c] do a.v *= factor;
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
