
module MDThermo {
	use MDInteg;
	use MDSystem;
	use MDForce;
	use MDConfig;
	use Time;

	class Thermo {
		var nstat, mstat, ntimes : int; // stores information about iterations
		var t_act, p_act, e_act : real; // 
		var t_scale, p_scale, e_scale, mvv2e, dof_boltz : real;
		var rho : real;

		// holds data regarding temperature, energy, pressure
		// later used in more verbose output (unimplemented)
		var stepSpace : domain(1);
		var steps : [stepSpace] int;
		var temps : [stepSpace] real;
		var engs : [stepSpace] real;
		var prs : [stepSpace] real;

		proc Thermo(inter : Integrator, sys : System, con : Config) {
			this.rho = con.rho;
			this.nstat = con.thermo_nstat;
			this.ntimes = sys.con.ntimes;
			var dims = sys.dim;
			
			var maxstat : int;
			if nstat == 0 then maxstat = 2;
			else maxstat = ntimes / nstat + 2;
			stepSpace = {1..maxstat};

			// from the c++ version, looks like scientific constants
			if !con.units {
				mvv2e = 1.0;
				dof_boltz = sys.natoms * 3 - 3;
				t_scale = mvv2e / dof_boltz;
				p_scale = 1.0 / 3 / dims.x / dims.y / dims.z;
				e_scale = 0.5;
			} else {
				mvv2e = 1.036427e-4;
				dof_boltz = (sys.natoms * 3 - 3) * 8.617343e-5;
				t_scale = mvv2e / dof_boltz;
				p_scale = 1.602176e6 / 3 / dims.x / dims.y / dims.z;
				e_scale = 524287.985533;
				inter.dtforce /= mvv2e;
			} 
		}

		proc compute( flag : int, sys : System, force : Force, total : Timer) {
			var t, eng, p : real;
			if flag > 0 && (flag % nstat >= 1) then return;
			if flag == -1 && nstat > 0 && (ntimes % nstat == 0) then return;

			t_act = 0;
			e_act = 0;
			p_act = 0;

			// calculate
			t = temperature(sys);
			eng = energy(sys,force);
			p = pressure(t, force);

			var istep = flag;
			if flag == -1 then istep = ntimes;
			if flag == 0 then mstat = 1;

			// store data
			steps[mstat] = istep;
			temps[mstat] = t;
			engs[mstat] = eng;
			prs[mstat] = p;

			mstat += 1;

			var tval : real = 0.0;
			if istep != 0 then tval = total.elapsed();
				if printOriginal then writef("%i %er %er %er %.6dr\n", istep, t, eng, p, tval);
				else if printCorrect then  writef("%i %er %er %er\n", istep, t, eng, p);
		}

		proc energy(sys : System, force : Force) {
			e_act = force.eng_vdwl;

			if sys.con.half_neigh then e_act *= 2.0;

			e_act *= e_scale;

			return e_act / sys.natoms;
		}

		proc temperature(sys : System) {
			t_act = 0;
			t_act = + reduce forall r in sys.realStencil do + reduce forall i in 1..sys.binCount[r] do (sys.bins[r][i].v.dot(sys.bins[r][i].v) * sys.mass);

			return t_act * t_scale;
		}

		proc pressure(t : real, force : Force) {
			return (t * dof_boltz + force.virial) * p_scale;
		}
	} 
} 
