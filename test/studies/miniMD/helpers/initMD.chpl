use Time;
use thermo; // used in case where we build our own atoms
use MDForce;

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

// system variables from input files
var psize = new v3(real, size:real,size:real,size:real);
var t_request = 1.44;
var rho = .8442;
var dt = .005;
var dtforce = .0025;
var force_cut = 2.5; 
var cutneigh = 2.8;
var neigh_every = 20;
var nstat = 100;

// 3d vector
record v3 { 
	type t = real;
	var x,y,z: t;
	proc zero() {
		x = 0;
		y = 0;
		z = 0;
	}
	proc dot(v : v3) : t {
		return x*v.x + y*v.y + z*v.z;
	}
}

type v3int = v3(int);

type binidx = ((int,int,int),int);

record atom {
	var x, v, f : v3; // position, velocity, force
	var nspace : domain(1) = {1..100};
	var neighs : [nspace] binidx;
	var ncount : int = 0;
	var ghostof : binidx = ((-1,-1,-1),-1);
}

proc +(left : v3, right : v3) : v3 {
	left.x += right.x;
	left.y += right.y;
	left.z += right.z;
	return left;
}

proc -(left : v3, right : v3) : v3 {
	var ret : v3;
	ret.x = left.x - right.x;
	ret.y = left.y - right.y;
	ret.z = left.z - right.z;
	return ret;
}

proc *(v : v3, s : real) : v3 {
	var r : v3;
	r.x = v.x * s;
	r.y = v.y * s;
	r.z = v.z * s;
	return r;
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
	natoms = (4 * psize.x * psize.y * psize.z) : int;

	const ns = 5.0/6.0;
	nbin.x = (ns * psize.x) : int;
	nbin.y = (ns * psize.y) : int;
	nbin.z = (ns * psize.z) : int;
} else {
	indata = open(data_file, iomode.r);
	dreader = indata.reader();

	dreader.readln(); // skip first line

	// more complex LAMMPS files can have multiple atom types
	// as far as I can tell, c++ miniMD takes only 1 type
	var types = dreader.readln(int);
	assert(types == 1, "You can only have one type of atom.");

	// sim dimensions
	(dimlo.x, dimhi.x) = dreader.readln(real,real);
	(dimlo.y, dimhi.y) = dreader.readln(real,real);
	(dimlo.z, dimhi.z) = dreader.readln(real,real);
	dim = dimhi - dimlo;

	var masses = dreader.readln(string);
	assert(masses == "Masses");
	var (mass_type, mass) = dreader.readln(int,real);
	
	// density overriden if data file provided
	rho = natoms / ( dim.x * dim.y * dim.z);
	var nbs : real = (rho * 16) ** (1.0/3.0 : real);
	nbin.x = (dim.x / nbs) : int;
	nbin.y = (dim.y / nbs) : int;
	nbin.z = (dim.z / nbs) : int;
//	setupNeighbors();

}
setupNeighbors();
dtforce = .5 * dt;
initThermo();

const binSpace = {1..mbin.x, 1..mbin.y, 1..mbin.z};
var perBinSpace : domain(1) = {1..8};
var bins : [binSpace] [perBinSpace] atom; 
var binCount : [binSpace] int(32);

// build stencils for real and ghost bins, so we don't iterate over useless bins
const realStencil : subdomain(binSpace) = {mbinhi.x - nbin.x + 1 .. mbinhi.x, mbinhi.y - nbin.y + 1 .. mbinhi.y, mbinhi.z - nbin.z + 1 .. mbinhi.z};
// ghosts
var ghostStencil : sparse subdomain(binSpace);
for b in binSpace do if !realStencil.member(b) then ghostStencil += b;

// actually generate/store atoms
if generating {
	create_atoms();
	create_velocity(t_request);
} else {
	dreader.readln(); // skip 'Atoms' line
	var tempPos : [1..natoms] v3;
	for x in tempPos {
		dreader.read(int,int);
		dreader.readln(x.x,x.y,x.z);
	}
	for x in tempPos {
		var v : v3;
		dreader.read(int,int);
		dreader.readln(v.x,v.y,v.z);
		addatom(new atom(x,v), coord2bin(x));
	}
	dreader.close();
	indata.close();
}

/* --------- init/helper functions ----------------*/

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
	(psize.x,psize.y,psize.z) = r.readln(int,int,int);
	
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
	writef("%.2dr %.2dr %.2dr (unit cells: %i %i %i)\n", dim.x, dim.y, dim.z, psize.x, psize.y, psize.z);
	writef("\t%# density: %.6dr\n", rho);
	writef("\t%# Force cutoff: %.6dr\n", force_cut);
	writef("\t%# neigh cutoff: %.6dr\n", cutneigh);
	writeln("\t# half neighborlists: ", half_neigh);
	writeln("\t# number of bins: ", nbin.x, " ", nbin.y, " ", nbin.z);
	writeln("\t# neighbor frequency ", neigh_every);
	writef("\t%# timestep size: %.6dr\n", dt );
	writeln("\t# thermo frequency: ", nstat);
	writeln("\t# ghost newton: ", ghost_newton);
}

