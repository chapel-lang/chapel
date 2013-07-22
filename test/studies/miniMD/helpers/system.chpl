module MDSystem {
	use Time;
	use MDThermo; // used in case where we build our own atoms
	use MDForce;
	use MDConfig;

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
	// stores information about an atom
	record atom {
		var x, v, f : v3; // position, velocity, force
		var nspace : domain(1) = {1..100};
		var neighs : [nspace] int;
		var ncount : int = 0;
	}
	proc +(left : v3, right : v3) : v3 {
		var r : v3;
		r.x = left.x + right.x;
		r.y = left.y + right.y;
		r.z = left.z + right.z;
		return r;
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
	type v3int = v3(int);
	class System {
		// dimensions of simulation space
		var dim : v3;
		var xlo, xhi : real;
		var ylo, yhi : real;
		var zlo, zhi : real;

		// atom storage
		var atomSpace : domain(1);
		var atoms : [atomSpace] atom;
		var natoms : int;

		var con : Config;

		var ghost_newton : bool = true;
		var mass : real = 1.0; // default mass

		// neighbor information
		var nbin : v3int;
		var binsize : v3;
		var bininv : v3;
		var binSpace : domain(3);
		var perBinSpace : domain(1) = {1..8};
		var bins : [binSpace] [perBinSpace] int(32); 
		var binCount : [binSpace] int(32);

		// ghosts
		var ghostSpace : domain(1);
		var ghosts : [ghostSpace] (int,v3);

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
		var cutneigh : real;
		var cutneighsq : real;
		var buildTime : real; // total buildNeighbors time (minus pbc and findghosts, so we can mimic c++)
		var commTime : real; // sort of a misnomer, but measures pbc and findghosts time

		proc System(force : Force,ref con : Config) {
			if force.style == ForceStyle.FORCEEAM then mass = force.mass;
			ghost_newton = con.ghost_newton;
			this.con = con;

			cutneigh = con.neigh_cut;
			cutneighsq = cutneigh * cutneigh;

			// no data file, use input file to generate uniform lattice
			if con.dfile == "" || con.dfile == "none" {
				var lattice : real = (4.0 / con.rho) ** (1.0 / 3.0);
				dim = con.n * lattice;

				xlo = 0.0; ylo = 0.0; zlo = 0.0;
				xhi = dim.x; yhi = dim.y; zhi = dim.z;
				natoms = (4 * con.n.x * con.n.y * con.n.z) : int;
				// finish things up in main after we create a Thermo

				var ns = 5.0/6.0;
				nbin.x = (ns * con.n.x) : int;
				nbin.y = (ns * con.n.y) : int;
				nbin.z = (ns * con.n.z) : int;
			} else {
				var indata = open(con.dfile, iomode.r);
				var r = indata.reader();

				r.readln(); // skip first line

				natoms = r.readln(int);
				atomSpace = {1..natoms};

				// more complex LAMMPS files can have multiple atom types
				// as far as I can tell, c++ miniMD takes only 1 type
				var types = r.readln(int);
				assert(types == 1, "You can only have one type of atom.");

				// sim dimensions
				(xlo, xhi) = r.readln(real,real);
				(ylo, yhi) = r.readln(real,real);
				(zlo, zhi) = r.readln(real,real);
				dim.x = xhi-xlo;
				dim.y = yhi-ylo;
				dim.z = zhi-zlo;

				var masses = r.readln(string);
				assert(masses == "Masses");
				var (mass_type, mass) = r.readln(int,real);
				this.mass = mass;

				// get atoms' positions and velocities
				readAttribute("Atoms", r, false);

				readAttribute("Velocities", r, true);
				r.close();
				indata.close();

				// density overriden if data file provided
				con.rho = natoms / ( dim.x * dim.y * dim.z);
				var nbs : real = (con.rho * 16) ** (1.0/3.0 : real);
				nbin.x = (dim.x / nbs) : int;
				nbin.y = (dim.y / nbs) : int;
				nbin.z = (dim.z / nbs) : int;
				setupNeighbors();
			}
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

			var coord : real = xlo - cutneigh - small * dim.x;
			mbinlo.x = (coord * bininv.x) : int;
			if coord < 0.0 then mbinlo.x -= 1;

			coord = xhi + cutneigh + small * dim.x;
			mbinhi.x = (coord * bininv.x) : int;
			
		
			coord = ylo - cutneigh - small * dim.y;
			mbinlo.y = (coord * bininv.y) : int;
			if coord < 0.0 then mbinlo.y -= 1;

			coord = yhi + cutneigh + small * dim.y;
			mbinhi.y = (coord * bininv.y) : int;

			coord = zlo - cutneigh - small * dim.z;
			mbinlo.z = (coord * bininv.z) : int;
			if coord < 0.0 then mbinlo.z -= 1;

			coord = zhi + cutneigh + small * dim.z;
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

			if con.half_neigh && ghost_newton { // where to put ghost newton?
				kstart = 0;
				stencil[nstencil] = (0,0,0);
				nstencil += 1;
			}

			for k in kstart..next.z {
				for j in -next.y .. next.y {
					for i in -next.x .. next.x {
						if !ghost_newton || !con.half_neigh || (k > 0 || j > 0 || (j == 0 && i > 0)) {
							if bindist(i,j,k) < cutneighsq {
								stencil[nstencil] = (i,j,k);
								nstencil += 1;  
							}
						}
					}
				}
			}
			mbins = mbin.x * mbin.y * mbin.z;
			binSpace = {1..mbin.x, 1..mbin.y, 1..mbin.z};
		}

		proc finish(thermo : Thermo) {
			create_atoms();
			create_velocity(con.t_request, thermo);
			setupNeighbors();
		}

		proc create_atoms() {
			atomSpace = {1..natoms};
			var alat : real = (4.0 / con.rho) ** (1.0 / 3.0);
			var ilo = (xlo / (.5 * alat)) : int;
			var ihi = (xhi / (.5 * alat)) : int;
			var jlo = (ylo / (.5 * alat)) : int;
			var jhi = (yhi / (.5 * alat)) : int;
			var klo = (zlo / (.5 * alat)) : int;
			var khi = (zhi / (.5 * alat)) : int;

			ilo = max(ilo, 0);
			ihi = min(ihi, 2 * con.n.x - 1) : int;
			jlo = max(jlo, 0);
			jhi = min(jhi, 2 * con.n.y - 1) : int;
			klo = max(klo, 0);
			khi = min(khi, 2 * con.n.z - 1) : int;

			var temp, v : v3;
			var s, o : v3int;
			var i, j, k, m, n : int;
			var subboxdim = 8;
			var flag = false;
			var cura = 1;
			var ghostc = 0;

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

					if temp.x >= xlo && temp.x < xhi &&
						temp.y >= ylo && temp.y < yhi &&
						temp.z >= zlo && temp.z < zhi {

						n = (k * (2 * con.n.y) * (2*con.n.x) + j * (2 * con.n.x) + i + 1) : int;
						for m in 1..5 { pmrand(n); }
						v.x = pmrand(n);
						for m in 1..5 { pmrand(n); }
						v.y = pmrand(n);
						for m in 1..5 { pmrand(n); }
						v.z = pmrand(n);
						//atoms[cura] = new atom();
						atoms[cura].x = temp;
						atoms[cura].v = v;
						cura += 1;
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

		proc create_velocity(treq : real, thermo : Thermo) {
			var i : int;
			var vtot : v3;
			var cur = 1;

			vtot.x = + reduce forall a in atoms do a.v.x;
			vtot.y = + reduce forall a in atoms do a.v.y;
			vtot.z = + reduce forall a in atoms do a.v.z;

			vtot.x /= natoms;
			vtot.y /= natoms;
			vtot.z /= natoms;

			forall a in atoms {
				a.v.x -= vtot.x;
				a.v.y -= vtot.y;
				a.v.z -= vtot.z;
			}

			thermo.t_act = 0;
			var t = thermo.temperature(this);
			var factor = sqrt(treq / t);

			forall a in atoms {
				a.v.x *= factor;
				a.v.y *= factor;
				a.v.z *= factor;
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

		proc numGhosts(v : v3) {
			var c = cutneigh;
			var foundx = false;
			var foundy = false;
			var ghostc = 0;

			if v.x <= xlo+c || v.x >= xhi-c { ghostc += 1; foundx = true; }
			if v.y <= ylo+c || v.y >= yhi-c { 
				foundy = true; 
				ghostc += 1;
				if foundx then ghostc += 1;
			}
			if v.z <= zlo+c || v.z >= zhi-c {
				ghostc += 1;
				if foundx then ghostc += 1;
				if foundy then ghostc += 1;
				if foundx && foundy then ghostc += 1;
			}
			return ghostc;
		}

		proc findGhosts() {
			var ghostc = 0;

			// first, count # ghosts to establish our array size
			ghostc = + reduce forall a in atoms do numGhosts(a.x);
			ghostSpace = {1..ghostc};
			ghostc = 1;
			
			// An atom is a ghost if at least one position component (x,y,z) 
			// is within cutneigh distance to a 'wall' of the simulation
			// forall + fetchadd?
			for (a,i) in zip(atoms,atoms.domain) {
				var c = cutneigh;
				var t : v3;
				t.zero();
				var foundx = false;
				var foundy = false;
				var xres, yres : v3;
				if a.x.x <= xlo+c || a.x.x >= xhi-c {
					if a.x.x <= xlo + c then t.x += dim.x;
					else t.x -= dim.x;
					xres = t;
					ghosts[ghostc] = (i,t);
					ghostc += 1;
					foundx = true;
				}
				if a.x.y <= ylo+c || a.x.y >= yhi-c {
					var q : v3;
					q.zero();
					if a.x.y <= ylo + c then q.y += dim.x;
					else q.y -= dim.x;
					ghosts[ghostc] = (i,q);
					ghostc += 1;
					yres = q;
					if foundx {
						if a.x.y <= ylo + c then t.y += dim.x;
						else t.y -= dim.x;
						ghosts[ghostc] = (i,t);
						ghostc += 1;
					}
					foundy = true;
				}
				if a.x.z <= zlo+c || a.x.z >= zhi-c {
					var q : v3;
					if a.x.z <= zlo + c then q.z += dim.x;
					else q.z -= dim.x;
					ghosts[ghostc] = (i,q);
					ghostc += 1;

					if foundx && foundy {
						var xyz = q;
						xyz.x = xres.x;
						xyz.y = yres.y;
						ghosts[ghostc] = (i,xyz);
						ghostc += 1;
					}
					if foundx { 
						var xz = q;
						xz.x = xres.x;
						ghosts[ghostc] = (i,xz);
						ghostc += 1;
					} 
					if foundy {
						var yz = q;
						yz.y = yres.y;
						ghosts[ghostc] = (i,yz);
						ghostc += 1;
					}
				}
			}


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

		proc readAttribute(title : string, rdr, isvel) {
			var titlestr = rdr.readln(string);
			assert(titlestr == title);
			for (a,i) in zip(atoms,atoms.domain) {
				//if a == nil then a = new atom();
				rdr.read(int, int);
				if isvel then // bit messy, probably a better way than a flag
					rdr.readln(a.v.x,a.v.y,a.v.z);
				else
					rdr.readln(a.x.x,a.x.y,a.x.z);
			}
		}

		// if atoms moved outside the box, adjust their locations 
		proc pbc() {
			forall a in atoms {
				if a.x.x < 0 then a.x.x += dim.x;
				if a.x.x >= dim.x then a.x.x -= dim.x;

				if a.x.y < 0 then a.x.y += dim.y;
				if a.x.y >= dim.y then a.x.y -= dim.y;

				if a.x.z < 0 then a.x.z += dim.z;
				if a.x.z >= dim.z then a.x.z -= dim.z;
			}
		}

		proc buildNeighbors(pn = false) {
			var tim : Timer;
			tim.start();
			
			pbc();
			findGhosts();
			commTime += tim.elapsed();
			tim.stop();
			tim.clear();
			tim.start();

			binatoms(); // not the source of lag, maybe a little at the start
			forall (a,i) in zip(atoms,atoms.domain) {
				a.ncount = 0;
				var binc = coord2bin(a.x);

				for q in 1..nstencil-1 { // for each surrounding bin
					var s = stencil[q];
					var other_bin = binc + s;
					for x in 1..binCount[other_bin] { // for each atom in that bin
						var j = bins[other_bin][x];
						var n : v3;
						if j <= natoms then n = atoms[j].x;
						else {
							n = ghosts[j-natoms](2);
							n = n + atoms[ghosts[j-natoms](1)].x;
						}

						// skip atoms we already looked at
						if con.half_neigh && !ghost_newton && j < i then continue;
						if binc == other_bin {
							if j == i then continue;
							if con.half_neigh && ghost_newton && ((j < i) || ((j > natoms) && 
										((n.z < a.x.z) || (n.z == a.x.z && n.y < a.x.y) ||
										 (n.z == a.x.z && n.y == a.x.y && n.x < a.x.x)))) then continue;
						}

						var del = a.x - n;
						var rsq = del.dot(del);
						if rsq <= cutneighsq {
							a.ncount += 1;
							if a.ncount > a.nspace.high { // resize neighbor list of necessary
								a.nspace = {1..(a.nspace.high*1.2 : int)};
							}
							a.neighs[a.ncount] = j;
						}
					}
				}
			}
			buildTime += tim.elapsed();
		} // end of buildNeighbors

		proc binatoms() {
			var resizing = true;
			// fetchadd bincount?
			forall b in binCount { 
				b = 0;
			}
			for (a,i) in zip(atoms,atoms.domain) {
				var binc = coord2bin(a.x);
				// do we have room?
				var offset = binCount[binc];
				binCount[binc] += 1;
				if binCount[binc] >= perBinSpace.high { // resize this bin's atom list
					var h = (perBinSpace.high * 1.2) : int;
					perBinSpace = {1..h};
				}
				bins[binc][offset+1] = (i : int(32));
			}
			for (g,i) in zip(ghosts,ghosts.domain) {
				var t = g(2);
				t = t + atoms[g(1)].x;
				var binc = coord2bin(t);
				var offset = binCount[binc];
				binCount[binc] += 1;
				if binCount[binc] >= perBinSpace.high {
					var h = (perBinSpace.high * 1.2) : int;
					perBinSpace = {1..h};
				}
				bins[binc][offset+1] = (natoms+i) : int(32);
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
		
	} 
}
