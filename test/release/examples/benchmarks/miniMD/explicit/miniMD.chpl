use initMD;
use forces;
use thermo;
use neighbor;

/*
The actual entry point of miniMD lies in initMD.chpl. This is done so that 
some of our global variables can be made const. 
*/

proc main() {
  // build neighbor lists for each atom
  buildNeighbors();

  printSim();

  // initial force computation pass
  fobj.compute(true);

  // initial thermo pass
  if printOriginal then writeln("# Starting dynamics ...\n# Timestep T U P Time");

  // master timer
  var master : Timer;

  // compute initial temp, energy, pressure values
  computeThermo(0, master);

  // start master timer
  var totalTime : real;
  master.start();

  // start the actual simulation
  run(master);

  totalTime = master.elapsed();

  // get final thermo data
  fobj.compute(true);
  computeThermo(-1, master);

  if printPerf then writef("Time: %.6dr\n", totalTime);
  else if printOriginal {
    writeln("# Performance Summary:");
    writeln("# Locales Tasks numSteps numAtoms t_total, t_force t_neigh t_comm");
    writef("   %i      %i     %i     %i  %.6dr %.6dr %.6dr %.6dr\n", numLocales,
        + reduce Locales.maxTaskPar, numSteps, numAtoms, 
        totalTime, forceTime, buildTime, commTime);
  }
  cleanup();
}

// update positions and velocities based on forces
proc initialIntegrate() {
  // On each Locale in parallel, and for each bin in parallel, 
  // update the velocity and position of the bin's atoms
  coforall ijk in LocaleGridDom {
    on LocaleGrid[ijk] {
      const Me = Grid[ijk];
      forall (b,p,c) in zip(Me.Bins, Me.Pos[Me.Real], Me.Count[Me.Real]) {
        for (a, x) in zip(b[1..c],p[1..c]) {
          a.v += dtforce * a.f;
          x += dt * a.v;
        }
      }
    }
  }
}

// update velocities
proc finalIntegrate() {
  // On each Locale in parallel, and for each bin in parallel, 
  // update the velocity of the bin's atoms. We do this again so that 
  // the thermo computation has up-to-date values to work with.
  coforall ijk in LocaleGridDom {
    on LocaleGrid[ijk] {
      const Me = Grid[ijk];
      forall (b,c) in zip(Me.Bins, Me.Count[Me.Real]) {
        for a in b[1..c] {
          a.v += dtforce * a.f;
        }
      }
    }
  }
}

proc run(master : Timer) {
  dtforce = dtforce / mass;
  var iterTimer : Timer;
  for step in 1..numSteps {
    // update atoms' physical properties
    initialIntegrate();

    // every so often update our atoms' bins and neighbors 
    // in case something moved outside a bin. Otherwise, we update fluff 
    // positions
    if (step % neigh_every) == 0 {
      buildNeighbors();
    } else {
      iterTimer.start();

      // communicate positions
      updateFluff();

      commTime += iterTimer.elapsed();
      iterTimer.stop();
      iterTimer.clear();
    }

    iterTimer.start();

    // compute forces between atoms. a flag is passed that determines 
    // whether or not to update variables needed in thermo computation
    fobj.compute(step % thermoEvery == 0);

    forceTime += iterTimer.elapsed();
    iterTimer.stop();
    iterTimer.clear();

    // update atoms' velocities before thermo computation
    finalIntegrate();

    if thermoEvery > 0 then 
      computeThermo(step, master);
    if debug then writeln("done with iteration ", step);
  }
  
  // Helpful in profiling performance
  if debug then 
    writeln("wipe: ", fobj.wipetime, ", main: ", fobj.maintime);
}
