
module MDInteg {
	use MDSystem;
	use MDForce;
	use MDThermo;
	use MDConfig;
	use Time;
	class Integrator {
		var dt : real;
		var dtforce : real;
		var forceTime : real;

		proc Integrator(con : Config) {
			this.dt = con.dt;
			this.dtforce = 0.5 * dt;
		}

		// update positions and velocities based on forces
		proc initialIntegrate(sys : System) {
				forall r in sys.realStencil {
					for i in 1..sys.binCount[r] {
						sys.bins[r][i].v.x += dtforce * sys.bins[r][i].f.x;
						sys.bins[r][i].v.y += dtforce * sys.bins[r][i].f.y;
						sys.bins[r][i].v.z += dtforce * sys.bins[r][i].f.z;
						
						sys.bins[r][i].x.x += dt * sys.bins[r][i].v.x;
						sys.bins[r][i].x.y += dt * sys.bins[r][i].v.y;
						sys.bins[r][i].x.z += dt * sys.bins[r][i].v.z;
					}
				}

				// a non-obvious state of the ghost here is that after
				// the force computation, we have the ghost store the 
				// offset to the real atom it represents. This is done 
				// so we don't have to redo the dt * a.v computation
				// for all ghosts
				forall g in sys.ghostStencil {
					for i in 1..sys.binCount[g] {
						sys.bins[g][i].x += sys.bins[sys.bins[g][i].ghostof(1)][sys.bins[g][i].ghostof(2)].x; // oh dear, this is ugly
					}
				}
		}

		// update velocities
		proc finalIntegrate(sys : System) {
			forall r in sys.realStencil {
				for i in 1..sys.binCount[r] {
					sys.bins[r][i].v.x += dtforce * sys.bins[r][i].f.x;
					sys.bins[r][i].v.y += dtforce * sys.bins[r][i].f.y;
					sys.bins[r][i].v.z += dtforce * sys.bins[r][i].f.z;
				}
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
