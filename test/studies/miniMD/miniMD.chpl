

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
		writef("   %i      %i     %i     %i  %.6dr %.6dr %.6dr %.6dr\n", numLocales, + reduce Locales.numCores, nsteps, natoms, totalTime, forceTime, buildTime, commTime);
	}
	return 0;
}


// update positions and velocities based on forces
proc initialIntegrate() {
	forall (b,c) in zip(bins[realStencil],binCount[realStencil]) {
		for a in b[1..c] {
			a.v.x += dtforce * a.f.x;
			a.v.y += dtforce * a.f.y;
			a.v.z += dtforce * a.f.z;
			
			a.x.x += dt * a.v.x;
			a.x.y += dt * a.v.y;
			a.x.z += dt * a.v.z;
		}
	}

	// a non-obvious state of the ghost here is that after
	// the force computation, we have the ghost store the 
	// offset to the real atom it represents. This is done 
	// so we don't have to redo the dt * a.v computation
	// for all ghosts
	forall g in ghostStencil {
		for a in bins[g][1..binCount[g]] {
			a.x += bins[a.ghostof(1)][a.ghostof(2)].x; 
		}
	}
}

// update velocities
proc finalIntegrate() {
	forall (b,c) in zip(bins[realStencil],binCount[realStencil]) {
		for a in b[1..c] {
			a.v.x += dtforce * a.f.x;
			a.v.y += dtforce * a.f.y;
			a.v.z += dtforce * a.f.z;
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
		}
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
