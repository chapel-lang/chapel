use Time;

use Configs;
use Util;
use Simulation;

proc main() {

  printSystemInfo();
  Configs.printConfigs();
  printSimulationDataYaml();

  timestampMessage("Initialization Finished");

  timestampMessage("Starting Simulation");
  writeln("#                                                                                         Performance");
  writeln("#  Loop   Time(fs)       Total Energy   Potential Energy     Kinetic Energy  Temperature   (us/atom)     # Atoms");

  var progressTimer : Timer;

  proc getProgress() {
    const ret = progressTimer.elapsed();
    if progressTimer.running then progressTimer.stop();
    progressTimer.clear();
    progressTimer.start();
    return ret;
  }

  for i in 0..#nSteps by printRate {
    sumAtoms();
    printProgress(i, getProgress());

    timestep(printRate, dt);
  }

  sumAtoms();
  printProgress(nSteps, getProgress());
  timestampMessage("Ending Simulation");

  validateResult();

  // Started in Simulation module-init
  Timers["total"].stop();

  writeln();
  printTimings();
}

proc printProgress(step : int, elapsed : real) {
  const time        = step * dt;
  const eTotal      = (ePotential + eKinetic) / numAtoms;
  const eK          = eKinetic / numAtoms;
  const eU          = ePotential / numAtoms;
  const temp        = eK / (kB_eV * 1.5);
  const nEval       = if step == 0 then 1 else printRate;
  const timePerAtom = 1e6 * elapsed / (nEval * numAtoms);

  writef(" %6i %10.2dr %18.12dr %18.12dr %18.12dr %12.4dr %10.4dr %12i\n",
         step, time, eTotal, eU, eK, temp, timePerAtom, numAtoms);
}

proc printSystemInfo() {
  proc uname(cmd : string) {
    // Spawn doesn't work correctly under ugni or gn-ibv:
    //  - https://github.com/chapel-lang/chapel/issues/7550
    //  - https://github.com/chapel-lang/chapel/issues/13387
    const ugni = CHPL_COMM == "ugni";
    const gn_ibv = CHPL_COMM == "gasnet" && CHPL_COMM_SUBSTRATE == "ibv";
    if ugni || gn_ibv then return "unknown";

    use Spawn;
    var sub = spawn(["uname", cmd], stdout=PIPE);
    sub.wait();

    if sub.exit_status == 0 {
      var ret, buf : string;
      while sub.stdout.readline(buf) do ret += buf;
      return ret.strip();
    } else {
      return "unknown";
    }
  }

  writeln("Mini-Application Name    : CoMD-Chapel-DomainMap");
  writeln("Mini-Application Version : 0.1");
  writeln("Platform:");
  writeln("  hostname: ", uname("-n"));
  writeln("  kernel name: '", uname("-s"), "'");
  writeln("  kernel release: '", uname("-r"), "'");
  writeln("  processor: '", uname("-p"), "'");
  writeln("Run Date/Time: ", Util.dateString());
  writeln();
}

proc validateResult() {
  const eFinal = (ePotential + eKinetic) / numAtoms;
  const delta  = numAtoms - initialNumAtoms;

  writeln();
  writeln();
  writeln("Simulation Validation:");
  writef ("  Initial energy  : %14.12dr\n", initialEnergy);
  writef ("  Final energy    : %14.12dr\n", eFinal);
  writef ("  eFinal/eInitial : %dr\n", eFinal/initialEnergy);

  if delta == 0 {
    writef("  Final atom count : %i, no atoms lost\n", numAtoms);
  } else {
    writeln("#############################");
    writef ("# WARNING: %6i atoms lost #\n", delta);
    writeln("#############################");
  }
}

proc printTimings() {
  writeln("Timings across all Locales:");
  writeln("total: ", Timers["total"].elapsed());
  writeln("  position: ", Timers["position"].elapsed());
  writeln("  velocity: ", Timers["velocity"].elapsed());
  writeln("  redistribute: ", Timers["redistribute"].elapsed());
  for (sub, ST) in RedistTimers {
    writeln("    ", sub, ": ", ST.elapsed());
  }
  writeln("  force: ", Timers["computeForce"].elapsed());
  writeln("  commReduce: ", Timers["commReduce"].elapsed());
}
