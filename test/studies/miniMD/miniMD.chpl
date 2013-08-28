use initMD;
use MDForce;
use thermo;

var forceTime : real;

proc main() {
	// set up neighbors
	buildNeighbors(); // pass true to print neighbors. Be warned, may be extremely verbose

	printSim();

	// initial force computation pass
	fobj.compute(true);

	// initial thermo pass
	if printOriginal then writeln("# Starting dynamics ...\n# Timestep T U P Time");
	var tim : Timer;
	compute(0, fobj, tim);

	// run force computations ntimes with occasional thermo output
	var totalTime : real;
	tim.start();

	run(fobj, tim);

	totalTime = tim.elapsed();

	// get final thermo data
	fobj.compute(true);
	thermo.compute(-1, fobj, tim);

	if printPerf then writef("Time: %.6dr\n", totalTime);
	else if printOriginal {
		writeln("# Performance Summary:");
		writeln("# Locales Tasks nsteps natoms t_total, t_force t_neigh t_comm");
		writef("   %i      %i     %i     %i  %.6dr %.6dr %.6dr %.6dr\n", numLocales,
				+ reduce Locales.numCores, nsteps, natoms, 
				totalTime, forceTime, buildTime, commTime);
	}
	return 0;
}


// update positions and velocities based on forces
proc initialIntegrate() {
	coforall ijk in LocaleGridDom {
		on LocaleGrid[ijk] {
			var Real = Bins[ijk].Real;
			forall (b,c,r) in zip(Bins[ijk].Arr[Real], binCount[Real], Real) {
				for (a,x) in zip(b[1..c],1..c) {
					a.v += dtforce * a.f;
					a.x += dt * a.v;
				}
			}
		}
	}
}

// update velocities
proc finalIntegrate() {
	coforall ijk in LocaleGridDom {
		on LocaleGrid[ijk] {
			var Real = Bins[ijk].Real;
			forall (b,c) in zip(Bins[ijk].Arr[Real], binCount[Real]) {
				for a in b[1..c] {
					a.v += dtforce * a.f;
				}
			}
		}
	}
}

// iterates # times
proc run(f : Force, total : Timer) {
	dtforce = dtforce / mass;
	var tim : Timer;
	for i in 1..nsteps {
		initialIntegrate();

		if (i % neigh_every) == 0 {
			buildNeighbors();
		} else communicateCopies();
		tim.start();

		f.compute(i % nstat == 0);

		forceTime += tim.elapsed();
		tim.stop();
		tim.clear();

		finalIntegrate();

		if nstat > 0 then compute(i, f, total);
	}
	//writeln("wipe: ", f.wipetime, ", main: ", f.maintime, ", ghost: ", f.gtime);
} 
