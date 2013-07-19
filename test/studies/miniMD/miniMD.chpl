

use MDSystem;
use MDInteg;
use MDForce;
use MDThermo;
use MDConfig;

proc main(args : [] string) {

	// read args and input file.
	// args override input file
	var con : Config;
	con.parse(args);

	// initialze force based on input config
	var force : Force;
	if con.force == ForceStyle.FORCELJ {
		force = new ForceLJ(con.force_cut);
	} else {
		force = new ForceEAM(con.force_cut);
		if printOriginal then writeln("# EAM currently requires '--ghost_newton 0'; Changing setting now.");
		con.ghost_newton = false;
	}
	if printOriginal then writeln("# Create System:");

	// Initialize atom positions, velocities, and neighboring
	var mdsys = new System(force, con);

	var inter = new Integrator(con);
	var thermo = new Thermo(inter, mdsys, con);
	if con.dfile == "" || con.dfile == "none" then mdsys.finish(thermo);
	
	// set up neighbors
	mdsys.buildNeighbors(); // pass true to print neighbors. Be warned, may be extremely verbose

	// display sim settings
	con.print(mdsys);
		
	// initial force computation pass
	force.compute(mdsys, true);

	// initial thermo pass
	if printOriginal then writeln("# Starting dynamics ...\n# Timestep T U P Time");
	var tim : Timer;
	thermo.compute(0, mdsys, force, tim);

	// run force computations ntimes with occasional thermo output
	var totalTime : real;
	tim.start();

	inter.run(mdsys, force, thermo, tim);

	totalTime = tim.elapsed();

	// get final thermo data
	force.compute(mdsys, true);
	thermo.compute(-1, mdsys, force, tim);

	if printPerf then writef("Time: %.6dr\n", totalTime);
	else if printOriginal {
		writeln("# Performance Summary:");
		writeln("# Locales Tasks nsteps natoms t_total, t_force t_neigh t_comm");
		writef("   %i      %i     %i     %i  %.6dr %.6dr %.6dr %.6dr\n", numLocales, + reduce Locales.numCores, con.ntimes, mdsys.natoms, totalTime, inter.forceTime, mdsys.buildTime, mdsys.commTime);
	}
	return 0;
} 
