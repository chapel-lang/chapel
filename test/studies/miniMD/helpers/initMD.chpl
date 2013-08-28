use Time;
use thermo; // used in case where we build our own atoms
use MDForce;
use BlockDist;

// used in testing system
config param printOriginal = false;
config param printPerf = true;
config param printCorrect = false;

// Stores user-configurable variables
config var half_neigh = true;
config var ghost_newton = true;
config var input_file = "in.lj.miniMD";
config var nsteps = 100;
config var size = 32;
config var neigh_bins = -1;
config var units = "lj";
config var force = ForceStyle.FORCELJ;
config var data_file = "";
config var yaml_output = 1;
config var yaml_screen = false;
config var debug = false;

type v3 = 3*real;
type v3int = 3*int;

// system variables from input files
var psize : v3 = (size:real,size:real,size:real);
var t_request = 1.44;
var rho = .8442;
var dt = .005;
var dtforce = .0025;
var force_cut = 2.5; 
var cutneigh = 2.8;
var neigh_every = 20;
var nstat = 100;

const IA = 16807;
const IM = 2147483647;
const AM = 1.0/IM;
const IQ = 127773;
const IR = 2836;
const MASK = 123459876;

proc dot(a,b : v3) {
	var c = a * b;
	return c(1) + c(2) + c(3);
}

proc dot(a,b : v3int) {
	var c = a*b;
	return c(1) + c(2) + c(3);
}

type binidx = ((int,int,int),int);

record atom {
	var x, v, f : v3; // position, velocity, force
	var nspace : domain(1) = {1..100};
	var neighs : [nspace] binidx;
	var ncount : int = 0;
}

// dimensions of simulation space
var dim : v3;
var dimlo, dimhi : v3;

// atom storage
var natoms : int;

var mass : real = 1.0; // default mass

// neighbor information
var nbin : v3int;
var binsize : v3;
var bininv : v3;
var next : v3int;


// bounds of bins, # in each direction, total #
var mbinlo : v3int;
var mbinhi : v3int;
var mbin : v3int;
var mbins : int;

// constants from c++ version, used in neighboring
const small = 1.0e-6;
const factor = .999;

var stencilSpace : domain(1);
var stencil : [stencilSpace] (int,int,int);
var nstencil : int;
var cutneighsq : real;
var buildTime : real; // total buildNeighbors time (minus pbc and findghosts, so we can mimic c++)
var commTime : real; // sort of a misnomer, but measures pbc and findghosts time

inputFile();

const generating = data_file == "" || data_file == "none";

var fobj : Force;
if force == ForceStyle.FORCELJ {
	fobj = new ForceLJ(force_cut);
} else {
	fobj = new ForceEAM(force_cut);
	if printOriginal then writeln("# EAM currently requires '--ghost_newton 0'; Changing setting now.");
	ghost_newton = false;
}
if printOriginal then writeln("# Create System:");

/* ---------- Initialize System ----------*/
if fobj.style == ForceStyle.FORCEEAM then mass = fobj.mass;
cutneighsq = cutneigh * cutneigh;

var indata : file;
var dreader : channel(false, iokind.dynamic,false);
// no data file, use input file to generate uniform lattice
if generating {
	const lattice : real = (4.0 / rho) ** (1.0 / 3.0);
	dim = psize * lattice;

	dimhi = dim;
	natoms = (4 * psize(1) * psize(2) * psize(3)) : int;

	const ns = 5.0/6.0;
	nbin(1) = (ns * psize(1)) : int;
	nbin(2) = (ns * psize(2)) : int;
	nbin(3) = (ns * psize(3)) : int;
} else {
	indata = open(data_file, iomode.r);
	dreader = indata.reader();

	dreader.readln(); // skip first line

	// more complex LAMMPS files can have multiple atom types
	// as far as I can tell, c++ miniMD takes only 1 type
	var types = dreader.readln(int);
	assert(types == 1, "You can only have one type of atom.");

	// sim dimensions
	(dimlo(1), dimhi(1)) = dreader.readln(real,real);
	(dimlo(2), dimhi(3)) = dreader.readln(real,real);
	(dimlo(3), dimhi(3)) = dreader.readln(real,real);
	dim = dimhi - dimlo;

	var masses = dreader.readln(string);
	assert(masses == "Masses");
	var (mass_type, mass) = dreader.readln(int,real);
	
	// density overriden if data file provided
	rho = natoms / ( dim(1) * dim(2) * dim(3));
	var nbs : real = (rho * 16) ** (1.0/3.0 : real);
	nbin(1) = (dim(1) / nbs) : int;
	nbin(2) = (dim(2) / nbs) : int;
	nbin(3) = (dim(3) / nbs) : int;
}

