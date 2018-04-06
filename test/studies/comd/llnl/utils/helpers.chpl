// Any auxilliary functions, classes etc should go here

use MDTypes;
use configs;
use Time;

inline proc dot(const in v1:real3, const in v2:real3) : real {
  var r : real = 0.0;
  for i in 1..3 do r += v1(i) * v2(i);
  return r;
}

class Ticker {
  var name : string;
  var tick : Timer;
  var duration : real;
  var times : int;

  proc init(const in tName : string) {
    this.complete();
    name = tName;
    tick = new Timer();
    tick.clear();
    duration = 0.0;
    times = 0;
  }

  inline proc start() {
    tick.start();
  }

  inline proc stop() : real {
    tick.stop();
    var elapsed : real = tick.elapsed();
    duration += elapsed;
    times += 1;
    tick.clear();
    return elapsed;
  }
	
  inline proc print(loopTime: real) : void {
    writef("%s %8di %12.4dr %12.4dr %12.2dr\n", name, times, duration/times, duration, duration/loopTime * 100);
  }
}

const timers = 19;
enum timerEnum {TOTAL = 1, INIT, FCREATE, INITGRID, EPILOGUE, REDIST1, F1, LOOP, TIMESTEP, POSITION, VELOCITY, REDIST, ATOMHALO, SORT, FORCE, FORCE1, FORCE2, EAMHALO, COMMREDUCE};
var tArray : [1..timers] Ticker = 
(	new Ticker("total              "),
	new Ticker("init               "),
	new Ticker("  createForce      "),
	new Ticker("  initGrid         "),
	new Ticker("  replicateForce   "),
	new Ticker("  redistributeInit "),
	new Ticker("  forceInit        "),
	new Ticker("loop               "),
	new Ticker("timestep           "),
	new Ticker("  position         "),
	new Ticker("  velocity         "),
	new Ticker("  redistribute     "),
	new Ticker("    atomHalo       "),
	new Ticker("    sortAtoms      "),
	new Ticker("  force            "),
	new Ticker("    force1         "),
	new Ticker("    force2         "),
	new Ticker("    eamHalo        "),
	new Ticker("reductions         "));