proc setupNeighbors() {
	// lines of code until end of constructor deal with
	// non-trivial neighboring, a WIP
	binsize.x = (dim.x / nbin.x) : real;
	binsize.y = (dim.y / nbin.y) : real;
	binsize.z = (dim.z / nbin.z) : real;
	bininv.x = 1.0 / binsize.x;
	bininv.y = 1.0 / binsize.y;
	bininv.z = 1.0 / binsize.z;

	var coord : real = dimlo.x - cutneigh - small * dim.x;
	mbinlo.x = (coord * bininv.x) : int;
	if coord < 0.0 then mbinlo.x -= 1;

	coord = dimhi.x + cutneigh + small * dim.x;
	mbinhi.x = (coord * bininv.x) : int;
	
	coord = dimlo.y - cutneigh - small * dim.y;
	mbinlo.y = (coord * bininv.y) : int;
	if coord < 0.0 then mbinlo.y -= 1;

	coord = dimhi.y + cutneigh + small * dim.y;
	mbinhi.y = (coord * bininv.y) : int;

	coord = dimlo.z - cutneigh - small * dim.z;
	mbinlo.z = (coord * bininv.z) : int;
	if coord < 0.0 then mbinlo.z -= 1;

	coord = dimhi.z + cutneigh + small * dim.z;
	mbinhi.z = (coord * bininv.z) : int;

	mbinlo.x -= 1;
	mbinhi.x += 1;
	mbin.x = mbinhi.x - mbinlo.x + 1;

	
	mbinlo.y -= 1;
	mbinhi.y += 1;
	mbin.y = mbinhi.y - mbinlo.y + 1;
	
	mbinlo.z -= 1;
	mbinhi.z += 1;
	mbin.z = mbinhi.z - mbinlo.z + 1;

	var next : v3int;

	next.x = (cutneigh * bininv.x) : int;
	if next.x * binsize.x < factor * cutneigh then next.x += 1;

	next.y = (cutneigh * bininv.y) : int;
	if next.y * binsize.y < factor * cutneigh then next.y += 1;

	next.z = (cutneigh * bininv.z) : int;
	if next.z * binsize.z < factor * cutneigh then next.z += 1;

	var nmax = (2 * next.z + 1) * (2 * next.y + 1) * (2 * next.x + 1);
	stencilSpace = {1..nmax};
	
	var kstart = -next.z;
	nstencil = 1;

	if half_neigh && ghost_newton { // where to put ghost newton?
		kstart = 0;
		stencil[nstencil] = (0,0,0);
		nstencil += 1;
	}

	for k in kstart..next.z {
		for j in -next.y .. next.y {
			for i in -next.x .. next.x {
				if !ghost_newton || !half_neigh || (k > 0 || j > 0 || (j == 0 && i > 0)) {
					if bindist(i,j,k) < cutneighsq {
						stencil[nstencil] = (i,j,k);
						nstencil += 1;  
					}
				}
			}
		}
	}
	mbins = mbin.x * mbin.y * mbin.z;
}

