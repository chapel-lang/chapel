
module MDInteg {
	use MDSystem;
	use MDForce;
	use MDThermo;
	use MDConfig;
	use Time;
	class Integrator {
		var dt : real;
		var dtforce : real;
		var ntimes : int;
		var forceTime : real;

		proc Integrator(con : Config) {
			this.dt = con.dt;
			this.ntimes = con.ntimes;
			this.dtforce = 0.5 * dt;
		}

		// update positions and velocities based on forces
		proc initialIntegrate(sys : System) {
				forall a in sys.atoms {
					a.v.x += dtforce * a.f.x;
					a.v.y += dtforce * a.f.y;
					a.v.z += dtforce * a.f.z;
					
					a.x.x += dt * a.v.x;
					a.x.y += dt * a.v.y;
					a.x.z += dt * a.v.z;
				}
		}

		// update velocities
		proc finalIntegrate(sys : System) {
			forall a in sys.atoms {
				a.v.x += dtforce * a.f.x;
				a.v.y += dtforce * a.f.y;
				a.v.z += dtforce * a.f.z;
			}
		}

		// iterates # times
		proc run(sys : System, force : Force, thermo : Thermo, total : Timer) {
			dtforce = dtforce / sys.mass;
			var tim : Timer;
			for i in 1..sys.con.ntimes {
				initialIntegrate(sys);

				if (i % sys.con.neigh_every) == 0 {
					sys.buildNeighbors();
				}
				tim.start();

				force.compute(sys, i % thermo.nstat == 0);

				forceTime += tim.elapsed();
				tim.stop();
				tim.clear();

				finalIntegrate(sys);

				if thermo.nstat > 0 then thermo.compute(i, sys, force, total);
			}
		} 
	}

}