if neigh_bins > 0 {
	nbin = (neigh_bins,neigh_bins,neigh_bins);
}

setupNeighbors();
dtforce = .5 * dt;
initThermo();

const MaxSpace = {1..mbin(1), 1..mbin(2), 1..mbin(3)};

// the space in which our 'real' atoms will live. 
// TODO: there's probably a better way to define this 
const binSpace = {mbinhi(1) - nbin(1) + 1 .. mbinhi(1), 
									mbinhi(2) - nbin(2) + 1 .. mbinhi(2), 
									mbinhi(3) - nbin(3) + 1 .. mbinhi(3)};
// The 'special sauce'. Will distribute stuff across locales
const DistSpace = binSpace dmapped Block(binSpace);
var perBinSpace : domain(1) = {1..8};
var CountSpace : domain(3) = binSpace;
var binCount : [CountSpace] int(32);

// locale's chunk of 'bins' (consists of whole bins)
class Chunk {
	var Dom : domain(3);
	var Real : domain(3);
	var NeighDom : domain(rank=3, stridable=true);
	var Arr : [Dom] [perBinSpace] atom;
	var NSlice, WSlice : [NeighDom] domain(3);
	var Neighs : [NeighDom] (int,int,int);
}

// use for naive barriers
var takeTurn$ : sync int = 0;

// this breaks the abstraction layer a bit. hopefully we can push this into a 
// Chapel distribution at some point
const LocaleGridDom = DistSpace._value.dist.targetLocDom;
const LocaleGrid = DistSpace._value.dist.targetLocales;

var Bins : [LocaleGridDom] Chunk;
var BinToLocale : [DistSpace] (int,int,int);

const maxBins = max(nbin(1),nbin(2),nbin(3));
setupComms();

// actually generate/store atoms
if generating {
	create_atoms();
	create_velocity(t_request);
} else {
	dreader.readln(); // skip 'Atoms' line
	var tempPos : [1..natoms] v3;
	for x in tempPos {
		dreader.read(int,int);
		dreader.readln(x(1),x(2),x(3));
	}
	for x in tempPos {
		var v : v3;
		dreader.read(int,int);
		dreader.readln(v(1),v(2),v(3));
		addatom(new atom(x,v), coord2bin(x));
	}
	dreader.close();
	indata.close();
}

/* --------- init/helper functions ----------------*/

// TODO: better names? also make sure here.id is accurate
inline proc startBarrier() { while (takeTurn$.readXX() != here.id) {} }
inline proc stopBarrier() { takeTurn$.writeXF((here.id + 1) % numLocales); }