proc create_atoms() {
	var alat : real = (4.0 / rho) ** (1.0 / 3.0);
	var ilo = (dimlo.x / (.5 * alat)) : int;
	var ihi = (dimhi.x / (.5 * alat)) : int;
	var jlo = (dimlo.y / (.5 * alat)) : int;
	var jhi = (dimhi.y / (.5 * alat)) : int;
	var klo = (dimlo.z / (.5 * alat)) : int;
	var khi = (dimhi.z / (.5 * alat)) : int;

	ilo = max(ilo, 0);
	ihi = min(ihi, 2 * psize.x - 1) : int;
	jlo = max(jlo, 0);
	jhi = min(jhi, 2 * psize.y - 1) : int;
	klo = max(klo, 0);
	khi = min(khi, 2 * psize.z - 1) : int;

	var temp, v : v3;
	var s, o : v3int;
	var i, j, k, m, n : int;
	var subboxdim = 8;
	var flag = false;

	while o.z * subboxdim <= khi {
		k = o.z * subboxdim + s.z;
		j = o.y * subboxdim + s.y;
		i = o.x * subboxdim + s.x;

		if flag then continue;

		if ((i + j + k) % 2 == 0) &&
			(i >= ilo) && (i <= ihi) &&
			(j >= jlo) && (j <= jhi) &&
			(k >= klo) && (k <= khi) {
			temp.x = .5 * alat * i;
			temp.y = .5 * alat * j;
			temp.z = .5 * alat * k;

			if temp.x >= dimlo.x && temp.x < dimhi.x &&
				temp.y >= dimlo.y && temp.y < dimhi.y &&
				temp.z >= dimlo.z && temp.z < dimhi.z {

				n = (k * (2 * psize.y) * (2*psize.x) + j * (2 * psize.x) + i + 1) : int;
				for m in 1..5 { pmrand(n); }
				v.x = pmrand(n);
				for m in 1..5 { pmrand(n); }
				v.y = pmrand(n);
				for m in 1..5 { pmrand(n); }
				v.z = pmrand(n);
				addatom(new atom(temp,v), coord2bin(temp));
			}
		}

		s.x += 1;

		if s.x == subboxdim {
			s.x = 0;
			s.y += 1;
		}

		if s.y == subboxdim {
			s.y = 0;
			s.z += 1;
		}

		if s.z == subboxdim {
			s.z = 0;
			o.x += 1;
		}

		if o.x * subboxdim > ihi {
			o.x = 0;
			o.y += 1;
		}

		if o.y * subboxdim > jhi {
			o.y = 0;
			o.z += 1;
		}
	}
}

proc create_velocity(treq : real) {
	var i : int;
	var vtot : v3;
	var cur = 1;

	// i think we could just do one reduce on the v3 record ,as it defines +()
	vtot = + reduce forall r in realStencil do 
				+ reduce forall i in 1..binCount[r] do bins[r][i].v;

	vtot.x /= natoms;
	vtot.y /= natoms;
	vtot.z /= natoms;

	forall r in realStencil  {
		for i in 1..binCount[r] {
			bins[r][i].v -= vtot;
		}
	}

	thermo.t_act = 0;
	const t = temperature();
	const factor = sqrt(treq / t);

	forall r in realStencil {
		for i in 1..binCount[r] {
			bins[r][i].v.x *= factor;
			bins[r][i].v.y *= factor;
			bins[r][i].v.z *= factor;
		}
	}
}

proc pmrand(ref n : int) : real {
	var k : int;
	var ans : real;

	// constants from C++ version
	// comment from c++ regarding this random function: 
	// "Park/Miller RNG w/out MASKING, so as to be like f90s version"
	const IA = 16807;
	const IM = 2147483647;
	const AM = 1.0/IM;
	const IQ = 127773;
	const IR = 2836;
	const MASK = 123459876;

	k = n / IQ;
	n = IA * (n - k * IQ) - IR * k;
	if n < 0 then n += IM;
	ans = AM * n;
	return ans;
}

proc findGhosts() {
	forall g in ghostStencil do binCount[g] = 0;
	forall (b,r,c) in zip(bins[realStencil],realStencil,binCount[realStencil]) {
		for i in 1..c {
			var c = cutneigh;
			var t = b[i].x;
			var foundx = false;
			var foundy = false;
			var xres, yres : v3;
			if b[i].x.x <= dimlo.x+c || b[i].x.x >= dimhi.x-c {
				if b[i].x.x <= dimlo.x + c then t.x += dim.x;
				else t.x -= dim.x;
				xres = t;
				addGhost(t,(r,i));
				foundx = true;
			}
			if b[i].x.y <= dimlo.y+c || b[i].x.y >= dimhi.y-c {
				var q = b[i].x;
				if b[i].x.y <= dimlo.y + c then q.y += dim.x;
				else q.y -= dim.x;
				yres = q;
				addGhost(q,(r,i));
				if foundx {
					if bins[r][i].x.y <= dimlo.y + c then t.y += dim.x;
					else t.y -= dim.x;
					addGhost(t, (r,i));
				}
				foundy = true;
			}
			if b[i].x.z <= dimlo.z+c || b[i].x.z >= dimhi.z-c {
				var q = b[i].x;
				if b[i].x.z <= dimlo.z + c then q.z += dim.x;
				else q.z -= dim.x;
				addGhost(q, (r,i));

				if foundx && foundy {
					var xyz = q;
					xyz.x = xres.x; // get the ghost we just added
					xyz.y = yres.y;
					addGhost(xyz, (r,i));
				}
				if foundx { 
					var xz = q;
					xz.x = xres.x;
					addGhost(xz, (r,i));
				} 
				if foundy {
					var yz = q;
					yz.y = yres.y;
					addGhost(yz, (r,i));
				}
			}
		}
	}
}

inline proc addGhost(x : v3, b : ((int,int,int),int(32))) {
	const temp = coord2bin(x);
	binCount[temp] += 1;
	bins[temp][binCount[temp]].x = x;
	bins[temp][binCount[temp]].ghostof = b;
}

