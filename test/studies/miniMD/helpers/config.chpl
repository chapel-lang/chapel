
module MDConfig {
	use Help;
	use MDForce;
	use MDSystem;

	config param printOriginal = false;
	config param printPerf = true;
	config param printCorrect = false;
	// Stores user-configurable variables
	record Config {
		var half_neigh = true;
		var ghost_newton = true;
		var input_file = "in.lj.miniMD";
		var ntimes = -1;
		var neigh_bins = -1;
		var units = false;
		var force = ForceStyle.FORCELJ;
		var dfile = "";
		var yaml_output = 1;
		var yaml_screen = false;
		var n : v3;
		var t_request, rho, dt, force_cut, neigh_cut : real;
		var neigh_every, thermo_nstat : int;

		// Most of this might be avoided using Chapel's 'config' type, 
		// but this code was written with the intent of most closely
		// imitating the behavior of the original C++ version of miniMD.
		proc parse(args : [] string) {
			for i in args.domain {
				if args[i] == "-i" || args[i] == "--input_file" {
					input_file = args[i+1];
				}
			}
			inputFile(input_file);
			for i in args.domain {
				var arg = args[i];
				if arg == "-h" || arg == "--help" then printHelp();
				else if arg == "--chapel" { printUsage(); exit(0); }
				else if arg == "-n" || arg == "--nsteps" then ntimes = args[i+1] : int;
				else if arg == "-s" || arg == "--size" { 
					n.x = args[i+1] : int;
					n.y = n.x;
					n.z = n.x;
				}
				else if arg == "-b" || arg == "--neigh_bins" then neigh_bins = args[i+1] : int;
				else if arg == "--half_neigh" then half_neigh = args[i+1] : bool;
				else if arg == "-o" || arg == "--yaml_output" then yaml_output = args[i+1] : int;
				else if arg == "--yaml_screen" then yaml_screen = true;
				else if arg == "-f" || arg == "--data_file" then dfile = args[i+1];
				else if arg == "-u" || arg == "--units" then units = args[i+1] == "metal";
				else if arg == "-p" || arg == "--force" then force = 
					if args[i+1] == "eam" then ForceStyle.FORCEEAM
					else ForceStyle.FORCELJ;
				else if arg == "-gn" || arg == "--ghost_newton" then ghost_newton = args[i+1] : bool;
			}
		}
		
		// Reads an input file
		// In mimicking the c++ version, this should be called 
		// before parse.
		proc inputFile(infile : string ) {
				var err : syserr;
				var fchan = open(err, infile, iomode.r);

				if err then {
					input_file = "none";
					units = false;
					dfile = "none";
					force = ForceStyle.FORCELJ;
					(n.x,n.y,n.z) = (32,32,32);
					ntimes = 100;
					dt = 0.005;
					t_request = 1.44;
					rho = .8442;
					neigh_every = 20;
					(force_cut, neigh_cut) = (2.5,.3);
					thermo_nstat = 100;
					neigh_cut += force_cut;
					return;
				}

				var r = fchan.reader();

				// skip first line
				r.readln();

				// get unit type ('lj' vs 'metal')
				units = r.readln(string) == "metal";

				// use data file?
				dfile = r.readln(string);

				// get force type : 'lj' vs 'eam'
				var fstyletmp = r.readln(string);
				if fstyletmp == "lj" then force = ForceStyle.FORCELJ;
				else force = ForceStyle.FORCEEAM;

				// 'size of problem', not sure what intent is yet
				(n.x,n.y,n.z) = r.readln(int,int,int);
				
				// # iterations
				ntimes = r.readln(int);

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
				(force_cut, neigh_cut) = r.readln(real,real);

				// do thermo compute every N iteratiosn
				thermo_nstat = r.readln(int);

				// neigh_cut = skin + force_cutoff, used in neighboring algorithm
				neigh_cut += force_cut;

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
		proc print(sys : System) {
			if !printOriginal then return;
			writeln("# Done .... ");
			writeln("# miniMD-Reference 0.1 (Chapel) output ...");
			writeln("# Systemparameters: ");
			writeln("\t# input_file: ", input_file);
			if dfile != "" then 
				writeln("\t# datafile: ", dfile);
			else
				writeln("\t# datafile: none");
			write("\t# force: ");
			if force == ForceStyle.FORCELJ then writeln("LJ");
			else writeln("EAM");
			write("\t# units: ");
			if units then writeln("METAL");
			else writeln("LJ");
			writeln("\t# atoms: ", sys.natoms);
			write("\t# System size: ");
			writef("%.2dr %.2dr %.2dr (unit cells: %i %i %i)\n", sys.dim.x, sys.dim.y, sys.dim.z, n.x, n.y, n.z);
			writef("\t%# density: %.6dr\n", rho);
			writef("\t%# Force cutoff: %.6dr\n", force_cut);
			writef("\t%# neigh cutoff: %.6dr\n", neigh_cut);
			writeln("\t# half neighborlists: ", half_neigh);
			writeln("\t# number of bins: ", sys.nbin.x, " ", sys.nbin.y, " ", sys.nbin.z);
			writeln("\t# neighbor frequency ", neigh_every);
			writef("\t%# timestep size: %.6dr\n", dt );
			writeln("\t# thermo frequency: ", thermo_nstat);
			writeln("\t# ghost newton: ", ghost_newton);
		}
	}
}