// setup/store slices and neighbors so we don't have to recompute them every 
// time. Our neighbors won't change, and we'll always need the same slices.
proc setupComms() {
	CountSpace = binSpace.expand(next);
	coforall (i,j,k) in LocaleGridDom {
		on LocaleGrid(i,j,k) {
			// grab our chunk of the real domain
			const MyLocDom = DistSpace._value.locDoms[i,j,k].myBlock;
			if debug then writeln(here, " owns ", MyLocDom);

			// expand the maximum distance we might need
			// TODO: I think expand(nbin) would be OK, let's try the naive case first
			const WithFluff = MyLocDom.expand(next);
			if debug then writeln(here, "'s fluff: ", WithFluff);

			const ND = {-next(1) .. next(1) by next(1),-next(2) .. next(2) by next(2),-next(3) .. next(3) by next(3)};
			startBarrier();
			var MyChunk = new Chunk(Dom=WithFluff, Real=MyLocDom, NeighDom=ND);
			stopBarrier();

			Bins[i,j,k] = MyChunk;

			// alias to save typing
			var Data => MyChunk.Arr;
			var NSlice => MyChunk.NSlice;
			var WSlice => MyChunk.WSlice;
			var Neighs => MyChunk.Neighs;

			forall xyz in ND {
				NSlice[xyz] = MyLocDom.exterior(xyz);
				WSlice[xyz] = NSlice[xyz];
			}

			// use so we can move atoms around more easily
			BinToLocale[NSlice[0,0,0]] = (i,j,k);

			startBarrier();
			stopBarrier();
			startBarrier();
			stopBarrier();

			// adjust our remote-translated slices and neighbor pointer
			// store for future use (no need to recompute every time!)
			for xyz in ND {
				if xyz == (0,0,0) then continue; // no comms with ourself

				// get the index of the neighboring Locale
				var neighbor = (i,j,k) + xyz / next;
				var offset = (0,0,0);
				// var trans = (0,0,0); // TODO: assign trans, only do 1 translate at the end
				if neighbor(1) < 0 {
					neighbor(1) = LocaleGridDom.high(1);
					offset(1) = binSpace.high(1)-NSlice[xyz].high(1);
				}
				else if neighbor(1) > LocaleGridDom.high(1) {
					neighbor(1) = 0;
					offset(1) = -binSpace.high(1)+binSpace.low(1)-1;
				}	
				if neighbor(2) < 0 {
					neighbor(2) = LocaleGridDom.high(2);
					offset(2) = binSpace.high(2)-NSlice[xyz].high(2);
				}
				else if neighbor(2) > LocaleGridDom.high(2) {
					neighbor(2) = 0;
					offset(2) = -binSpace.high(2)+binSpace.low(2)-1;
				}
				if neighbor(3) < 0 {
					neighbor(3) = LocaleGridDom.high(3);
					offset(3) = binSpace.high(3)-NSlice[xyz].high(3);
				}
				else if neighbor(3) > LocaleGridDom.high(3) {
					neighbor(3) = 0;
					offset(3) = -binSpace.high(3)+binSpace.low(3)-1;
				}
				WSlice[xyz] = WSlice[xyz].translate(offset);
				Neighs[xyz] = neighbor;

				if debug then writeln(WSlice[xyz], " is the remote neighbor of ", NSlice[xyz]);
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
	var fstyletmp = r.readln(string);
	if fstyletmp == "lj" then force = ForceStyle.FORCELJ;
	else force = ForceStyle.FORCEEAM;

	// 'size of problem', not sure what intent is yet
	//(psize.x,psize.y,psize.z) = r.readln(int,int,int);
	psize = r.readln(int,int,int);
	
	// # iterations
	nsteps = r.readln(int);

	// simulated time step size
	dt = r.readln(real);

	// starting temperature
	t_request = r.readln(real);

	// start density (overriden if data file), used in EAM
	rho = r.readln(real);

	// recompute neighbors every N iterations
	neigh_every = r.readln(int);

	// force cutoff: used to determine significance of force computation
	// neighbor skin: distance past force cutoff we care about
	(force_cut, cutneigh) = r.readln(real,real);

	// do thermo compute every N iteratiosn
	nstat = r.readln(int);

	// cutneigh = skin + force_cutoff, used in neighboring algorithm
	cutneigh += force_cut;

	// cleanup
	r.close();
	fchan.close();
}

proc printHelp() {
	writeln("Commandline Options:");
	writeln("\n  Chapel help:");
	writeln("\t--chapel:                  Show Chapel's command line options");
	writeln("\n  Execution configuration:");
	writeln("\t--numThreadsPerLocale <threads>: set number of threads per Locale (chapel-specific)");
	writeln("\t--half_neigh <int>:           use half neighborlists (default 1)");
	writeln("\t                                0: full neighborlist");
	writeln("\t                                1: half neighborlist");
	writeln("\t                               -1: original miniMD half neighborlist force (not OpenMP safe)");
	writeln("\t-gn / --ghost_newton <int>:   set usage of newtons third law for ghost atoms");
	writeln("\t                                (only applicable with half neighborlists)"); 
	writeln("\n  Simulation setup:");
	writeln("\t-i / --input_file <string>:   set input file to be used (default: in.lj.miniMD)");
	writeln("\t-n / --nsteps <int>:          set number of timesteps for simulation");
	writeln("\t-s / --size <int>:            set linear dimension of systembox");
	writeln("\t-b / --neigh_bins <int>:      set linear dimension of neighbor bin grid");
	writeln("\t-u / --units <string>:        set units (lj or metal), see LAMMPS documentation");
	writeln("\t-p / --force <string>:        set interaction model (lj or eam)");
	writeln("\t-f / --data_file <string>:    read configuration from LAMMPS data file"); 
	writeln("\n  Miscelaneous:");
	writeln("\t                                within rcut_neighbor (outer force cutoff)");
	writeln("\t-o / --yaml_output <int>:     level of yaml output (default 1)");
	writeln("\t--yaml_screen:                write yaml output also to screen");
	writeln("\t-h / --help:                  display this help message");
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
	write("\t# force: ");
	if force == ForceStyle.FORCELJ then writeln("LJ");
	else writeln("EAM");
	write("\t# units: ");
	if units == "eam" then writeln("METAL");
	else writeln("LJ");
	writeln("\t# atoms: ", natoms);
	write("\t# System size: ");
	writef("%.2dr %.2dr %.2dr (unit cells: %i %i %i)\n", 
			dim(1), dim(2), dim(3), psize(1), psize(2), psize(3));
	writef("\t%# density: %.6dr\n", rho);
	writef("\t%# Force cutoff: %.6dr\n", force_cut);
	writef("\t%# neigh cutoff: %.6dr\n", cutneigh);
	writeln("\t# half neighborlists: ", half_neigh);
	writeln("\t# Neighbor bins: ", nbin(1), " ", nbin(2), " ", nbin(3));
	writeln("\t# neighbor frequency ", neigh_every);
	writef("\t%# timestep size: %.6dr\n", dt );
	writeln("\t# thermo frequency: ", nstat);
	writeln("\t# ghost newton: ", ghost_newton);
}

// establish bin and neighbor sizes/amounts
proc setupNeighbors() {
	binsize = dim / nbin;
	bininv = 1.0 / binsize;

	var coord : real = dimlo(1) - cutneigh - small * dim(1);
	mbinlo(1) = (coord * bininv(1)) : int;
	if coord < 0.0 then mbinlo(1) -= 1;

	coord = dimhi(1) + cutneigh + small * dim(1);
	mbinhi(1) = (coord * bininv(1)) : int;
	
	coord = dimlo(2) - cutneigh - small * dim(2);
	mbinlo(2) = (coord * bininv(2)) : int;
	if coord < 0.0 then mbinlo(2) -= 1;

	coord = dimhi(2) + cutneigh + small * dim(2);
	mbinhi(2) = (coord * bininv(2)) : int;

	coord = dimlo(3) - cutneigh - small * dim(3);
	mbinlo(3) = (coord * bininv(3)) : int;
	if coord < 0.0 then mbinlo(3) -= 1;

	coord = dimhi(3) + cutneigh + small * dim(3);
	mbinhi(3) = (coord * bininv(3)) : int;

	mbinlo -= 1;
	mbinhi += 1;
	mbin = mbinhi - mbinlo + 1;

	// determines how many neighboring bins we need from other nodes
	next(1) = (cutneigh * bininv(1)) : int;
	if next(1) * binsize(1) < factor * cutneigh then next(1) += 1;

	next(2) = (cutneigh * bininv(2)) : int;
	if next(2) * binsize(2) < factor * cutneigh then next(2) += 1;

	next(3) = (cutneigh * bininv(3)) : int;
	if next(3) * binsize(3) < factor * cutneigh then next(3) += 1;

	var nmax = (2 * next(3) + 1) * (2 * next(2) + 1) * (2 * next(1) + 1);
	stencilSpace = {1..nmax};
	
	var kstart = -next(3);
	nstencil = 1;

	// start at the halfway point
	if half_neigh && ghost_newton {
		kstart = 0;
		stencil[nstencil] = (0,0,0);
		nstencil += 1;
	}

	for k in kstart..next(3) {
		for j in -next(2) .. next(2) {
			for i in -next(1) .. next(1) {
				if !ghost_newton || !half_neigh || 
					(k > 0 || j > 0 || (j == 0 && i > 0)) {
					if bindist(i,j,k) < cutneighsq {
						stencil[nstencil] = (i,j,k);
						nstencil += 1;  
					}
				}
			}
		}
	}
	mbins = mbin(1) * mbin(2) * mbin(3);
}

proc create_atoms() {
	var alat : real = (4.0 / rho) ** (1.0 / 3.0);
	var ilo = (dimlo(1) / (.5 * alat)) : int;
	var ihi = (dimhi(1) / (.5 * alat)) : int;
	var jlo = (dimlo(2) / (.5 * alat)) : int;
	var jhi = (dimhi(2) / (.5 * alat)) : int;
	var klo = (dimlo(3) / (.5 * alat)) : int;
	var khi = (dimhi(3) / (.5 * alat)) : int;

	ilo = max(ilo, 0);
	ihi = min(ihi, 2 * psize(1) - 1) : int;
	jlo = max(jlo, 0);
	jhi = min(jhi, 2 * psize(2) - 1) : int;
	klo = max(klo, 0);
	khi = min(khi, 2 * psize(3) - 1) : int;

	var temp, v : v3;
	var s, o : v3int;
	var i, j, k, m, n : int;
	var subboxdim = 8;
	var flag = false;

	while o(3) * subboxdim <= khi {
		(i,j,k) = (o * subboxdim + s);

		if flag then continue;

		if ((i + j + k) % 2 == 0) &&
			(i >= ilo) && (i <= ihi) &&
			(j >= jlo) && (j <= jhi) &&
			(k >= klo) && (k <= khi) {
			temp(1) = .5 * alat * i;
			temp(2) = .5 * alat * j;
			temp(3) = .5 * alat * k;

			if temp(1) >= dimlo(1) && temp(1) < dimhi(1) &&
				temp(2) >= dimlo(2) && temp(2) < dimhi(2) &&
				temp(3) >= dimlo(3) && temp(3) < dimhi(3) {

				n = (k * (2 * psize(2)) * (2*psize(1)) + j * (2 * psize(1)) + i + 1) : int;
				for m in 1..5 { pmrand(n); }
				v(1) = pmrand(n);
				for m in 1..5 { pmrand(n); }
				v(2) = pmrand(n);
				for m in 1..5 { pmrand(n); }
				v(3) = pmrand(n);
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

		if o(1) * subboxdim > ihi {
			o(1) = 0;
			o(2) += 1;
		}

		if o(2) * subboxdim > jhi {
			o(2) = 0;
			o(3) += 1;
		}
	}
}

proc create_velocity(treq : real) {
	var i : int;
	var vtot : v3;
	var cur = 1;

	// i think we could just do one reduce on the v3 record ,as it defines +()
	//vtot = + reduce forall r in realStencil do 
	//			+ reduce forall i in 1..binCount[r] do bins[r][i].v;
	var vtotDist : [LocaleGridDom] v3;
	coforall (ijk) in LocaleGridDom {
		on LocaleGrid[ijk] {
			vtotDist[ijk] = + reduce forall bin in Bins[ijk].Real do 
			+ reduce forall ind in 1..binCount[bin] do Bins[ijk].Arr[bin][ind].v;
		}
	}
	vtot = + reduce vtotDist;

	vtot /= (natoms,natoms,natoms);

	coforall (ijk) in LocaleGridDom {
		on LocaleGrid[ijk] {
			forall bin in Bins[ijk].Real {
				for ind in 1..binCount[bin] {
					Bins[ijk].Arr[bin][ind].v -= vtot;
				}
			}
		}
	}


	thermo.t_act = 0;
	const t = temperature();
	const factor = sqrt(treq / t);

	coforall (ijk) in LocaleGridDom {
		on LocaleGrid[ijk] {
			var fac = (factor,factor,factor);
			forall bin in Bins[ijk].Real {
				for ind in 1..binCount[bin] {
					Bins[ijk].Arr[bin][ind].v *= fac;
				}
			}
		}
	}
}

proc pmrand(ref n : int) : real {
	var k : int;
	var ans : real;

	// constants from C++ version
	// comment from c++ regarding this random function: 
	// "Park/Miller RNG w/out MASKING, so as to be like f90s version"

	k = n / IQ;
	n = IA * (n - k * IQ) - IR * k;
	if n < 0 then n += IM;
	ans = AM * n;
	return ans;
}

proc communicateCopies() {
	coforall ijk in LocaleGridDom {
		on LocaleGrid[ijk] {
			for neigh in Bins[ijk].NeighDom {
				// don't look at ourself
				if neigh == (0,0,0) then continue;

				// ease of typing
				var Data => Bins[ijk].Arr;
				var Neighs => Bins[ijk].Neighs;
				var NSlice => Bins[ijk].NSlice;
				var WSlice => Bins[ijk].WSlice;

				// copy over the binCount
				binCount[NSlice[neigh]] = binCount[WSlice[neigh]];
				
				// if a dimension of the bin exists in adjacent 'real' chunk of bins, 
				// we shouldn't offset
				var dimtest = (1.0,1.0,1.0);
				if NSlice[neigh].low(1) == WSlice[neigh].low(1) then dimtest(1) = 0.0;
				if NSlice[neigh].low(2) == WSlice[neigh].low(2) then dimtest(2) = 0.0;
				if NSlice[neigh].low(3) == WSlice[neigh].low(3) then dimtest(3) = 0.0;
				
				// ghost's position offset
				const offset = (neigh/next) * dim * dimtest;

				// for every atom in every bin, clone & wrap the position
				forall (N, W) in zip(NSlice[neigh], WSlice[neigh]) {
					var c = binCount[N];
					for (loc, rem) in zip(Data[N][1..c], Bins[Neighs[neigh]].Arr[W][1..c]) {
						if N == W then loc.x = rem.x;
						else { 
							loc.x = rem.x + (offset);
						}
					}
				}
			}
		}
	}
}

// compute distance between 'central' bin and bin ijk
proc bindist(i, j, k : int) {
	var del : v3;

	if i > 0 then del(1) = (i - 1) * binsize(1);
	else if i == 0 then del(1) = 0;
	else del(1) = (i+1) * binsize(1);

	if j > 0 then del(2) = (j - 1) * binsize(2);
	else if j == 0 then del(2) = 0;
	else del(2) = (j + 1) * binsize(2);

	if k > 0 then del(3) = (k - 1) * binsize(3);
	else if k == 0 then del(3) = 0;
	else del(3) = (k + 1) * binsize(3);

	return dot(del,del);
}

// if atoms moved outside the box, adjust their locations 
proc pbc() {
	coforall (i,j,k) in LocaleGridDom {
		on LocaleGrid[i,j,k] {
			var rs = Bins[i,j,k].Real;
			forall (b,c) in zip(Bins[i,j,k].Arr[rs],binCount[rs]) {
				for a in b[1..c] {
					if a.x(1) < 0 then a.x(1) += dim(1);
					else if a.x(1) >= dim(1) then a.x(1) -= dim(1);

					if a.x(2) < 0 then a.x(2) += dim(2);
					else if a.x(2) >= dim(2) then a.x(2) -= dim(2);

					if a.x(3) < 0 then a.x(3) += dim(3);
					else if a.x(3) >= dim(3) then a.x(3) -= dim(3);
				}
			}
		}
	}
}

proc buildNeighbors(pn = false) {
	if debug then writeln("starting to build...");
	var tim : Timer;
	tim.start();
	pbc();
	commTime += tim.elapsed();
	tim.stop();
	tim.clear();
	tim.start();

	binatoms(); 

	buildTime += tim.elapsed();
	tim.stop();
	tim.clear();
	tim.start();
	if debug then writeln("starting comms...");
  communicateCopies();
	if debug then writeln("comms done...");

	commTime += tim.elapsed();
	tim.stop();
	tim.clear();
	tim.start();

	coforall ijk in LocaleGridDom {
		on LocaleGrid[ijk] {
			var Real = Bins[ijk].Real;
			var Data => Bins[ijk].Arr;
			forall (b,r,c) in zip(Data[Real],Real, binCount[Real]) {
				for (a,i) in zip(b[1..c],1..c) {
					a.ncount = 0;

					for s in stencil[1..nstencil-1] { // for each surrounding bin
						const o = r + s;
						for (n,x) in zip(Data[o][1..binCount[o]], 
								1..binCount[o]) { // for each atom in that bin
							// skip atoms we already looked at
							var lookedAt = o < r || (o == r && x < i);
							if half_neigh && !ghost_newton && lookedAt then continue;
							if r == o {
								if x == i then continue; // same atom, ignore
								if half_neigh && ghost_newton && 
									(lookedAt || ((!Real.member(o)) && 
									((n.x(3) < a.x(3)) || (n.x(3) == a.x(3) && n.x(2) < a.x(2)) ||
									(n.x(3) == a.x(3) && n.x(2) == a.x(2) && n.x(1) < a.x(1))))) 
									then continue;
							}
							const del = a.x - n.x;
							const rsq = dot(del,del);
							if rsq <= cutneighsq {
								a.ncount += 1;
								// resize neighbor list of necessary
								var h = a.nspace.high;
								while a.ncount > h { 
									h = ((a.nspace.high*1.2) : int);
									a.nspace = {1..h};
								}
								a.neighs[a.ncount] = (o,x);
							}
						}
					}
				}
			}
		}
	}
	buildTime += tim.elapsed();
	if debug then writeln("building done...");
} // end of buildNeighbors

inline proc addatom(a : atom, b : (int,int,int)) {
	binCount[b] += 1;
	if binCount[b] >= perBinSpace.high {
		var h = (perBinSpace.high * 1.5) : int;
		perBinSpace = {1..h};
	}
	Bins[BinToLocale[b]].Arr[b][binCount[b]] = a;
}

// TODO: reduce communication here
proc binatoms() {
	var mspace : domain(1) = {1..50};
	var moved : [mspace] (atom,(int,int,int));
	var m = 0;
	for ijk in LocaleGridDom {
		on LocaleGrid[ijk] {
			var Real = Bins[ijk].Real;
			for (b,r,c) in zip(Bins[ijk].Arr[Real],
					Real,
					binCount[Real]) {
				var cur = 1;
				while(cur <= c) {
					const destBin = coord2bin(b[cur].x);
					if destBin != r { // atom moved
						m += 1;
						if m >= mspace.high then mspace = {1..mspace.high * 2};
						moved[m] = (b[cur], destBin);
						// replace with atom at end of list, if one exists
						if cur < c then b[cur] = b[c]; 
						c -= 1; // correct this bin's counter
					} else cur += 1;
				}
			}
		}
	}
									
	for a in moved[1..m] {
		addatom(a(1),a(2));
	}
}

proc coord2bin(x : v3){
	var cur : v3int;

	if x(1) >= dim(1) then 
		cur(1) = ((x(1) - dim(1)) * bininv(1) + nbin(1) - mbinlo(1)) : int;
	else cur(1) = ((x(1) * bininv(1)) - mbinlo(1)) : int;


	if x(2) >= dim(2) then 
		cur(2) = ((x(2) - dim(2)) * bininv(2) + nbin(2) - mbinlo(2)) : int;
	else cur(2) = ((x(2) * bininv(2)) - mbinlo(2)) : int;


	if x(3) >= dim(3) then 
		cur(3) = ((x(3) - dim(3)) * bininv(3) + nbin(3) - mbinlo(3)) : int;
	else cur(3) = ((x(3) * bininv(3)) - mbinlo(3)) : int;

	return (cur(1), cur(2), cur(3));
}