// compute distance between 'central' bin and bin ijk
proc bindist(i, j, k : int) {
	var del : v3;

	if i > 0 then del.x = (i - 1) * binsize.x;
	else if i == 0 then del.x = 0;
	else del.x = (i+1) * binsize.x;

	if j > 0 then del.y = (j - 1) * binsize.y;
	else if j == 0 then del.y = 0;
	else del.y = (j + 1) * binsize.y;

	if k > 0 then del.z = (k - 1) * binsize.z;
	else if k == 0 then del.z = 0;
	else del.z = (k + 1) * binsize.z;

	return del.dot(del);
}

// if atoms moved outside the box, adjust their locations 
proc pbc() {
	forall (b,c) in zip(bins[realStencil],binCount[realStencil]) {
		for a in b[1..c] {
			if a.x.x < 0 then a.x.x += dim.x;
			else if a.x.x >= dim.x then a.x.x -= dim.x;

			if a.x.y < 0 then a.x.y += dim.y;
			else if a.x.y >= dim.y then a.x.y -= dim.y;

			if a.x.z < 0 then a.x.z += dim.z;
			else if a.x.z >= dim.z then a.x.z -= dim.z;
		}
	}
}

proc buildNeighbors(pn = false) {
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
	tim.start();

	findGhosts();

	commTime += tim.elapsed();
	tim.stop();
	tim.clear();
	tim.start();
	forall (b,r,c) in zip(bins[realStencil],realStencil, binCount[realStencil]) {
		for (a,i) in zip(b[1..c],1..c) {
			a.ncount = 0;

			for s in stencil[1..nstencil-1] { // for each surrounding bin
				const o = r + s;
				for (n,x) in zip(bins[o][1..binCount[o]], 1..binCount[o]) { // for each atom in that bin
					// skip atoms we already looked at
					var lookedAt = o < r || (o == r && x < i);
					if half_neigh && !ghost_newton && lookedAt then continue;
					if r == o {
						if x == i then continue; // same atom, ignore
						if half_neigh && ghost_newton && (lookedAt || ((bins[o][x].ghostof(2) != -1) && 
									((n.x.z < a.x.z) || (n.x.z == a.x.z && n.x.y < a.x.y) ||
									 (n.x.z == a.x.z && n.x.y == a.x.y && n.x.x < a.x.x)))) then continue;
					}
					const del = a.x - n.x;
					const rsq = del.dot(del);
					if rsq <= cutneighsq {
						a.ncount += 1;
						if a.ncount > a.nspace.high { // resize neighbor list of necessary
							a.nspace = {1..(a.nspace.high*1.2 : int)};
						}
						a.neighs[a.ncount] = (o,x);
					}
				}
			}
		}
	}
	buildTime += tim.elapsed();
} // end of buildNeighbors

inline proc addatom(a : atom, b : (int,int,int)) {
	binCount[b] += 1;
	if binCount[b] >= perBinSpace.high {
		var h = (perBinSpace.high * 1.5) : int;
		perBinSpace = {1..h};
	}
	bins[b][binCount[b]] = a;
}

proc binatoms() {
	var mspace : domain(1) = {1..50};
	var moved : [mspace] (atom,(int,int,int));
	var m = 0;
	for (b,r,c) in zip(bins[realStencil],realStencil,binCount[realStencil]) {
		var cur = 1;
		while(cur <= c) {
			const destBin = coord2bin(b[cur].x);
			if destBin != r { // atom moved
				m += 1;
				if m >= mspace.high then mspace = {1..mspace.high * 2};
				moved[m] = (b[cur], destBin);
				if cur < c then b[cur] = b[c]; // replace with atom at end of list, if one exists
				c -= 1; // correct this bin's counter
			} else cur += 1;
		}
	}
	for a in moved[1..m] {
		addatom(a(1),a(2));
	}
}

proc coord2bin(x : v3){
	var cur : v3int;

	if x.x >= dim.x then cur.x = ((x.x - dim.x) * bininv.x + nbin.x - mbinlo.x) : int;
	else cur.x = ((x.x * bininv.x) - mbinlo.x) : int;


	if x.y >= dim.y then cur.y = ((x.y - dim.y) * bininv.y + nbin.y - mbinlo.y) : int;
	else cur.y = ((x.y * bininv.y) - mbinlo.y) : int;


	if x.z >= dim.z then cur.z = ((x.z - dim.z) * bininv.z + nbin.z - mbinlo.z) : int;
	else cur.z = ((x.z * bininv.z) - mbinlo.z) : int;

	return (cur.x, cur.y, cur.z);
}
